#pragma once
#include "Component.h"
#include "InputManager.h"
#include "CharacterBase.h"

// プレイヤーの移動を制御するコンポーネントクラス
class PlayerMovementComponent : public Component{
public:
    // コンストラクタ：キャラクターの所有者を初期化
    PlayerMovementComponent(std::shared_ptr<CharacterBase> owner) :mMovementOwner(owner), Component(owner) {
        mComponentName = "PlayerMovementComponent";
    }

    // 更新関数
    void Update(const float deltaTime) override;

private:
    std::weak_ptr<CharacterBase>mMovementOwner;
};


