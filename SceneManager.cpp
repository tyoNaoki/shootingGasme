#include "SceneManager.h"
#include "PlayerFactory.h"
#include "Game.h"

#include "GameScene.h"
#include "TitleScene.h"
#include "MenuScene.h"
#include <memory>
#include "EffectBase.h"
#include "DebugHelper.h"
#include "EventManager.h"
#include "ActorManager.h"

SceneManager::SceneManager(){

}

SceneManager::~SceneManager()
{
    //Singleton<ActorManager>::get_instance().End();
    mSceneList.clear();
    //Effect_Finalize();
}

void SceneManager::PauseCheck(SceneState& state, float deltaTime)
{
    // 現在のシーンがポーズをサポートしていない場合
    if (mSceneList[mCurrent_SceneIndex]->IsPauseEnabled() == false) {
        mSceneList[mCurrent_SceneIndex]->SetPaused(false); // シーンをポーズ解除
        mCoolTime_pause = 0.0f; // ポーズクールタイムをリセット
        return; // 処理を終了
    }

    // PキーまたはESCキーが押された場合
    if (Singleton<InputManager>::get_instance().GetPushKeyFrame(KEY_INPUT_P) > 0 || Singleton<InputManager>::get_instance().GetPushKeyFrame(KEY_INPUT_ESCAPE) > 0) {
        if (state == SceneState::PLAYING && !mSceneList[mCurrent_SceneIndex]->IsPaused()) {
            if (mCoolTime_pause <= 0.0f) {
                //bool isPause = mSceneList[mCurrent_SceneIndex]->IsPaused()?false:true;
                bool isPause = true; // ポーズ状態を切り替えるためにtrueに設定
                mSceneList[mCurrent_SceneIndex]->SetPaused(isPause); // シーンをポーズ
                mCoolTime_pause = 1.0f; // ポーズクールタイムを設定
            }
        }
    }
    else {
        if (mCoolTime_pause > 0.0f) {
            mCoolTime_pause -= deltaTime; // クールタイムを減少
        }
    }
}

bool SceneManager::Initialize()
{
    // すでに初期化済みの場合はtrueを返す
    if (mInitialized) { return true; }

    // シーンを追加
    AddScene(std::make_shared<TitleScene>(), 0);
    AddScene(std::make_shared<MenuScene>(), 1);
    AddScene(std::make_shared<GameScene>(Vector2D<int>(4000, 4000), Vector2D<float>(40, 40)), 2);

    // ActorManagerを初期化
    Singleton<ActorManager>::get_instance().Init();

    // 現在のシーンと次のシーンのインデックスを設定
    mCurrent_SceneIndex = 0;
    mNext_SceneIndex = 1;

    // 初期化フラグを設定
    mInitialized = true;

    // 初期化成功を返す
    return true;
}

bool SceneManager::Run(const float deltaTime)
{
    // 初期化が成功しない場合はfalseを返す
    if (!Initialize()) {
        return false;
    }

    // 現在のシーンが存在しない場合はfalseを返す
    if (!mSceneList[mCurrent_SceneIndex]) {
        return false;
    }

    // 現在のシーンの初期化が成功しない場合はfalseを返す
    if (!mSceneList[mCurrent_SceneIndex]->Initialize()) {
        return false;
    }

    // 現在のシーンの状態を取得
    auto curSceneState = mSceneList[mCurrent_SceneIndex]->GetCurrentState();

    // ポーズチェックを行う
    PauseCheck(curSceneState, deltaTime);

    // ゲーム終了フラグが立っている場合
    if (mIsExitGame) {
        curSceneState = SceneState::FINISH;
    }

    // シーンの状態が次のシーンの場合
    if (curSceneState == SceneState::NEXTSCENE) {
        mSceneList[mCurrent_SceneIndex]->End(); // 現在のシーンを終了
        mCurrent_SceneIndex = mNext_SceneIndex; // 次のシーンに移行
        if (mNext_SceneIndex + 1 >= mSceneList.size()) {
            mNext_SceneIndex = 0; // シーンのリストの終わりに達したら最初に戻る
        }
        else {
            mNext_SceneIndex++; // 次のシーンに進む
        }
        return true;
    }

    // シーンの状態が終了の場合
    if (curSceneState == SceneState::FINISH) {
        mSceneList[mCurrent_SceneIndex]->End(); // 現在のシーンを終了
        return false;
    }

    // シーンの状態が再生中の場合
    if (curSceneState == SceneState::PLAYING) {
        Update(deltaTime); // シーンを更新

        Render(deltaTime); // シーンを描画
    }

    return true;
}


void SceneManager::Update(const float deltaTime)const
{
    mSceneList[mCurrent_SceneIndex]->Update(deltaTime);
}

void SceneManager::Render(const float deltaTime)const
{
    mSceneList[mCurrent_SceneIndex]->Render(deltaTime);
}

void SceneManager::AddScene(std::shared_ptr<Scene> scene, int idx)
{
    mSceneList.insert(mSceneList.begin() + idx,scene);
}

int SceneManager::GetSceneIndex(std::string sceneName)const
{
    int index = 0;
    for(auto&s : mSceneList){
        if(s->GetSceneName()==sceneName){
            return index;
        }
        index++;
    }

    //見つからない場合、変わりにー１
    return -1;
}

void SceneManager::OnLevelUpEvent(int count)
{
    GetCurrentScene()->StartLevelUpEvent(count);
}

void SceneManager::OnLevelUpRewardSelected()
{
    GetCurrentScene()->LevelUpRewardSelected();
}

void SceneManager::ChangeScene(SceneState newScene)
{
    GetCurrentScene()->ChangeScene(newScene);
}

void SceneManager::ExitGame()
{
    mIsExitGame = true;
}

std::shared_ptr<Scene> SceneManager::GetCurrentScene()const
{
    //シーンが存在しない場合、NULL
    if(mSceneList.size()==0){
        return nullptr;
    }

    return mSceneList[mCurrent_SceneIndex];
}
