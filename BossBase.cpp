#include "BossBase.h"
#include "GraphicManager.h"
#include "ActorManager.h"
#include "DebugHelper.h"
#include "SceneManager.h"
#include "CollisionManager.h"

BossBase::BossBase() : CharacterBase()
{
    mStartAnimStateName = "BossIdle";
}

BossBase::~BossBase()
{
    mPlayer = nullptr;
}

void BossBase::Init(CharacterType ct, std::string name, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    if (id == -1) {
        return;
    }
    Initialize(ct, name, id, localPosition, worldPosition, rotation, isVisible);

    mPlayer = ACTOR_M.GetCurrentPlayer();

    //ボス戦用ステータス設定
    auto status = STATUS.GetCurrentBossStatus();
    mLevel = 1;
    mHP = status.hp;
    mMaxHP = status.hp;
    mMoveSpeed = status.moveSpeed;
    mScore = status.score;

    //コリジョン座標更新
    SetWorldPosition2D(worldPosition);

    ANIM_M.InitializeStates(this,mStartAnimStateName);
    //アニメーション初期化
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,name,mStartAnimStateName);

    mInitialize = true;
}

void BossBase::Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    if (id == -1) {
        mInitialize = false;
        return;
    }
    Initialize(GetActorType(), GetTypeName(), id, localPosition, worldPosition, rotation, isVisible);

    //ステータス再設定
    auto status = STATUS.GetCurrentEnemyStatus(GetTypeName());
    mLevel = status.level;
    mHP = status.hp;
    mMaxHP = status.hp;
    mMoveSpeed = status.moveSpeed;
    mScore = status.score;

    //コンポーネント初期化
    GetComponentManager().InitComponents();

    //コリジョン座標更新
    SetWorldPosition2D(worldPosition);

    ANIM_M.InitializeStates(this,mStartAnimStateName);
    //アニメーション初期化
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,GetTypeName(), mStartAnimStateName);

    mIsDead = false;
    mInitialize = true;
}

void BossBase::Update(const float deltaTime)
{
    if (!IsActive()) return;

    auto scene = Singleton<SceneManager>::get_instance().GetCurrentScene();
    if (!scene) { return; }
    
    if(!mTakeKnockBack){
        mKnockBack = false;
    }

    //コンポーネント更新
    CharacterBase::Update(deltaTime);

    //速度取得
    Vector2D<float> cur_vel = GetVelocity();

    //動いているかどうか
    bool isMove = IsMoving();

    //現在の描画角度取得
    float currentRotation = GetDrawRotation();

    if (currentRotation >= 0 && currentRotation < 180) {
        mReverse = false; // キャラクターは右を向いている
    }else {
        mReverse = true; // キャラクターは左を向いている
    }

    auto newPosi = GetWorldPosition2D();
    auto localPosi = scene->GetMap()->GetLocalFromWorldPosition(newPosi);

    //座標更新
    SetLocalPosition2D(localPosi);
    SetWorldPosition2D(newPosi);

    //DEBUG_HELPER.DrawCollision(GetCollision());

    std::string preAction = GetCurrentAnimState()->GetActionName();
    ANIM_M.Update(this);

    if(preAction!= GetCurrentAnimState()->GetActionName()){
        GRAPHIC_M.TransitionToAction(mCurrent_gHandle,mCurrent_AnimFrame,mCurrent_AnimFrame_adjust,mDrawSize,GetTypeName(),GetCurrentAnimState()->GetActionName());
    }else{
        //現在のアニメーション画像更新
        GRAPHIC_M.updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName());
    }
    
}

void BossBase::Draw(const float deltaTime)
{
    //現在ダメージを受けているか
    if (mDamage) {
        if (mCurrent_DamageAnimFrame < mDamage_MaxFrame) {
            //ダメージ処理
            mCurrent_DamageAnimFrame += deltaTime;
        }
        else{
            mDamage = false;
            mCurrent_DamageAnimFrame = 0.0f;
        }
    }

    auto scene = Singleton<SceneManager>::get_instance().GetCurrentScene();
    if (!scene) { return; }

    //ダミー画像の場合
    if (mVisible && mCurrent_gHandle == -2) {
        auto collision = GetCollision<Collision::Rect>();
        Vector2D<float> size = collision->mSize / 2;
        auto map = scene->GetMap();

        auto localPosi = map->GetLocalFromWorldPosition(GetWorldPosition2D());
        int x1 = static_cast<int>(localPosi.x - size.x);
        int y1 = static_cast<int>(localPosi.y - size.y);
        int x2 = static_cast<int>(localPosi.x + size.x);
        int y2 = static_cast<int>(localPosi.y + size.y);

        //四角の何かを描画
        if (mDamage) {
            DrawBox(x1, y1, x2, y2, GetColor(255, 255, 255), TRUE);
        }else if(mAlert){
            DrawBox(x1, y1, x2, y2, GetColor(0, 255, 0), TRUE);
        }else {
            DrawBox(x1, y1, x2, y2, GetColor(200, 0, 0), TRUE);
        }
        //DEBUG_HELPER.DrawCollision(GetCollision());
        //DrawBox(x1,y1,x2,y2,GetColor(200,0,0),TRUE);
        return;
    }

    int x = static_cast<int>(GetLocalPosition2D().x + mDrawOffset.x);
    int y = static_cast<int>(GetLocalPosition2D().y + mDrawOffset.y);
    int width = mDrawSize.x * mDrawExtendSize.x;
    int height = mDrawSize.y * mDrawExtendSize.y;
    Vector2D<int>leftTopDrawPosi;
    Vector2D<int>rightDownDrawPosi;
    //反転
    if (IsReverse()) {
        leftTopDrawPosi = Vector2D<int>(x + width, y);
        rightDownDrawPosi = Vector2D<int>(x, y + height);
    }
    else { //通常
        leftTopDrawPosi = Vector2D<int>(x, y);
        rightDownDrawPosi = Vector2D<int>(x + width, y + height);
    }

    //各状態で画像の描画に変化を加える
    if (mVisible && mCurrent_gHandle != -1) {
        if (mDamage) {//被ダメージ
            SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
            DrawExtendGraph(leftTopDrawPosi.x, leftTopDrawPosi.y, rightDownDrawPosi.x, rightDownDrawPosi.y, mCurrent_gHandle, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }else if(mAlert){//レーザーの発射前の警告
            SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
            DrawExtendGraph(leftTopDrawPosi.x, leftTopDrawPosi.y, rightDownDrawPosi.x, rightDownDrawPosi.y, mCurrent_gHandle, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }else {//通常
            DrawExtendGraph(leftTopDrawPosi.x, leftTopDrawPosi.y, rightDownDrawPosi.x, rightDownDrawPosi.y, mCurrent_gHandle, TRUE);
        }
    }
}

bool BossBase::IsInitialize()
{
    return mInitialize;
}

float BossBase::GetMoveSpeed() const
{
    return mMoveSpeed;
}

void BossBase::ChangeState(BossState newState)
{
    mState = newState;
}

BossState BossBase::GetCurrentState()
{
    return mState;
}

std::string BossBase::GetCurrentWeaponName() const
{
    return mWeaponName;
}

bool BossBase::IsState(BossState targetState)
{
    return mState == targetState;
}

void BossBase::SetAlert(bool isAlert)
{
    mAlert = isAlert;
}
