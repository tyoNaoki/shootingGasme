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
	//���ԃe�L�X�g����
	DeleteFontToHandle(mFontHandle);
	mScene = nullptr;
}

void UITime::Update(float deltaTime)
{
	if(!mScene){
		mScene = std::dynamic_pointer_cast<GameScene>(SCENE_M.GetCurrentScene());
	}
	//�N���A�܂ł̎��Ԃ�\������
	if(IsReverse){
		mCurrentTime = mScene->GetClearGameTime()-mScene->GetCurrentGameTime();
		if(mCurrentTime<0){mCurrentTime = 0;}
	}else{//���݂̃Q�[�����o�ߎ��Ԃ�\������
		mCurrentTime = mScene->GetCurrentGameTime();
	}
}

void UITime::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }
	//���Ԃ��擾
	std::string text = GetMinutes(mCurrentTime);
	//���Ԃ�`��
	DrawStringToHandle(mLocalPosition.x, mLocalPosition.y, text.c_str(), mColor, mFontHandle);
}

std::string UITime::GetMinutes(float gameTime)
{
	//��
	int minutes = static_cast<int>(gameTime) / 60;
	//�b
	int secs = static_cast<int>(gameTime) % 60;

	//�e�L�X�g�ݒ�
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << secs;

	return ss.str();
}
