#pragma once
#include "Component.h"
#include "WeaponStatus.h"
#include <vector>
#include "Laser.h"

enum class BossLaserState {
    SetUp,Alert,FireLaser,None
};

class PlayerCharacter;
class BossBase;

class BossLaserComponent :
    public Component
{
public:
    BossLaserComponent(std::shared_ptr<CharacterBase>owner);

    void Update(const float deltaTime) override;

    void SetStatus(std::shared_ptr<WeaponStatus>status)override;

private:
    bool IsPlaySound(float deltaTime);

    bool IsPlayAlertBlend(float deltaTime);

    void CurrentPlaySound();

    void CurrentStopSound();

    void StartAlertSound();

    void StartLaserSound();

    void FireLaser(std::shared_ptr<CharacterBase> owner, float targetRot);

    void FinishLaser();

    std::vector<std::shared_ptr<Laser>>lasers;

    std::shared_ptr<PlayerCharacter>mPlayer;
    std::weak_ptr<BossBase>mBossOwner;

    float mCurrentTime = 0.0f;
    float mLaserAlerTime = 2.0f;

    float mLaserDamage = 0.0f;
    float mLaserSpeed = 0.0f;
    float mLaserWidthSize = 0.0f;
    float mLaserSpreadAngle = 0.0f;
    int mLaserCount = 0;
    float mLaserKnockBack = 0.0f;
    float mLaserKnockBackTime = 0.0f;
    float mFireTime = 0.0f;

    std::string mPlaySound;
    float mSoundTime = 0.0f;
    float mSoundInterval = 0.0f;

    float mAlertBlendTime = 0.0f;
    float mAlertBlendStartInterval = 0.2f;
    float mAlertBlendFinishInterval = 0.3f;

    const Vector2D<float>mLaserPositionOffset = Vector2D<float>(0,-70);

    BossLaserState mCurState;
};

