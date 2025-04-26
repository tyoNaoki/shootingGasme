#include "Bullet.h"
#include "CollisionManager.h"
#include "ActorManager.h"
#include "CharacterBase.h"
#include "GraphicManager.h"
#include "PlayerCharacter.h"

Bullet::Bullet(Vector2D<float>drawOffset, Vector2D<float>extendSize):Actor()
{
    mStartAnimStateName = "Flight";
    mDrawOffset = drawOffset;
    mDrawExtendSize = extendSize;
}

Bullet::~Bullet()
{
    DeleteGraph(mCurrent_gHandle);
}

void Bullet::Init(CharacterType ct, CharacterType ownerType, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    mIgnoreCharas.clear();
    //初期化
    Initialize(ct, typeName, id, localPosition, worldPosition, rotation, isVisible);

    mOwnerType = ownerType;
    //発射したオーナーがプレイヤーなら
    if(ownerType == CharacterType::PLAYER){
        auto chara = std::dynamic_pointer_cast<CharacterBase>(ACTOR_M.GetCurrentPlayer());
        //衝突判定からプレイヤーを除外
        mIgnoreCharas.push_back(chara);
    }
    //アクションステート初期化
    ANIM_M.InitializeStates(this,mStartAnimStateName);

    //画像初期化
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,typeName,mStartAnimStateName);

    //各変数初期化
    mLifeTime = mMaxLifeTime;
    mIsHit = false;
    mIsFinish = false;
    mInitialize = true;
}

void Bullet::Reset(CharacterType ownerType, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    mIgnoreCharas.clear();
    //初期化
    Initialize(GetActorType(), GetTypeName(), id, localPosition, worldPosition, rotation, isVisible);

    mOwnerType = ownerType;
    //発射したオーナーがプレイヤーなら
    if (ownerType == CharacterType::PLAYER) {
        auto chara = std::dynamic_pointer_cast<CharacterBase>(ACTOR_M.GetCurrentPlayer());
        if(!chara){return;}
        //衝突判定からプレイヤーを除外
        mIgnoreCharas.push_back(chara);
    }
    //アクションステート初期化
    ANIM_M.InitializeStates(this, mStartAnimStateName);

    //画像初期化
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,GetTypeName(), mStartAnimStateName);

    //各変数初期化
    mLifeTime = mMaxLifeTime;
    mPenetrationCurrentCount = 0;
    mReflectionCurrentCount = 0;
    mHitCountEnemies.clear();
    mIsHit = false;
    mIsFinish = false;
    mInitialize = true;
}

bool Bullet::IsInitialize()
{
    return mInitialize;
}

