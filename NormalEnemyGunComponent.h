#pragma once
#include "WeaponComponent.h"
#include <chrono>
#include <thread>
#include <random>

//雑魚敵の銃を扱うコンポーネントクラス
class NormalEnemyGunComponent : public WeaponComponent{
public:
    // キャラクターの所有者を初期化
    NormalEnemyGunComponent(std::shared_ptr<CharacterBase> owner);

    // 初期化関数
    void Init() override;
    // 更新関数
    void Update(const float deltaTime) override;

    // ステータス設定関数
    void SetStatus(std::shared_ptr<WeaponStatus> status) override;

private:
    // 弾を発射する関数
    void FireBullet(std::shared_ptr<Actor> owner, float targetRot);

    float mAttack = 0.0f; // 攻撃力
    float mShotRate = 0.2f; // 発射速度
    float mBurstRate = 1.0f; // バースト速度
    int mBurstCount = 0; // 一度のバーストで発射する数
    int mMaxBurstCount = 3; // 最大バースト数
    float mCurrentTime = 0.0f; // 現在の時間

    const std::string mBulletName = "EnemyBullet"; // 弾の名前
    float mLockOnRange = 400.0f; // ロックオン範囲
    float mBulletSpeed = 1000.0f; // 弾の速度

    // ランダム生成器と分布
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution{ 0.1f, 2.0f };
};


