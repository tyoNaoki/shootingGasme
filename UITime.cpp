#include "UITime.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "PlayerCharacter.h"
#include <iomanip>
#include <sstream>

UITime::UITime(int fontHandle, unsigned int color,bool isReverse) :mFontHandle(fontHandle), mColor(color),IsReverse(isReverse) {
	player = ACTOR_M.GetCurrentPlayer();
}

UITime::~UITime()
{
	//時間テキスト消去
	DeleteFontToHandle(mFontHandle);
	mScene = nullptr;
}

void UITime::Update(float deltaTime)
{
	if(!mScene){
		mScene = std::dynamic_pointer_cast<GameScene>(SCENE_M.GetCurrentScene());
	}
	//クリアまでの時間を表示する
	if(IsReverse){
		mCurrentTime = mScene->GetClearGameTime()-mScene->GetCurrentGameTime();
		if(mCurrentTime<0){mCurrentTime = 0;}
	}else{//現在のゲーム内経過時間を表示する
		mCurrentTime = mScene->GetCurrentGameTime();
	}
}

void UITime::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }
	//時間を取得
	std::string text = GetMinutes(mCurrentTime);
	//時間を描画
	DrawStringToHandle(mLocalPosition.x, mLocalPosition.y, text.c_str(), mColor, mFontHandle);
}

std::string UITime::GetMinutes(float gameTime)
{
	//分
	int minutes = static_cast<int>(gameTime) / 60;
	//秒
	int secs = static_cast<int>(gameTime) % 60;

	//テキスト設定
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << secs;

	return ss.str();
}