void Bullet::Update(const float deltaTime)
{
    auto scene = SCENE_M.GetCurrentScene();
    if (!scene ||!mInitialize) { return; }

    //消失アニメーション更新
    if(mIsFinish){

        //DEBUG_HELPER.DrawCollision(&GetCollision());
        SetLocalPosition2D(GetLocalFromWorldPosi(GetWorldPosition2D()));
        ANIM_M.Update(this);
        //現在のアニメーション画像更新
        Singleton<GraphicManager>::get_instance().updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName());
        return;
    }

    //生存時間が０秒以下
    if (mLifeTime <= 0.0f) {
        StartDeadAnimation();
        return;
    }

    //ホーミング弾の場合、自分からターゲットまでの角度を自分の角度に設定
    if(mIsHoming){
        if(mHomingTarget||mHomingTarget->IsActive()){
            SetRotation((float)Vector2D<float>::GetLookAtAngle(GetWorldPosition2D(), mHomingTarget->GetWorldPosition2D()));
        }
    }

    //角度から進行ベクトルを計算
    double rad = Vector2D<float>::GetRadiansFromDegrees(GetRotation());
    auto unitVec = Vector2D<float>::GetUnitVectorByRadian(rad);
    //移動量を計算
    float speed = GetMoveSpeed();
    auto moveVel = unitVec * speed * deltaTime;
    //座標計算
    auto newPosi = moveVel + GetWorldPosition2D();

    auto collision = GetCollision<Collision::Circle>();

    //反射弾専用処理
    if (mIsReflection) {
        int width, height;
        GetWindowSize(&width, &height);

        auto localPosi = GetLocalFromWorldPosi(newPosi);
        bool refrection = false;
        
        //プレイヤーの画面外の場合、移動ベクトルを反転させる
        if (localPosi.x < collision->mRadius|| localPosi.x > width - collision->mRadius){
            moveVel.x *= -1;
            refrection = true;
        }
        if (localPosi.y < collision->mRadius|| localPosi.y > height - collision->mRadius){
            moveVel.y *= -1;
            refrection = true;
        }
        //反射フラグがったている場合
        if(refrection){
            //反射回数が最大に達していた場合、消去処理
            if(mReflectionMaxCount<mReflectionCurrentCount + 1){
                StartDeadAnimation();
                return;
            }
            //現在の反射回数を記録
            mReflectionCurrentCount++;
            //角度、座標再設定
            SetRotation(Vector2D<float>::GetLookAtAngle(moveVel));
            newPosi = moveVel + GetWorldPosition2D();
        }
    }

    SetVelocity(moveVel);

    //マップ外にいた場合、消失
    if (!COLLISION_M.IsBulletInMap(newPosi,scene->GetMap())) {
        auto localPosi = GetLocalFromWorldPosi(newPosi);
        std::string message = "Bullet is out";
        DEBUG_HELPER.Add(message);
        SetActive(false);
        return;
    }

    mIsHit = false;
    //プレイヤーが主
    if(mOwnerType == CharacterType::PLAYER){
        //周囲の敵検索
        auto enemies = COLLISION_M.GetNearCharacters(newPosi, collision->mRadius, mIgnoreCharas);

        if(enemies.size()!= 0 || ACTOR_M.IsSpawnBoss()){

            for (auto& x : enemies){

                //衝突しない
                if (!collision->IsHit(*x->GetCollision())) {
                    continue;
                }

                HandleBulletHit(x);
            }
            //ボス用処理
            if (ACTOR_M.IsSpawnBoss()) {
                auto boss = ACTOR_M.GetCurrentBossEnemy();
                if(collision->IsHit(*boss->GetCollision())){
                    if (IsFinish(boss)) {
                        boss->TakeDamage(mAttack);
                        mIsHit = true;
                        StartDeadAnimation();
                        return;
                    }

                    if (IsPenetratingHit(boss)) {
                        mIsHit = true;

                        boss->TakeDamage(mAttack);
                        mHitCountEnemies[boss->GetName()] = mPenetrationCoolTime;
                        mPenetrationCurrentCount++;
                    }else{
                        mHitCountEnemies[boss->GetName()] -= deltaTime;
                    }
                }
            }
        }
    }else{
        //他キャラの処理
        auto player = ACTOR_M.GetCurrentPlayer();
        if(player&&player->IsActive()){
            if (collision->IsHit(*player->GetCollision())) {
                mIsHit = true;
                player->TakeDamage(mAttack);
                StartDeadAnimation();
                return;
            }
        }
    }

    Vector2D<float>localPosi = scene->GetMap()->GetLocalFromWorldPosition(newPosi);

    //座標更新
    SetLocalPosition2D(localPosi);
    SetWorldPosition2D(newPosi);

    //DEBUG_HELPER.DrawCollision(mCollision);

    ANIM_M.Update(this);

    //現在のアニメーション画像更新
    Singleton<GraphicManager>::get_instance().updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(),GetCurrentAnimState()->GetActionName());

    mLifeTime -=deltaTime;
}

void Bullet::Draw(const float deltaTime)
{
    
    //ダミー処理
    if (mVisible && mCurrent_gHandle == -2) {
        auto collision = GetCollision<Collision::Circle>();
        auto localPosi = GetLocalPosition2D();
        int x = static_cast<int>(localPosi.x);
        int y = static_cast<int>(localPosi.y);
        int r = static_cast<int>(collision->mRadius);

        if(mOwnerType==CharacterType::PLAYER){
            auto color = GetColor(255,255,0);
            DrawCircle(x, y, r, color, TRUE);
        }else{
            DrawCircle(x, y, r, GetColor(150, 0, 0), TRUE);
        }
        
        return;
    }

    //描画関係
    auto localPosi = GetLocalPosition2D();
    float x = localPosi.x + mDrawOffset.x;
    float y = localPosi.y + mDrawOffset.y;
    float width = mDrawSize.x * mDrawExtendSize.x;
    float height = mDrawSize.y * mDrawExtendSize.y;
    Vector2D<float>leftTopDrawPosi = Vector2D<float>(x, y);
    Vector2D<float>rightDownDrawPosi = Vector2D<float>(x + width, y + height);

    if (mVisible && mCurrent_gHandle != -1) { 
        float angle = Vector2D<float>::GetRadiansFromDegrees(GetRotation());
        DrawRotaGraph(x,y,mDrawExtendSize.x,angle,mCurrent_gHandle,TRUE,FALSE);
    }
}

void Bullet::StartDeadAnimation()
{
    mIsFinish = true;
    SetVelocity(Vector2D<float>(0, 0));
    //Deadアニメーションが必要かどうか
    if (!HasAnimationState("Dead")) {//必要なければ、そのまま消す
        mVisible = false;
        SetActive(false);
        return;
    }

    //Deadアニメーションへ遷移
    ANIM_M.TransitionToState(this, "Dead");
    GRAPHIC_M.TransitionToAction(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, GetTypeName(), GetCurrentAnimState()->GetActionName());
}

