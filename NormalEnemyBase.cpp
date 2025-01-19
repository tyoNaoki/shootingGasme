#include "NormalEnemyBase.h"
#include "DxLib.h"
#include "InputManager.h"
#include "GraphicManager.h"
#include "DebugHelper.h"
#include "Scene.h"
#include "SceneManager.h"
#include "EnemyChaseMoveComponent.h"
#include "NormalEnemyAttackComponent.h"
#include "PlayerCharacter.h"
#include "StatusManager.h"
#include "ActorManager.h"
#include "CollisionManager.h"

NormalEnemyBase::NormalEnemyBase(Collision::Rect rect) : CharacterBase(rect)
{
}

NormalEnemyBase::~NormalEnemyBase()
{
    mCharaToIgnores.clear();
}

void NormalEnemyBase::Init(CharacterType ct, std::string name, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    if (id == -1) {
        return;
    }
    //初期化
    Initialize(ct, name, id, localPosition, worldPosition, rotation, isVisible);

    //ステータス設定
    auto status = STATUS.GetCurrentEnemyStatus(name);
    mLevel = status.level;
    mHP = status.hp;
    mMaxHP = status.hp;
    mMoveSpeed = status.moveSpeed;
    mScore = status.score;

    //衝突判定除外用
    std::shared_ptr<NormalEnemyBase> self = std::static_pointer_cast<NormalEnemyBase>(shared_from_this());
    mCharaToIgnores = { self,ACTOR_M.GetCurrentPlayer()};

    //コンポーネント追加
    AddComponent(std::make_shared<EnemyChaseMoveComponent>(self), 0);
    AddComponent(std::make_shared<NormalEnemyAttackComponent>(self), 1);

    //コリジョン座標更新
    SetWorldPosition2D(worldPosition);
    //ノックバック初期化
    InitKnockBack();
    //アニメーション初期化
    ANIM_M.InitializeStates(this, mStartAnimStateName);
    //アニメーション初期化
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,name,mStartAnimStateName);

    mInitialize = true;
}

void NormalEnemyBase::Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    if (id == -1) {
        mInitialize = false;
        return;
    }
    //初期化
    Initialize(GetActorType(), GetTypeName(), id, localPosition, worldPosition, rotation, isVisible);

    //ステータス再設定
    auto status = STATUS.GetCurrentEnemyStatus(GetTypeName());
    mLevel = status.level;
    mHP = status.hp;
    mMaxHP = status.hp;
    mMoveSpeed = status.moveSpeed;
    mScore = status.score;

    //コンポーネント初期化
    InitComponents();

    //コリジョン座標更新
    SetWorldPosition2D(worldPosition);
    //ノックバック初期化
    InitKnockBack();
    //アニメーション初期化
    ANIM_M.InitializeStates(this, mStartAnimStateName);
    //アニメーション初期化
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,GetTypeName(), mStartAnimStateName);

    mIsDead = false;
    mInitialize = true;
}

void NormalEnemyBase::Update(const float deltaTime)
{
    auto scene = Singleton<SceneManager>::get_instance().GetCurrentScene();
    if (!scene) { return; }

    //死んでいる場合
    if (mIsDead) {
        DEBUG_HELPER.DrawCollision(GetCollision());
        SetLocalPosition2D(GetLocalFromWorldPosi(GetWorldPosition2D()));
        //アニメーション更新
        ANIM_M.Update(this);
        //現在のアニメーション画像更新
        Singleton<GraphicManager>::get_instance().updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName());
        return;
    }

    //コンポーネント更新
    CharacterBase::Update(deltaTime);

    //速度取得
    Vector2D<float> cur_vel = GetVelocity();

    //動いているかどうか
    bool isMove = IsMoving();

    //現在の描画角度取得
    float currentRotation = GetDrawRotation();

    //左向きかどうか
    bool isReverse = false;

    //アニメーション画像タイプ更新(通常歩行、反転歩行)
    if (isMove) {
        if (nearEqual(currentRotation, 90.0f) || nearEqual(currentRotation, 270.0f)) {
            //角度が90度か270度に近い数値なら、向きはそのままの向きで固定
            isReverse = mReverse;
        }
        //左向きにさせる
        else if (currentRotation > 90.0f && currentRotation < 270.0f) {
            isReverse = true;
            //角度が90度か270度に近い数値なら向きはそのままで返す
        }
        else {
            //右向き
            isReverse = false;
        }
        //止まっている時
    }
    else if (!isMove) {
        isReverse = mReverse;
    }

    mReverse = isReverse;

    //衝突検知//
    auto newPosi = GetWorldPosition2D();
    auto newCollisionPosi = GetCollision();

    //マップとの当たり判定管理
    auto map = scene->GetMap();

    //マップ外にいた場合、中に戻す
    if (COLLISION_M.PushBackFromOut(newPosi, newCollisionPosi, map)) {
        std::string message = "Enemy is out";
        DEBUG_HELPER.Add(message);
    }

    //コリジョンの一番長い状態を距離に設定
    float dis = newCollisionPosi.mSize.x >= newCollisionPosi.mSize.y ? newCollisionPosi.mSize.x : newCollisionPosi.mSize.y;
    auto nearActors = COLLISION_M.DetectionNearCharacters(newPosi, newCollisionPosi, dis, mCharaToIgnores);
    if (nearActors.size() > 0) {
        for (auto& x : nearActors) {
            //衝突時、更新前の座標への方向に衝突分、新しい座標を戻して返す
            if (Collision::IsColliding(newCollisionPosi, x->GetCollision())) {
                auto rect2 = x->GetCollision();
                Collision::PushBackRect(newCollisionPosi, rect2);
                newPosi = newCollisionPosi.mLeftTop + (newCollisionPosi.mSize / 2);
            }
        }
    }

    auto localPosi = scene->GetMap()->GetLocalFromWorldPosition(newPosi);

    //座標更新
    SetLocalPosition2D(localPosi);
    SetWorldPosition2D(newPosi);

    std::string preActionName = GetCurrentAnimState()->GetActionName();

    DEBUG_HELPER.DrawCollision(GetCollision());

    //アニメーション遷移更新
    ANIM_M.Update(this);

    //現在のアニメーション画像更新
    //新アニメーション遷移
    if (preActionName != GetCurrentAnimState()->GetActionName()) {
        GRAPHIC_M.TransitionToAction(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,GetTypeName(), GetCurrentAnimState()->GetActionName());
    }
    else {
        GRAPHIC_M.updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName());
    }
}

bool NormalEnemyBase::IsInitialize()
{
    return mInitialize;
}

float NormalEnemyBase::GetMoveSpeed() const
{
    return mMoveSpeed;
}
