#include "Scene.h"

SceneState Scene::GetCurrentState() const
{
	return mCurrentState;
}

void Scene::ChangeScene(SceneState newScene)
{
	mCurrentState = newScene;
}

void Scene::LevelUpRewardSelected()
{
	//���x���A�b�v�񐔂����炷
	mLevelUpCount--;
	//���x���A�b�v�C�x���g�����I���������Ƃ�ʒm����
	FinishLevelUp();
}

void Scene::FinishLevelUp()
{
	mIsLevelUp = false;
	//���x���A�b�v�񐔂��c���Ă���Ȃ�A���x���A�b�v�C�x���g���J�n
	if(mLevelUpCount>0){
		LevelUpEvent();
	}
}
