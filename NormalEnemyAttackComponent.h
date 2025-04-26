#pragma once
#include "WeaponComponent.h"

//雑魚敵のプレイヤー衝突時のダメージを与えるコンポーネントクラス
class NormalEnemyAttackComponent : public WeaponComponent{
public:
    // コンストラクタ：キャラクターの所有者を初期化
    NormalEnemyAttackComponent(std::shared_ptr<CharacterBase> owner);

    // 初期化関数
    void Init() override;

    // 更新関数
    void Update(const float deltaTime) override;

    // ステータス設定関数
    void SetStatus(std::shared_ptr<WeaponStatus> status) override;

private:
    // 攻撃力
    float mAttack = 0.0f;

    // 攻撃の間隔
    const float mAttackTime = 3.0f;

    // 現在の時間
    float currentTime = 0.0f;
};


