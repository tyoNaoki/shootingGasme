#pragma once
#include "WeaponComponent.h"

// 爆弾を投げるプレイヤーのコンポーネントクラス
class PlayerThrowBombComponent : public WeaponComponent {
public:
    // コンストラクタ：キャラクターの所有者を初期化
    PlayerThrowBombComponent(std::shared_ptr<CharacterBase> owner);
    // 初期化関数
    void Init() override;

    // 更新関数
    void Update(const float deltaTime) override;

    // ステータス設定関数
    void SetStatus(std::shared_ptr<WeaponStatus> status) override;

    //ボムを投げる位置を乱数で計算する関数
    Vector2D<float> CalcRandomSlowPosition(std::shared_ptr<CharacterBase> owner, float radius, std::shared_ptr<Map> map);

private:
    float mAttack = 0.0f; // 攻撃力
    float mThrowRate = 0.0f; // 投擲速度
    float mExplosionRange = 0.0f; // 爆発範囲
    float mExplosionTime = 0.0f; // 爆発時間
    float mTimeToExplode = 0.0f; // 爆発までの時間
    float mKnockBackStrength = 0.0f; // ノックバック強度
    float mThrowRange = 0.0f; // 投擲距離
    bool mHasContinuousDamage = false; // 持続ダメージの有無

    const std::string mBombName = "PlayerBomb"; // 爆弾の名前
    const std::string mSlowSound = "FireBulletSound"; // スローサウンド

    const Vector2D<float> mDefaultBombSize = Vector2D<float>(6.1, 6.1); // デフォルトの爆弾サイズ

    const float mThrowRangeMin = 200.0f; // 最小投擲距離

    float mCurrentTime = 0.0f; // 現在の時間
    float mDefaultExplosionRange; // デフォルトの爆発範囲
    float mDefaultExplosionTime; // デフォルトの爆発時間
    const float defaultPlaySpeed = 1.0f; // デフォルトのプレイスピード
};


