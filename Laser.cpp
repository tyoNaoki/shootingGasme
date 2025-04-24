#include "Laser.h"
#include "GraphicManager.h"
#include "SceneManager.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"

Laser::Laser()
{
    mStartAnimStateName = "Laser";
}

Laser::~Laser()
{
    DeleteGraph(mCurrent_gHandle);
    DeleteGraph(mCurrent_startHandle);
}

void Laser::Init(CharacterType ct, std::shared_ptr<CharacterBase> owner, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    //初期化
    Initialize(ct, typeName, id, localPosition, worldPosition, rotation, isVisible);

    mOwner = owner;
    //アニメーションステート初期化
    ANIM_M.InitializeStates(this, mStartAnimStateName);
    //レーザーの照準付近の画像取得
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,typeName,mAnimStartName);
    //レーザーの画像取得
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_laserHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, typeName, mAnimLaserName);

    mCurrentTime = 0.0f;
    mIsHit = false;
    mIsInitialize = true;
}

void Laser::Reset(std::shared_ptr<CharacterBase> owner, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    //初期化
    Initialize(GetActorType(), GetTypeName(), id, localPosition, worldPosition, rotation, isVisible);
    mOwner = owner;
    //アニメーションステート初期化
    ANIM_M.InitializeStates(this, mStartAnimStateName);
    //レーザーの照準付近の画像取得
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, GetTypeName(), mAnimStartName);
    //レーザーの画像取得
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_laserHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, GetTypeName(), mAnimLaserName);

    mLaserLength = 100.0f;
    mCurrentTime = 0.0f;
    mIsHit = false;
    mIsInitialize = true;
}

bool Laser::IsInitialize()
{
    return mIsInitialize;
}

void Laser::Update(const float deltaTime)
{
    auto scene = SCENE_M.GetCurrentScene();
    if (!scene|| !mIsInitialize) { return; }
    auto owner = mOwner.lock();

    if(owner->IsDead()||!owner->IsActive()) {
        SetActive(false);
        return;
    }

    //レーザーの長さ計算
    mLaserLength += mSpeed*deltaTime;
    //レーザーの当たり判定の範囲を計算
    mRotRect = Collision::CalculateAttackRange(GetWorldPosition2D(),mLaserWidth, mLaserLength,GetRotation());
    mIsHit = false;
    auto mOwnerType = owner->GetActorType();

    //ボス、敵専用処理
    if(mOwnerType==CharacterType::BOSS||mOwnerType==CharacterType::ENEMY){
        auto player = ACTOR_M.GetCurrentPlayer();
        if(!player){return;}
        //Hit
<<<<<<< HEAD
        auto collision = player->GetCollision<Collision::Rect>();
        if(mCurrentTime <= 0.0f&&Collision::IsColliding(mRotRect,*collision)){
=======
        if(mCurrentTime <= 0.0f&&Collision::IsColliding(mRotRect,player->GetCollision())){
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
            //レーザーのノックバック角度をレーザーの照射位置からプレイヤーまでの角度とレーザーの照射角度との差分で計算、設定する
            float laserRot = GetRotation();
            float targetRot = (float)Vector2D<float>::GetLookAtAngle(GetWorldPosition2D(),player->GetWorldPosition2D());
            float diff = Vector2D<float>::AngleDiff(laserRot,targetRot);
            float targetRadian = 0;
            if(diff > 0){
                targetRadian = Vector2D<float>::GetRadiansFromDegrees(laserRot - 20.0f);
            }else{
                targetRadian = Vector2D<float>::GetRadiansFromDegrees(laserRot + 20.0f);
            }
            //ダメージ処理
            ACTOR_M.GetCurrentPlayer()->TakeDamage(mAttack, Vector2D<float>(cos(targetRadian), sin(targetRadian)), mKnockBackStrength,mknockBackTime);

            mCurrentTime = mInvincibilityTime;
            mIsHit = true;
        }else if(mCurrentTime>0.0f){
            mCurrentTime-= deltaTime;
        }
    }
    //アニメーション更新
    ANIM_M.Update(this);
    //画像更新
    GRAPHIC_M.updateHandle(deltaTime,mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(),mAnimStartName);
}

void Laser::Draw(const float deltaTime)
{
    //DEBUG_HELPER.DrawCollision(mRotRect, 0.2f, mIsHit);

    if (mVisible && mCurrent_gHandle == -2) {
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
     
        DEBUG_HELPER.DrawCollision(mRotRect, 0.2f, mIsHit);
        float angle = Vector2D<float>::GetRadiansFromDegrees(GetRotation());
        auto vertices = mRotRect.GetVertices();
        auto offset = vertices[0] - vertices[3];
        auto start = (vertices[0] - vertices[3]) / 2 + vertices[0] + offset * -0.18;
        auto drawPosi = GetLocalFromWorldPosi(start);
        //レーザースタート位置描画
        DrawRotaGraph(drawPosi.x, drawPosi.y, 2.5f, angle, mCurrent_gHandle, TRUE);

        //レーザーの長さの数、画像を描画する
        for (int i = 0; i < mLaserLength - 97; i++) {
            auto drawLaserPosi = start +  ((vertices[1] - vertices[0]) / mLaserLength) * 60;
            auto drawLength = ((vertices[1] - vertices[0])/mLaserLength) * i;
            drawPosi = GetLocalFromWorldPosi(drawLaserPosi + drawLength);
            DrawRotaGraph(drawPosi.x, drawPosi.y, 2.5f, angle, mCurrent_laserHandle, TRUE);
        }
    }
}

bool Laser::IsHit()
{
    return mIsHit;
}

void Laser::SetStatus(float laserWidth,float speed,float attack, float knockBackStrength, float knockBackTime)
{
    mLaserWidth = laserWidth;
    mSpeed = speed;
    mAttack = attack;
    mKnockBackStrength = knockBackStrength;
    mknockBackTime = knockBackTime;
}
