#pragma once
#include "Actor.h"
#include <unordered_map>

class CharacterBase;

class Bullet :
    public Actor
{
public:
<<<<<<< HEAD
    Bullet(Vector2D<float>drawOffset,Vector2D<float>extendSize);
=======
    

    Bullet(Collision::Circle collision,Vector2D<float>drawOffset,Vector2D<float>extendSize);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420

    Bullet() = delete;
    ~Bullet();

    void Init(CharacterType ct, CharacterType ownerType,std::string typeName, int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible);

    void Reset(CharacterType ownerType,int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible);

    bool IsInitialize()override;
    void Update(const float deltaTime) override;
    void Draw(const float deltaTime) override;

    void StartDeadAnimation()override;
    void FinishDeadAnimation()override;

    //貫通弾
    void SetStatus(float speed,float attack,bool IsPenetration,int penetrationMaxCount);

    //ホーミング弾
    void SetStatus(float speed, float attack,float homingTime,std::shared_ptr<CharacterBase>homingTarget);

    //反射弾
    void SetStatus(float speed, float attack, float lifeTime,int reflectionMaxCount);

    //ワールド座標設定
    void SetWorldPosition2D(Vector2D<float> worldPosition) override;

    void ApplyDrawOffset(Vector2D<float>offset);

    virtual float GetMoveSpeed() const;

    bool IsHit();

    bool IsHomingBullet();

    bool IsPenetrationBullet();

private:
    bool IsFinish(std::shared_ptr<CharacterBase>target);

    bool IsPenetratingHit(std::shared_ptr<CharacterBase>target);

    bool CanPenetratingOnCurrentCount();

<<<<<<< HEAD
=======
    Collision::Circle mCollision;

>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
    CharacterType mOwnerType = CharacterType::EMPTY;

    std::vector<std::weak_ptr<CharacterBase>>mIgnoreCharas;

    //キャラName(キャラタイプ＋ID)、貫通時間
    std::unordered_map<std::string,float> mHitCountEnemies; // 貫通した敵のダメージ回数記録

    int mCurrent_gHandle = -1;

    bool mInitialize = false;

    bool mIsFinish = false;
    
    bool mIsHit = false;

    bool mIsPenetration = false;

    bool mIsHoming = false;;

    bool mIsReflection = false;

    std::shared_ptr<CharacterBase>mHomingTarget;

    int mPenetrationCurrentCount = 0;

    int mPenetrationMaxCount = 1;

    float mPenetrationCoolTime = 0.1f;

    int mReflectionMaxCount = 0;

    int mReflectionCurrentCount = 0;

    float mShock = 0.5f;

    float mMoveSpeed = 1000.0f;

    float mAttack = 5.0f;

    float mLifeTime = 0.0f;

    float mMaxLifeTime = 5.0f;

    Vector2D<float>mDrawOffset;
};

