#pragma once
#include "CharacterBase.h"

class PlayerCharacter;

enum class BossState {
    Wait,
    Move,
    Attack,
    Dead,
    None
};

class BossBase :
    public CharacterBase
{
public:
    BossBase() = delete;
    BossBase(Collision::Rect rect);
    ~BossBase();

    virtual void Init(CharacterType ct, std::string name, int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;

    void Reset(int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;

    virtual void Update(const float deltaTime) override;

    virtual void Draw(const float deltaTime) override;

    bool IsInitialize() override;

    float GetMoveSpeed() const override;

    bool IsState(BossState targetState);

    void SetAlert(bool isAlert);

    BossState GetCurrentState();

    std::string GetCurrentWeaponName()const;

    virtual void FinishState(){};

protected:
    std::shared_ptr<PlayerCharacter>mPlayer;
    bool mTakeKnockBack = false;

    void ChangeState(BossState newState);

    bool mAlert = false;

    std::string mWeaponName;
private:
    float mMoveSpeed = 0.0f;

    float mScore = 0.0f;

    bool mInitialize = false;

    BossState mState = BossState::Move;
};

