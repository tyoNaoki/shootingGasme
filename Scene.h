#pragma once
#include <memory>
#include "Map.h"
#include <unordered_map>
#include "DxLib.h"
#include <string>

class Map;
enum class SceneState { PLAYING, NEXTSCENE, FINISH };

// シーンクラスの定義
class Scene {
public:
    virtual ~Scene() = default;

    // 初期化関数（純粋仮想関数）
    virtual bool Initialize() = 0;

    // 更新関数（純粋仮想関数）
    virtual void Update(const float deltaTime) = 0;

    // 描画関数（純粋仮想関数）
    virtual void Render(const float deltaTime) = 0;

    // 現在のシーン状態を取得する関数
    SceneState GetCurrentState() const;

    // シーンを変更する関数
    void ChangeScene(SceneState newScene);

    // マップを取得する関数（純粋仮想関数）
    virtual std::shared_ptr<Map> GetMap() const = 0;

    // シーン名を取得する関数
    std::string GetSceneName() const { return mSceneName; }

    // ポーズ状態を設定する関数
    void SetPaused(bool isPause) { mIsPaused = isPause; }

    // ポーズ状態を取得する関数
    bool IsPaused() { return mIsPaused; }

    // ポーズが有効かどうかを確認する関数
    bool IsPauseEnabled() const { return mPauseEnabled; }

    // レベルアップ状態を設定する関数
    void SetIsLevelUp(bool isLevelUp) { mIsLevelUp = isLevelUp; }

    // レベルアップ状態を確認する関数
    bool IsLevelUp() { return mIsLevelUp; }

    // レベルアップイベントを開始する関数（オーバーライド可能）
    virtual void StartLevelUpEvent(int count) {}

    // レベルアップイベント関数（オーバーライド可能）
    virtual void LevelUpEvent() {}

    // レベルアップ報酬が選択された際の処理
    void LevelUpRewardSelected();

    // レベルアップが終了した際の処理
    void FinishLevelUp();

    // 終了処理関数（純粋仮想関数）
    virtual void End() = 0;

protected:
    std::string mSceneName = "EMPTY"; // シーン名
    bool mIsPaused = false; // ポーズ状態
    bool mPauseEnabled = true; // ポーズが有効か
    bool mIsLevelUp = false; // レベルアップ状態
    int mLevelUpCount = 0; // レベルアップ回数

private:
    SceneState mCurrentState; // 現在のシーン状態
};


