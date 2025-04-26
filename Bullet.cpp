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
    //������
    Initialize(ct, typeName, id, localPosition, worldPosition, rotation, isVisible);

    mOwnerType = ownerType;
    //���˂����I�[�i�[���v���C���[�Ȃ�
    if(ownerType == CharacterType::PLAYER){
        auto chara = std::dynamic_pointer_cast<CharacterBase>(ACTOR_M.GetCurrentPlayer());
        //�Փ˔��肩��v���C���[�����O
        mIgnoreCharas.push_back(chara);
    }
    //�A�N�V�����X�e�[�g������
    ANIM_M.InitializeStates(this,mStartAnimStateName);

    //�摜������
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,typeName,mStartAnimStateName);

    //�e�ϐ�������
    mLifeTime = mMaxLifeTime;
    mIsHit = false;
    mIsFinish = false;
    mInitialize = true;
}

void Bullet::Reset(CharacterType ownerType, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    mIgnoreCharas.clear();
    //������
    Initialize(GetActorType(), GetTypeName(), id, localPosition, worldPosition, rotation, isVisible);

    mOwnerType = ownerType;
    //���˂����I�[�i�[���v���C���[�Ȃ�
    if (ownerType == CharacterType::PLAYER) {
        auto chara = std::dynamic_pointer_cast<CharacterBase>(ACTOR_M.GetCurrentPlayer());
        if(!chara){return;}
        //�Փ˔��肩��v���C���[�����O
        mIgnoreCharas.push_back(chara);
    }
    //�A�N�V�����X�e�[�g������
    ANIM_M.InitializeStates(this, mStartAnimStateName);

    //�摜������
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,GetTypeName(), mStartAnimStateName);

    //�e�ϐ�������
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

    //�����A�j���[�V�����X�V
    if(mIsFinish){

        //DEBUG_HELPER.DrawCollision(&GetCollision());
        SetLocalPosition2D(GetLocalFromWorldPosi(GetWorldPosition2D()));
        ANIM_M.Update(this);
        //���݂̃A�j���[�V�����摜�X�V
        Singleton<GraphicManager>::get_instance().updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName());
        return;
    }

    //�������Ԃ��O�b�ȉ�
    if (mLifeTime <= 0.0f) {
        StartDeadAnimation();
        return;
    }

    //�z�[�~���O�e�̏ꍇ�A��������^�[�Q�b�g�܂ł̊p�x�������̊p�x�ɐݒ�
    if(mIsHoming){
        if(mHomingTarget||mHomingTarget->IsActive()){
            SetRotation((float)Vector2D<float>::GetLookAtAngle(GetWorldPosition2D(), mHomingTarget->GetWorldPosition2D()));
        }
    }

    //�p�x����i�s�x�N�g�����v�Z
    double rad = Vector2D<float>::GetRadiansFromDegrees(GetRotation());
    auto unitVec = Vector2D<float>::GetUnitVectorByRadian(rad);
    //�ړ��ʂ��v�Z
    float speed = GetMoveSpeed();
    auto moveVel = unitVec * speed * deltaTime;
    //���W�v�Z
    auto newPosi = moveVel + GetWorldPosition2D();

    auto collision = GetCollision<Collision::Circle>();

    //���˒e��p����
    if (mIsReflection) {
        int width, height;
        GetWindowSize(&width, &height);

        auto localPosi = GetLocalFromWorldPosi(newPosi);
        bool refrection = false;
        
        //�v���C���[�̉�ʊO�̏ꍇ�A�ړ��x�N�g���𔽓]������
        if (localPosi.x < collision->mRadius|| localPosi.x > width - collision->mRadius){
            moveVel.x *= -1;
            refrection = true;
        }
        if (localPosi.y < collision->mRadius|| localPosi.y > height - collision->mRadius){
            moveVel.y *= -1;
            refrection = true;
        }
        //���˃t���O�������Ă���ꍇ
        if(refrection){
            //���ˉ񐔂��ő�ɒB���Ă����ꍇ�A��������
            if(mReflectionMaxCount<mReflectionCurrentCount + 1){
                StartDeadAnimation();
                return;
            }
            //���݂̔��ˉ񐔂��L�^
            mReflectionCurrentCount++;
            //�p�x�A���W�Đݒ�
            SetRotation(Vector2D<float>::GetLookAtAngle(moveVel));
            newPosi = moveVel + GetWorldPosition2D();
        }
    }

    SetVelocity(moveVel);

    //�}�b�v�O�ɂ����ꍇ�A����
    if (!COLLISION_M.IsBulletInMap(newPosi,scene->GetMap())) {
        auto localPosi = GetLocalFromWorldPosi(newPosi);
        std::string message = "Bullet is out";
        DEBUG_HELPER.Add(message);
        SetActive(false);
        return;
    }

    mIsHit = false;
    //�v���C���[����
    if(mOwnerType == CharacterType::PLAYER){
        //���͂̓G����
        auto enemies = COLLISION_M.GetNearCharacters(newPosi, collision->mRadius, mIgnoreCharas);

        if(enemies.size()!= 0 || ACTOR_M.IsSpawnBoss()){

            for (auto& x : enemies){

                //�Փ˂��Ȃ�
                if (!collision->IsHit(*x->GetCollision())) {
                    continue;
                }

                HandleBulletHit(x);
            }
            //�{�X�p����
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
        //���L�����̏���
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

    //���W�X�V
    SetLocalPosition2D(localPosi);
    SetWorldPosition2D(newPosi);

    //DEBUG_HELPER.DrawCollision(mCollision);

    ANIM_M.Update(this);

    //���݂̃A�j���[�V�����摜�X�V
    Singleton<GraphicManager>::get_instance().updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(),GetCurrentAnimState()->GetActionName());

    mLifeTime -=deltaTime;
}

