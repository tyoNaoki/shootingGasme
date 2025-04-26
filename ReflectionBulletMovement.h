#pragma once
#include "Component.h"
#include "Bullet.h"

class CharacterBase;

class ReflectionBulletMovement :
    public Component
{
public:
    ReflectionBulletMovement(std::shared_ptr<Bullet> owner) : mBulletOwner(owner),Component(owner) {}

    void Init() override;
    void Update(const float deltaTime) override;

private:
    int mReflectionMaxCount = 0;
    int mReflectionCurrentCount = 0;
    float mLifeTime = 0.0f;

    std::weak_ptr<Bullet>mBulletOwner;
};

