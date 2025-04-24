#pragma once
#include "Component.h"
#include "WeaponStatus.h"
#include "CharacterBase.h"

class WeaponComponent :
    public Component
{
public:
    WeaponComponent(std::shared_ptr<CharacterBase> owner) : Component(owner){};

    virtual void SetStatus(std::shared_ptr<WeaponStatus>newStatus) {};
};