void Bullet::Draw(const float deltaTime)
{
    
    //�_�~�[����
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

    //�`��֌W
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
    //Dead�A�j���[�V�������K�v���ǂ���
    if (!HasAnimationState("Dead")) {//�K�v�Ȃ���΁A���̂܂܏���
        mVisible = false;
        SetActive(false);
        return;
    }

    //Dead�A�j���[�V�����֑J��
    ANIM_M.TransitionToState(this, "Dead");
    GRAPHIC_M.TransitionToAction(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, GetTypeName(), GetCurrentAnimState()->GetActionName());
}

void Bullet::FinishDeadAnimation()
{
    //�`��t���O���낷
    mVisible = false;
    //��A�N�e�B�u��
    SetActive(false);
}

void Bullet::HandleBulletHit(std::shared_ptr<CharacterBase>target)
{
    //�ђʒe�ł͂Ȃ��ꍇ�A���̏�ŏ����B
                //�ђʒe�̏ꍇ�A�G�����̂݃_���[�W�A�{�X���i�q�b�g
    if (IsFinish(target)) {
        target->TakeDamage(mAttack);

        if(target->GetActorType()==CharacterType::ENEMY)//�G���G�ɂ̓m�b�N�o�b�N������
        {
            float targetRadian = (float)Vector2D<float>::GetLookAtRadian(GetWorldPosition2D(), target->GetWorldPosition2D());
            target->AddKnockBack(Vector2D<float>(cos(targetRadian), sin(targetRadian)), mShock, 0.2f);
        }

        mIsHit = true;
        StartDeadAnimation();
        return;
    }

    //�ђʃ_���[�W������Ȃ�
    if (!IsPenetratingHit(target)) {
        return;
    }

    //�e�̌��ݍ��W����q�b�g�����G�̍��W�܂ł̊p�x�œG���m�b�N�o�b�N������
    target->TakeDamage(mAttack);
    if (target->GetActorType() == CharacterType::ENEMY)//�G���G�ɂ̓m�b�N�o�b�N������
    {
        float targetRadian = (float)Vector2D<float>::GetLookAtRadian(GetWorldPosition2D(), target->GetWorldPosition2D());
        target->AddKnockBack(Vector2D<float>(cos(targetRadian), sin(targetRadian)), mShock, 0.2f);
    }

    mIsHit = true;

    //�ђʂ̃N�[���^�C���ݒ�
    mHitCountEnemies[target->GetName()] = mPenetrationCoolTime;
    //���݂̊ђʉ�
    mPenetrationCurrentCount++;
}

void Bullet::SetStatus(float speed,float attack,bool IsPenetration,int penetrationMaxCount)
{
    //�X�e�[�^�X�ݒ�
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
    //�z�[�~���O��p�̃X�e�[�^�X�ݒ�
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
    //���˒e��p�X�e�[�^�X�ݒ�
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
    //�ђʒe�ł͂Ȃ��A�ђʉ񐔂��ő吔�ɒB���Ă���
    return (!mIsPenetration || !CanPenetratingOnCurrentCount() && IsPenetratingHit(target));
}


bool Bullet::IsPenetratingHit(std::shared_ptr<CharacterBase>target)
{
    //���܂Ńq�b�g�����G���擾
    auto it = mHitCountEnemies.find(target->GetName());
    if(target->GetActorType()==CharacterType::ENEMY || target->GetActorType() == CharacterType::BOSS){
        //���߂ăq�b�g�����G
        if (it == mHitCountEnemies.end()) {
            return true;
        }
        
        //���G���Ԍo�߂������ǂ���
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
