#pragma once
#include "Component.h"

class CharacterBase;

class EnemyChaseMoveComponent :
    public Component
{
public:
    EnemyChaseMoveComponent(std::shared_ptr<CharacterBase>owner) :Component(owner) {};

    void Update(const float deltaTime) override;
};

