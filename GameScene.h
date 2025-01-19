#pragma once
#include "Scene.h"

class GameScene :
    public Scene
{
public:
    //mapWidth,mapHeight,
    GameScene(Vector2D<int>mapWorldSize,Vector2D<float>cell);

    ~GameScene();

    //初期化
    bool Initialize() override;

    //更新
    void Update(const float deltaTime) override;

    //描画
    void Render(const float deltaTime) override;

    //終了処理
    void End() override;

    //現在のマップ取得
    std::shared_ptr<Map>GetMap() const override;
    
    //ゲーム内時間取得
    float GetCurrentGameTime();

    //クリアまでの時間取得
    float GetClearGameTime();

    //レベルアップイベント開始処理
    void StartLevelUpEvent(int count)override;

    //レベルアップ画面表示、レベルアップ時の報酬更新など
    void LevelUpEvent()override;

    void GameOver();

    void GameClear();

private:
    //ゲーム内時間更新
    void UpdateTimer(float deltaTime);
    //ボス登場イベント
    void SpawnBoss();
    //初期化フラグ
    bool mInitialze = false;
    //マップの幅
    Vector2D<float>mMapSize;
    //マップの一つのセルサイズ
    Vector2D<float>mCellSize;

    std::shared_ptr<Map> mGameMap;

    //ゲーム内のマップに存在する敵の最大数
    const int mMaxSpawnEnemyCount = 500;

    //一度に何体スポーンさせるか
    int mCurrentSpawnEnemyCount = 0;
    //最初の一度のスポーン数
    int mDefaultSpawnCount = 5;
    //時間ごとにスポーン数を足し算で増やす
    int mLevelUpSpawnCount = 2;
    
    //エネミーAの何体に一回スポーンさせるか
    int mCurrentSpawnEnemyBToSpawnCount = 0;
    const int mSpawnEnemyBToSpawnCount = 10;
    //一度の最大敵Bスポーン数
    const int mMaxSpawnEnemyBCount = 1;

    //敵レベル、生存ボーナスの時間管理
    int mPreLevelUpTime = -1;
    int mPreLevelUpBonusTime = -1;

    //現在の中ボスのスポーンが何番目か
    int mEnemyCspawnIndex = 0;

    //この時間ごとに敵は1レベルアップする
    int mEnemyLevelUpTime = 20;

    //敵のスポーン間隔
    float mCurrentNextSpawnTime = 0.0f;
    const float mDefaultSpawnTime = 3.0f;
    float mCurrentEnemySpawnTime = 0.0f;
    //float mLevelUpSpawnTime = 1.8f;
    float mLevelUpSpawnTime = 0.1f;

    //生存経験値ボーナス時間
    const float mStartLevelUpBonusTime = 180.0f;
    const int mDefaultBonusTime = 20;
    //float mCurrentBonusTime = 0.0f;

    //ゲーム内時間
    float mCurrentGameTime = 0.0f;
    //ゲームクリア時間
    const float mGameClearTime = 300.0f;
    //ボス登場時間
    const float mGameBossSpawnTime = 210.0f;

    //敵スポーン名
    std::string mSpawnEnemyName = "NormalEnemyA";
    std::string mSpawnEnemyNameB = "NormalEnemyB";
    std::string mSpawnEnemyNameC = "NormalEnemyC";

    //初期レベル
    const int mPlayerStartLevel = 1;
    const int mEnemyStartLevel = 1;

    //シーン遷移時間
    float mChangeSceneTime = 5.0f;

    //フラグ
    bool mGameOver = false;
    bool mGameClear = false;
};
