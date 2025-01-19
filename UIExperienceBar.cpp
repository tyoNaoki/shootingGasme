#include "UIExperienceBar.h"
#include "StatusManager.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"

UIExperienceBar::UIExperienceBar(float width, float height, float value, float maxValue, int barColor, int rectColor) :UIBar(width, height, value, maxValue, barColor, rectColor)
{
}

void UIExperienceBar::Update(float deltaTime)
{
	//���݂̎擾�o���l�擾
	float exp = ACTOR_M.GetCurrentPlayer()->GetCurrentExp();

	//���݂̃��x���擾
	int level = STATUS.GetCurrentPlayerStatus().level;

	//���̃��x���A�b�v�̂��߂̕K�v�o���l�擾
	float maxExp = STATUS.GetNeedExp(level);

	//�o���l���K�v�o���l�𒴂��Ă���Ȃ�A�K�v�o���l�Ɏ擾�o���l��ݒ�
	if(exp>maxExp){
		exp = maxExp;
	}

	//���l�ݒ�
	SetValue(exp);
	SetMaxValue(maxExp);
	//�X�V
	UIBar::Update(deltaTime);
}

