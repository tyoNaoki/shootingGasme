#include "CharacterBase.h"
#include "GraphicManager.h"
#include "Component.h"

CharacterBase::CharacterBase():Actor()
{
    mDrawOffset = Vector2D<int>(0,0);
    //Singleton<GraphicManager>::get_instance().CreateGraphic(name);
}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Update(const float deltaTime)
{
    SetVelocity(Vector2D<float>(0,0));
    mIsMove = false;
    //if (cur_vel.y > ) 
    GetComponentManager().UpdateComponents(deltaTime);

    //被弾方向にキャラを移動させる
    if (IsKnockBack()) {
        KnockBack(deltaTime);
    }
}

void CharacterBase::Draw(const float deltaTime)
{
    //描画
    //DrawFormatString(0, 0, GetColor(255, 0, 0), "CharacterBase::Update velocity is (%f,%f)",GetVelocity().x,GetVelocity().y);
    //DrawFormatString(0, 50, GetColor(255, 0, 0), "CharacterBase::Update drawRotation is %f",GetDrawRotation());

    if(mDamage){
        if(mCurrent_DamageAnimFrame<mDamage_MaxFrame){
            //ダメージ処理
            mCurrent_DamageAnimFrame+=deltaTime;
        }else{
            mDamage = false;
            mCurrent_DamageAnimFrame = 0.0f;
        }
    }

    auto scene = Singleton<SceneManager>::get_instance().GetCurrentScene();
    if(!scene){return;}

    //ダミー処理
    if(mVisible && mCurrent_gHandle == -2){
        auto collision = std::dynamic_pointer_cast<Collision::Rect>(GetCollision());
        Vector2D<float> size = collision->mSize / 2;
        auto map = scene->GetMap();
        
        auto localPosi = map->GetLocalFromWorldPosition(GetWorldPosition2D());
        int x1 = static_cast<int>(localPosi.x - size.x);
        int y1 = static_cast<int>(localPosi.y - size.y);
        int x2 = static_cast<int>(localPosi.x + size.x);
        int y2 = static_cast<int>(localPosi.y + size.y);

        //四角の何かを描画
        if(mDamage){
            DrawBox(x1, y1, x2, y2, GetColor(255, 255, 255), TRUE);
        }else{
            DrawBox(x1, y1, x2, y2, GetColor(200, 0, 0), TRUE);
        }
        //DEBUG_HELPER.DrawCollision(GetCollision());
        //DrawBox(x1,y1,x2,y2,GetColor(200,0,0),TRUE);
        return;
    }

    int x = static_cast<int>(GetLocalPosition2D().x + mDrawOffset.x);
    int y = static_cast<int>(GetLocalPosition2D().y + mDrawOffset.y);
    float width = mDrawSize.x * mDrawExtendSize.x;
    float height = mDrawSize.y * mDrawExtendSize.y;
    Vector2D<int>leftTopDrawPosi;
    Vector2D<int>rightDownDrawPosi;
    //反転
    if(IsReverse()){
        leftTopDrawPosi = Vector2D<int>(x + width,y);
        rightDownDrawPosi = Vector2D<int>(x, y + height);
    }else{ //通常
        leftTopDrawPosi = Vector2D<int>(x,y);
        rightDownDrawPosi = Vector2D<int>(x+width,y+height);
    }

    if(mVisible && mCurrent_gHandle != -1){
        if(mDamage){
            SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
            DrawExtendGraph(leftTopDrawPosi.x,leftTopDrawPosi.y,rightDownDrawPosi.x,rightDownDrawPosi.y,mCurrent_gHandle,TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            
        }else{
            DrawExtendGraph(leftTopDrawPosi.x, leftTopDrawPosi.y, rightDownDrawPosi.x, rightDownDrawPosi.y, mCurrent_gHandle, TRUE);
        }
    }
}

/*
void CharacterBase::SetPosition2D(Vector2D<float> localP)
{
    auto newP = GetWorldFromLocalPosi(localP);
    SetWorldPosition2D(newP);
}
*/

void CharacterBase::SetWorldPosition2D(Vector2D<float> worldP)
{
    auto collision = GetCollision<Collision::Rect>();
    Actor::SetWorldPosition2D(worldP);
    collision->mLeftTop = worldP - (collision->mSize / 2);
}

void CharacterBase::SetWorldPosition2D(Vector2D<float> worldP, bool isMove)
{
    Actor::SetWorldPosition2D(worldP);
    auto collision = GetCollision<Collision::Rect>();
    collision->mLeftTop = worldP - (collision->mSize / 2);
    mIsMove = isMove;
}

float CharacterBase::GetMoveSpeed() const
{
    return 1.0f;
}

void CharacterBase::TakeDamage(float damage)
{
    if(IsDead()){return;}
    //HPを減らす
    mHP-=damage;

    //被弾音を鳴らす
    if(GetActorType()==CharacterType::ENEMY|| GetActorType() == CharacterType::BOSS){
        SOUND_M.PlaySoundByID(mHitSound);
    }

    //HP０の場合、死亡アニメーションへ移行
    if(mHP<=0){
        mHP = 0.0f;
        StartDeadAnimation();
        return;
    }

    //被ダメージアニメーション
    DamageAnimation();
}

void CharacterBase::StartDeadAnimation()
{
    mKnockBack = false;
    mDamage = false;
    mIsDead = true;
    SetVelocity(Vector2D<float>(0, 0));
    //Deadアニメーションが必要かどうか
    if(!HasAnimationState("Dead")){//必要なければ、そのまま消す
        mVisible = false;
        SetActive(false);
        return;
    }

    //Deadアニメーションへ遷移
    ANIM_M.TransitionToState(this,"Dead");
    GRAPHIC_M.TransitionToAction(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, GetTypeName(), GetCurrentAnimState()->GetActionName());
}

void CharacterBase::FinishDeadAnimation()
{
    mVisible = false;
    SetActive(false);
}

void CharacterBase::DamageAnimation()
{
    mDamage = true;
    mCurrent_DamageAnimFrame = 0.0f;
}

void CharacterBase::InitKnockBack()
{
    //ノックバック初期化
    mKnockBackTime = 0.0f;
    mKnockBackDirection = Vector2D<float>(0, 0);
    mKnockBackStrength = 0.0f;
    mKnockBack = false;
    mCurrentKnockBackTime = 0.0f;
}

void CharacterBase::AddKnockBack(Vector2D<float> direction, float strength, float knockBackTime)
{
    //ノックバック付与
    mKnockBack = true;
    mKnockBackStrength += strength;
    mKnockBackDirection += direction;
    mKnockBackTime += knockBackTime;
}

void CharacterBase::KnockBack(float deltaTime)
{
    mCurrentKnockBackTime += deltaTime;
    //ノックバック時間が終わっているなら、ノックバックを初期化
    if(mCurrentKnockBackTime >= mKnockBackTime){
        //DEBUG_HELPER.Add("knockBack is " + GetWorldPosition2D().ToString(), 0.5f);
        InitKnockBack();
        return;
    }

    //ノックバックの大きさ、ノックバック時の座標計算
    float knockbackRatio = (float)pow(1.0f - (mCurrentKnockBackTime / mKnockBackTime), 2);
    Vector2D<float> newPosi = mKnockBackDirection * mKnockBackStrength * knockbackRatio * deltaTime;
    newPosi += GetWorldPosition2D();

    //座標更新
    SetLocalPosition2D(GetLocalFromWorldPosi(newPosi));
    SetWorldPosition2D(newPosi);
}

int CharacterBase::GetLevel()
{
    return mLevel;
}

float CharacterBase::GetHP()
{
    return mHP;
}

float CharacterBase::GetMaxHP()
{
    return mMaxHP;
}

float CharacterBase::GetScore()
{
    return mScore;
}

bool CharacterBase::IsKnockBack()
{
    return mKnockBack;
}

bool CharacterBase::IsDead()
{
    return mIsDead;
}
