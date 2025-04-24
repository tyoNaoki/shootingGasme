#pragma once
<<<<<<< HEAD
#include "WeaponComponent.h"
#include <functional>

// 通常敵の近接攻撃を制御するコンポーネントクラス
class NormalEnemyMeleeComponent : public WeaponComponent {
=======
#include "Component.h"
#include <functional>

// 通常敵の近接攻撃を制御するコンポーネントクラス
class NormalEnemyMeleeComponent : public Component {
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
public:
    //キャラクターの所有者を初期化
    NormalEnemyMeleeComponent(std::shared_ptr<CharacterBase> owner);

    // 初期化関数
    void Init() override;
    // 更新関数
    void Update(const float deltaTime) override;

    // ステータス設定関数
    void SetStatus(std::shared_ptr<WeaponStatus> status) override;

    // 近接攻撃の描画関数
    void DrawMelee(std::shared_ptr<CharacterBase> owner, Collision::RotatedRect& rotRect);

private:
    float attack = 0.0f; // 攻撃力
    float meleeSpeed = 0.5f; // 近接攻撃の速度
    float currentTime = 0.0f; // 現在の時間

    const float drawTime = 0.2f; // 描画時間
    float meleeRange = 200.0f; // 近接攻撃の範囲
    float meleeWidthRange = 50.0f; // 近接攻撃の幅範囲
    float meleeHeightRange = 150.0f; // 近接攻撃の高さ範囲
    float meleeShock = 1000.0f; // 近接攻撃の衝撃力

    const std::string mActionName = "Spore"; // アクション名
};


