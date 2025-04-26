#pragma once
#include "Component.h"
#include "CharacterBase.h"

class EnemyChaseMoveComponent : public Component{
public:
    EnemyChaseMoveComponent(std::shared_ptr<CharacterBase>owner) :mMoveOwner(owner),Component(owner) {};

    void Update(const float deltaTime) override;

private:
    std::weak_ptr<CharacterBase>mMoveOwner;
};

