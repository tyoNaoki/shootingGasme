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
<<<<<<< HEAD
    CharacterBase();

    ~CharacterBase();

=======
    CharacterBase(Collision::Rect collision);

    ~CharacterBase();

    std::shared_ptr<Component>GetComponent(int idx);
    std::shared_ptr<Component>GetComponent(std::string name);
    std::vector<std::shared_ptr<Component>>GetComponents();

    void AddComponent(std::shared_ptr<Component> component,int idx);
    void RemoveComponent(int idx);
    void InitComponents();

>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
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

    virtual void TakeDamage(float damage, Vector2D<float>direction = Vector2D<float>(), float strength = 0.0f, float knockBackTime = -1.0f);

    void StartDeadAnimation()override;

    void FinishDeadAnimation()override;

    void DamageAnimation();

    void InitKnockBack();

    void AddKnockBack(Vector2D<float> direction, float strength, float knockBackTime);

    void KnockBack(float deltaTime);
<<<<<<< HEAD
=======
    
    const Collision::Rect& GetCollision() const;
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420

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
<<<<<<< HEAD
=======

    Collision::Rect mCollision;

    std::vector<std::shared_ptr<Component>> mComponents;
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
};

