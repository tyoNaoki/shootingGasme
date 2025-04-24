#pragma once
#include "WeaponComponent.h"

// プレイヤーの銃を扱うコンポーネントクラス
class PlayerGunComponent : public WeaponComponent {
public:
    // コンストラクタ：キャラクターの所有者を初期化
    PlayerGunComponent(std::shared_ptr<CharacterBase> owner);

    // 初期化関数
    void Init() override;

    // 更新関数
    void Update(const float deltaTime) override;

    // ステータス設定関数
    void SetStatus(std::shared_ptr<WeaponStatus> status) override;

private:
    float mAttack = 0.0f; // 攻撃力
    float mShotRate = 0.2f; // 発射速度
    bool mIsPenetration = false; // 貫通の有無
    int mPenetrationMaxCount = 0; // 最大貫通回数
    float mCurrentTime = 0.0f; // 現在の時間

    const std::string mBulletName = "PlayerBullet"; // 弾の名前
    const std::string mFireSound = "FireBulletSound"; // 発射音

    float mLockOnRange = 400.0f; // ロックオン範囲
    float mBulletSpeed = 1000.0f; // 弾の速度
};


