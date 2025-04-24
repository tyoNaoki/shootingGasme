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
    //������
    Initialize(ct, typeName, id, localPosition, worldPosition, rotation, isVisible);

    mOwner = owner;
    //�A�j���[�V�����X�e�[�g������
    ANIM_M.InitializeStates(this, mStartAnimStateName);
    //���[�U�[�̏Ə��t�߂̉摜�擾
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,typeName,mAnimStartName);
    //���[�U�[�̉摜�擾
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_laserHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, typeName, mAnimLaserName);

    mCurrentTime = 0.0f;
    mIsHit = false;
    mIsInitialize = true;
}

void Laser::Reset(std::shared_ptr<CharacterBase> owner, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    //������
    Initialize(GetActorType(), GetTypeName(), id, localPosition, worldPosition, rotation, isVisible);
    mOwner = owner;
    //�A�j���[�V�����X�e�[�g������
    ANIM_M.InitializeStates(this, mStartAnimStateName);
    //���[�U�[�̏Ə��t�߂̉摜�擾
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, GetTypeName(), mAnimStartName);
    //���[�U�[�̉摜�擾
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

    //���[�U�[�̒����v�Z
    mLaserLength += mSpeed*deltaTime;
    //���[�U�[�̓����蔻��͈̔͂��v�Z
    mRotRect = Collision::CalculateAttackRange(GetWorldPosition2D(),mLaserWidth, mLaserLength,GetRotation());
    mIsHit = false;
    auto mOwnerType = owner->GetActorType();

    //�{�X�A�G��p����
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
            //���[�U�[�̃m�b�N�o�b�N�p�x�����[�U�[�̏Ǝˈʒu����v���C���[�܂ł̊p�x�ƃ��[�U�[�̏Ǝˊp�x�Ƃ̍����Ōv�Z�A�ݒ肷��
            float laserRot = GetRotation();
            float targetRot = (float)Vector2D<float>::GetLookAtAngle(GetWorldPosition2D(),player->GetWorldPosition2D());
            float diff = Vector2D<float>::AngleDiff(laserRot,targetRot);
            float targetRadian = 0;
            if(diff > 0){
                targetRadian = Vector2D<float>::GetRadiansFromDegrees(laserRot - 20.0f);
            }else{
                targetRadian = Vector2D<float>::GetRadiansFromDegrees(laserRot + 20.0f);
            }
            //�_���[�W����
            ACTOR_M.GetCurrentPlayer()->TakeDamage(mAttack, Vector2D<float>(cos(targetRadian), sin(targetRadian)), mKnockBackStrength,mknockBackTime);

            mCurrentTime = mInvincibilityTime;
            mIsHit = true;
        }else if(mCurrentTime>0.0f){
            mCurrentTime-= deltaTime;
        }
    }
    //�A�j���[�V�����X�V
    ANIM_M.Update(this);
    //�摜�X�V
    GRAPHIC_M.updateHandle(deltaTime,mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(),mAnimStartName);
}

void Laser::Draw(const float deltaTime)
{
    //DEBUG_HELPER.DrawCollision(mRotRect, 0.2f, mIsHit);

    if (mVisible && mCurrent_gHandle == -2) {
        return;
    }

    //�`��֌W
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
        //���[�U�[�X�^�[�g�ʒu�`��
        DrawRotaGraph(drawPosi.x, drawPosi.y, 2.5f, angle, mCurrent_gHandle, TRUE);

        //���[�U�[�̒����̐��A�摜��`�悷��
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
