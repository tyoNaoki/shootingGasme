#pragma once
#include "Component.h"
#include "WeaponStatus.h"

class PlayerCharacter;
class BossBase;

class BossTackleComponent :
    public Component
{
public:
    BossTackleComponent(std::shared_ptr<CharacterBase>owner);

    void Update(const float deltaTime) override;

    void SetStatus(std::shared_ptr<WeaponStatus>status)override;

private:
    std::shared_ptr<PlayerCharacter>mPlayer;
    std::weak_ptr<BossBase>mBossOwner;

    float mCurrentTime = 0.0f;
    float mTackleCoolTime = 1.0f;

    float mTackleDamage = 0.0f;
    float mTackleKnockBack = 0.0f;
};

