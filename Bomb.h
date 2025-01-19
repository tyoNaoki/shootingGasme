#pragma once
#include "Actor.h"
#include <unordered_map>

class CharacterBase;

class Bomb :
    public Actor
{
public:
    Bomb(Vector2D<float>drawOffset);

    Bomb() = delete;
    ~Bomb();

    void Init(CharacterType ct, CharacterType ownerType, std::string typeName, int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible);

    void Reset(CharacterType ownerType, int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible);

    bool IsInitialize()override;
    void Update(const float deltaTime) override;
    void Draw(const float deltaTime) override;

    void SetStatus(Vector2D<float>bombSize, Vector2D<float>explosionSize,float explosionAnimSpeed,float attack,float knockBack,float timeToExplode,float explosionTime,float explosionRange,bool hasContinuousDamage = false);

    void ApplyDrawOffset(Vector2D<float>offset);

    bool IsExplosion()const;

    //ダメージのクールタイムが過ぎているかどうか
    bool IsDamage(std::shared_ptr<CharacterBase>&enemy);

    void HitCheck(float deltaTime,std::shared_ptr<CharacterBase> enemy, const Collision::Circle& circle, float knockBackStrength, float attack,bool hasContinuousDamage);

private:
    int mCurrent_gHandle = -1;
    const std::string soundName = "ExplosionSound";
    
    float mAttack = 0.0f;
    float mExplosionTime = 1.0f; // 爆発の持続時間
    float mExplosionRange = 0.0f;
    float mKnockBackStrength = 0.0f;

    bool mInitialize = false;
    float currentTime = 0.0f;
    bool mIsExplosion = false;
    bool mHasContinuousDamage = false;

    CharacterType mOwnerType = CharacterType::EMPTY;
    std::vector<std::weak_ptr<CharacterBase>>mIgnoreCharas;
    Vector2D<float>mDrawOffset;
    Vector2D<float>mBombSize;
    float mExplosionAnimSpeed;

    const float mHitCoolTime = 0.4f;
    std::unordered_map<std::string, float> mHitCountEnemies; // 敵のダメージ回数記録
};

