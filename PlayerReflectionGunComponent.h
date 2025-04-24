#pragma once
<<<<<<< HEAD
#include "WeaponComponent.h"

// 反射銃を扱うプレイヤーのコンポーネントクラス
class PlayerReflectionGunComponent : public WeaponComponent {
=======
#include "Component.h"

// 反射銃を扱うプレイヤーのコンポーネントクラス
class PlayerReflectionGunComponent : public Component {
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
public:
    // コンストラクタ：キャラクターの所有者を初期化
    PlayerReflectionGunComponent(std::shared_ptr<CharacterBase> owner);

    // 初期化関数
    void Init() override;

    // 更新関数
    void Update(const float deltaTime) override;

    // ステータス設定関数
    void SetStatus(std::shared_ptr<WeaponStatus> status) override;

private:
    float mAttack = 0.0f; // 攻撃力
    float mShotRate = 0.2f; // 発射速度
    int mReflectionCount = 0; // 反射回数
    float mBulletLifeTime = 0.0f; // 弾の寿命
    float mCurrentTime = 0.0f; // 現在の時間

    const std::string mBulletName = "PlayerReflectionBullet"; // 弾の名前
    const std::string mFireSound = "MagicBulletSound"; // 発射音

    float mLockOnRange = 400.0f; // ロックオン範囲
    float mBulletSpeed = 1000.0f; // 弾の速度
};


