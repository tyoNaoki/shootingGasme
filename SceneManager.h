#pragma once
#include <vector>
#include <unordered_map>
#include "Singleton.h"
#include "Scene.h"
#include "GameSystemManager.h"

#define SCENE_M Singleton<SceneManager>::get_instance()

class Scene;

// シーンマネージャクラス
class SceneManager
{
    friend class Singleton<SceneManager>;

public:
    // 初期化関数
    bool Initialize();

    // 実行関数
    bool Run(const float deltaTime);

    // 更新関数
    void Update(const float deltaTime) const;

    // 描画関数
    void Render(const float deltaTime) const;

    // シーン追加関数
    void AddScene(std::shared_ptr<Scene> scene, int idx);

    // 現在のシーン取得関数
    std::shared_ptr<Scene> GetCurrentScene() const;

    // シーンインデックス取得関数
    int GetSceneIndex(std::string sceneName) const;

    // レベルアップイベント処理関数
    void OnLevelUpEvent(int count);

    // レベルアップ報酬選択時の処理関数
    void OnLevelUpRewardSelected();

    // シーン変更関数
    void ChangeScene(SceneState newScene);

    // ゲーム終了関数
    void ExitGame();

private:
    // コンストラクタ
    SceneManager();
    // デストラクタ
    ~SceneManager();

    // ポーズ状態チェック関数
    void PauseCheck(SceneState& state, float deltaTime);

    std::vector<std::shared_ptr<Scene>> mSceneList; // シーンリスト

    int mCurrent_SceneIndex = 0; // 現在のシーンインデックス

    int mNext_SceneIndex = -1; // 次のシーンインデックス

    bool mInitialized = false; // 初期化フラグ

    float mCoolTime_pause = 0.0f; // ポーズのクールタイム

    bool mIsExitGame = false; // ゲーム終了フラグ
};


