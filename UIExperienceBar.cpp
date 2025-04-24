#include "UIExperienceBar.h"
#include "StatusManager.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"

UIExperienceBar::UIExperienceBar(float width, float height, float value, float maxValue, int barColor, int rectColor) :UIBar(width, height, value, maxValue, barColor, rectColor)
{
}

void UIExperienceBar::Update(float deltaTime)
{
	//現在の取得経験値取得
	float exp = ACTOR_M.GetCurrentPlayer()->GetCurrentExp();

	//現在のレベル取得
	int level = STATUS.GetCurrentPlayerStatus().level;

	//次のレベルアップのための必要経験値取得
	float maxExp = STATUS.GetNeedExp(level);

	//経験値が必要経験値を超えているなら、必要経験値に取得経験値を設定
	if(exp>maxExp){
		exp = maxExp;
	}

	//数値設定
	SetValue(exp);
	SetMaxValue(maxExp);
	//更新
	UIBar::Update(deltaTime);
}

