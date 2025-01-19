#include "CharacterBase.h"
#include "GraphicManager.h"
#include "Component.h"

CharacterBase::CharacterBase(Collision::Rect collision):mCollision(collision),Actor()
{
    mDrawOffset = Vector2D<int>(0,0);
    //Singleton<GraphicManager>::get_instance().CreateGraphic(name);
}

CharacterBase::~CharacterBase()
{
    for(auto &x:mComponents){
         x = nullptr;
    }
    mComponents.clear();
}

std::shared_ptr<Component> CharacterBase::GetComponent(int idx)
{
    if(mComponents.size()!=0||mComponents.size()>idx){
        return mComponents[idx];
    }

    return nullptr;
}

std::shared_ptr<Component> CharacterBase::GetComponent(std::string name)
{
    for(auto&x:mComponents){
        if(x->GetName()==name){
            return x;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Component>> CharacterBase::GetComponents()
{
    return mComponents;
}

void CharacterBase::AddComponent(std::shared_ptr<Component> component,int idx)
{
    mComponents.insert(mComponents.begin()+idx,component);
}

void CharacterBase::RemoveComponent(int idx)
{
    if (idx >= 0 && idx < mComponents.size()) {
        mComponents.erase(mComponents.begin() + idx);
    }
}


void CharacterBase::InitComponents()
{
    for(auto&x:mComponents){
        x->Init();
    }
}

void CharacterBase::Update(const float deltaTime)
{
    SetVelocity(Vector2D<float>(0,0));
    mIsMove = false;
    //if (cur_vel.y > ) 
    for(auto&x : mComponents){
        x->Update(deltaTime);
    }

    //��e�����ɃL�������ړ�������
    if (IsKnockBack()) {
        KnockBack(deltaTime);
    }
}

void CharacterBase::Draw(const float deltaTime)
{
    //�`��
    //DrawFormatString(0, 0, GetColor(255, 0, 0), "CharacterBase::Update velocity is (%f,%f)",GetVelocity().x,GetVelocity().y);
    //DrawFormatString(0, 50, GetColor(255, 0, 0), "CharacterBase::Update drawRotation is %f",GetDrawRotation());

    if(mDamage){
        if(mCurrent_DamageAnimFrame<mDamage_MaxFrame){
            //�_���[�W����
            mCurrent_DamageAnimFrame+=deltaTime;
        }else{
            mDamage = false;
            mCurrent_DamageAnimFrame = 0.0f;
        }
    }

    auto scene = Singleton<SceneManager>::get_instance().GetCurrentScene();
    if(!scene){return;}

    //�_�~�[����
    if(mVisible && mCurrent_gHandle == -2){
        Vector2D<float> size = GetCollision().mSize / 2;
        auto map = scene->GetMap();
        
        auto localPosi = map->GetLocalFromWorldPosition(GetWorldPosition2D());
        int x1 = static_cast<int>(localPosi.x - size.x);
        int y1 = static_cast<int>(localPosi.y - size.y);
        int x2 = static_cast<int>(localPosi.x + size.x);
        int y2 = static_cast<int>(localPosi.y + size.y);

        //�l�p�̉�����`��
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
    //���]
    if(IsReverse()){
        leftTopDrawPosi = Vector2D<int>(x + width,y);
        rightDownDrawPosi = Vector2D<int>(x, y + height);
    }else{ //�ʏ�
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
    Actor::SetWorldPosition2D(worldP);
    mCollision.mLeftTop = worldP - (mCollision.mSize / 2);
}

void CharacterBase::SetWorldPosition2D(Vector2D<float> worldP, bool isMove)
{
    Actor::SetWorldPosition2D(worldP);
    mCollision.mLeftTop = worldP - (mCollision.mSize / 2);
    mIsMove = isMove;
}

float CharacterBase::GetMoveSpeed() const
{
    return 1.0f;
}

void CharacterBase::TakeDamage(float damage, Vector2D<float>direction, float strength, float knockBackTime)
{
    if(IsDead()){return;}
    //HP�����炷
    mHP-=damage;

    //��e����炷
    if(GetActorType()==CharacterType::ENEMY|| GetActorType() == CharacterType::BOSS){
        SOUND_M.PlaySoundByID(mHitSound);
    }

    //HP�O�̏ꍇ�A���S�A�j���[�V�����ֈڍs
    if(mHP<=0){
        mHP = 0.0f;
        StartDeadAnimation();
        return;
    }

    //�Ռ�������Ȃ�A�m�b�N�o�b�N�t�^
    if (strength > 0.0f) {
        AddKnockBack(direction,strength,knockBackTime);
    }
    //��_���[�W�A�j���[�V����
    DamageAnimation();
}

void CharacterBase::StartDeadAnimation()
{
    mKnockBack = false;
    mDamage = false;
    mIsDead = true;
    SetVelocity(Vector2D<float>(0, 0));
    //Dead�A�j���[�V�������K�v���ǂ���
    if(!HasAnimationState("Dead")){//�K�v�Ȃ���΁A���̂܂܏���
        mVisible = false;
        SetActive(false);
        return;
    }

    //Dead�A�j���[�V�����֑J��
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
    //�m�b�N�o�b�N������
    mKnockBackTime = 0.0f;
    mKnockBackDirection = Vector2D<float>(0, 0);
    mKnockBackStrength = 0.0f;
    mKnockBack = false;
    mCurrentKnockBackTime = 0.0f;
}

void CharacterBase::AddKnockBack(Vector2D<float> direction, float strength, float knockBackTime)
{
    //�m�b�N�o�b�N�t�^
    mKnockBack = true;
    mKnockBackStrength += strength;
    mKnockBackDirection += direction;
    mKnockBackTime += knockBackTime;
}

void CharacterBase::KnockBack(float deltaTime)
{
    mCurrentKnockBackTime += deltaTime;
    //�m�b�N�o�b�N���Ԃ��I����Ă���Ȃ�A�m�b�N�o�b�N��������
    if(mCurrentKnockBackTime >= mKnockBackTime){
        //DEBUG_HELPER.Add("knockBack is " + GetWorldPosition2D().ToString(), 0.5f);
        InitKnockBack();
        return;
    }

    //�m�b�N�o�b�N�̑傫���A�m�b�N�o�b�N���̍��W�v�Z
    float knockbackRatio = (float)pow(1.0f - (mCurrentKnockBackTime / mKnockBackTime), 2);
    Vector2D<float> newPosi = mKnockBackDirection * mKnockBackStrength * knockbackRatio * deltaTime;
    newPosi += GetWorldPosition2D();

    //���W�X�V
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

const Collision::Rect& CharacterBase::GetCollision() const
{
    return mCollision;
}

bool CharacterBase::IsKnockBack()
{
    return mKnockBack;
}

bool CharacterBase::IsDead()
{
    return mIsDead;
}
