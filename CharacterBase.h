#pragma once
#include "Actor.h"
#include<string>
#include <memory>
#include <vector>
#include "Collision.h"
#include "StatusManager.h"

class Component;

class CharacterBase :
    public Actor,public std::enable_shared_from_this<CharacterBase>
{
public:
    CharacterBase();

    ~CharacterBase();

    virtual void Init(CharacterType ct,std::string typeName, int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) = 0;

    virtual void Reset(int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) = 0;

    virtual bool IsInitialize() override = 0;
    virtual void Update(const float deltaTime) override;
    virtual void Draw(const float deltaTime) override;

    //virtual void SetPosition2D(Vector2D<float> localP) override;

    virtual void SetWorldPosition2D(Vector2D<float>worldP) override;

    virtual void SetWorldPosition2D(Vector2D<float>worldP,bool isMove);

    int GetLevel();

    float GetHP();

    float GetMaxHP();

    float GetScore();

    virtual float GetMoveSpeed() const;

    virtual void TakeDamage(float damage);

    void StartDeadAnimation()override;

    void FinishDeadAnimation()override;

    void DamageAnimation();

    void InitKnockBack();

    void AddKnockBack(Vector2D<float> direction, float strength, float knockBackTime);

    void KnockBack(float deltaTime);

    bool IsKnockBack();

    bool IsDead();

protected:
    int mLevel = 0;

    float mHP = 0.0f;
    float mMaxHP = 0.0f;

    float mScore = 0.0f;

    float mCurrent_DamageAnimFrame = 0.0f;

    float mDamage_MaxFrame = 0.2f;

    bool mKnockBack = false;

    bool mDamage = false;

    bool mIsDead = false;

private:
    const std::string mHitSound = "Hit";

    Vector2D<float> mKnockBackDirection; //è’åÇ

    float mKnockBackTime = 0.0f;

    float mCurrentKnockBackTime = 0.0f;

    float mKnockBackStrength = 0.0f;
};

