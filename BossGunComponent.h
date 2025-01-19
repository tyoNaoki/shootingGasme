#pragma once
#include "Component.h"

class Bullet;
class PlayerCharacter;
class BossBase;

class BossGunComponent :
    public Component
{
public:
    BossGunComponent(std::shared_ptr<CharacterBase>owner);

    void Init()override;

    void Update(const float deltaTime) override;

    void SetStatus(std::shared_ptr<WeaponStatus>status)override;

private:
    float mSpeed = 0.0f;
    float mAttack = 0.0f;
    float mHomingTime = 0.0f;
    float mFireTime = 3.0f;
    float mCurrentTime = 0.0f;

    bool mIsHoming = false;

    bool mIsFire = false;
    void FireBullet(std::shared_ptr<Actor>owner,std::shared_ptr<CharacterBase> homingTarget);

    const std::string soundName = "BossFireBulletSound";

    std::shared_ptr<PlayerCharacter>mPlayer;
    std::weak_ptr<BossBase>mBossOwner;
};