void Bullet::FinishDeadAnimation()
{
    //描画フラグおろす
    mVisible = false;
    //非アクティブに
    SetActive(false);
}

void Bullet::HandleBulletHit(std::shared_ptr<CharacterBase>target)
{
    //貫通弾ではない場合、その場で消失。
                //貫通弾の場合、雑魚一回のみダメージ、ボス多段ヒット
    if (IsFinish(target)) {
        target->TakeDamage(mAttack);

        if(target->GetActorType()==CharacterType::ENEMY)//雑魚敵にはノックバックをつける
        {
            float targetRadian = (float)Vector2D<float>::GetLookAtRadian(GetWorldPosition2D(), target->GetWorldPosition2D());
            target->AddKnockBack(Vector2D<float>(cos(targetRadian), sin(targetRadian)), mShock, 0.2f);
        }

        mIsHit = true;
        StartDeadAnimation();
        return;
    }

    //貫通ダメージが入らない
    if (!IsPenetratingHit(target)) {
        return;
    }

    //弾の現在座標からヒットした敵の座標までの角度で敵をノックバックさせる
    target->TakeDamage(mAttack);
    if (target->GetActorType() == CharacterType::ENEMY)//雑魚敵にはノックバックをつける
    {
        float targetRadian = (float)Vector2D<float>::GetLookAtRadian(GetWorldPosition2D(), target->GetWorldPosition2D());
        target->AddKnockBack(Vector2D<float>(cos(targetRadian), sin(targetRadian)), mShock, 0.2f);
    }

    mIsHit = true;

    //貫通のクールタイム設定
    mHitCountEnemies[target->GetName()] = mPenetrationCoolTime;
    //現在の貫通回数
    mPenetrationCurrentCount++;
}

void Bullet::SetStatus(float speed,float attack,bool IsPenetration,int penetrationMaxCount)
{
    //ステータス設定
    mMoveSpeed = speed;
    mAttack = attack;
    mIsReflection = false;
    mIsPenetration = IsPenetration;
    if(mIsPenetration){
        mPenetrationMaxCount = penetrationMaxCount;
    }else{
        mPenetrationMaxCount = 0;
    }
}

void Bullet::SetStatus(float speed, float attack,float homingTime,std::shared_ptr<CharacterBase> homingTarget)
{
    //ホーミング専用のステータス設定
    mMoveSpeed = speed;
    mAttack = attack;
    mIsPenetration = false;
    mIsReflection = false;
    mIsHoming = true;
    mHomingTarget = homingTarget;
    mLifeTime = homingTime;
}

void Bullet::SetStatus(float speed, float attack, float lifeTime,int reflectionMaxCount)
{
    //反射弾専用ステータス設定
    mMoveSpeed = speed;
    mAttack = attack;
    mIsPenetration = true;
    mPenetrationMaxCount = 999;
    mIsReflection = true;
    mReflectionMaxCount = reflectionMaxCount;
    mLifeTime = lifeTime;
}

void Bullet::SetWorldPosition2D(Vector2D<float> worldPosition)
{
    auto collision = GetCollision<Collision::Circle>();
    Actor::SetWorldPosition2D(worldPosition);
    collision->mCenter = worldPosition;
}

void Bullet::ApplyDrawOffset(Vector2D<float> offset)
{
    mDrawOffset = offset;
}

float Bullet::GetMoveSpeed() const
{
    return mMoveSpeed;
}

bool Bullet::IsHit()
{
    return mIsHit;
}

bool Bullet::IsHomingBullet()
{
    return mIsHoming;
}

bool Bullet::IsPenetrationBullet()
{
    return mIsPenetration;
}

bool Bullet::IsFinish(std::shared_ptr<CharacterBase> target)
{
    //貫通弾ではない、貫通回数が最大数に達している
    return (!mIsPenetration || !CanPenetratingOnCurrentCount() && IsPenetratingHit(target));
}


bool Bullet::IsPenetratingHit(std::shared_ptr<CharacterBase>target)
{
    //今までヒットした敵を取得
    auto it = mHitCountEnemies.find(target->GetName());
    if(target->GetActorType()==CharacterType::ENEMY || target->GetActorType() == CharacterType::BOSS){
        //初めてヒットした敵
        if (it == mHitCountEnemies.end()) {
            return true;
        }
        
        //無敵時間経過したかどうか
        return it->second <= 0.0f;
    }

    return false;
}

bool Bullet::CanPenetratingOnCurrentCount()
{
   return mPenetrationCurrentCount < mPenetrationMaxCount;
}

void Bullet::ProcessDamage(std::shared_ptr<Actor> target, float attack, float shock, float penetrationCoolTime)
{
}
