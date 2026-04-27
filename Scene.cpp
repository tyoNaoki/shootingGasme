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
	//レベルアップ回数を減らす
	mLevelUpCount--;
	//レベルアップイベントが一回終了したことを通知する
	FinishLevelUp();
}

void Scene::FinishLevelUp()
{
	mIsLevelUp = false;
	//レベルアップ回数が残っているなら、レベルアップイベントを開始
	if(mLevelUpCount>0){
		LevelUpEvent();
	}
}
