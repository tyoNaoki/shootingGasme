#include "Bomb.h"
#include "CollisionManager.h"
#include "ActorManager.h"
#include "CharacterBase.h"
#include "GraphicManager.h"
#include "PlayerCharacter.h"
#include "BossBase.h"

Bomb::Bomb(Vector2D<float> drawOffset): Actor()
{
    mStartAnimStateName = "Bomb";
    mDrawOffset = drawOffset;
}

Bomb::~Bomb()
{
    DeleteGraph(mCurrent_gHandle);
}

void Bomb::Init(CharacterType ct, CharacterType ownerType, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    mIgnoreCharas.clear();
    //������
    Initialize(ct, typeName, id, localPosition, worldPosition, rotation, isVisible);

    mOwnerType = ownerType;
    //���e�̎g�p�҂��v���C���[�Ȃ�
    if (ownerType == CharacterType::PLAYER) {
        auto chara = std::dynamic_pointer_cast<CharacterBase>(ACTOR_M.GetCurrentPlayer());
        //�����̏Փ˔��肩��v���C���[�����O
        mIgnoreCharas.push_back(chara);
    }

    //�����A�N�V�����X�e�[�g�擾
    ANIM_M.InitializeStates(this, mStartAnimStateName);

    //�����摜�擾
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, typeName, mStartAnimStateName);

    mIsExplosion = false;
    mInitialize = true;
}

void Bomb::Reset(CharacterType ownerType, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    mIgnoreCharas.clear();
    //������
    Initialize(GetActorType(), GetTypeName(), id, localPosition, worldPosition, rotation, isVisible);

    mOwnerType = ownerType;
    //���e�̎g�p�҂��v���C���[�Ȃ�
    if (ownerType == CharacterType::PLAYER) {
        auto chara = std::dynamic_pointer_cast<CharacterBase>(ACTOR_M.GetCurrentPlayer());
        if (!chara) { return; }

        //�����̏Փ˔��肩��v���C���[�����O
        mIgnoreCharas.push_back(chara);
    }
    //�����A�N�V�����X�e�[�g�擾
    ANIM_M.InitializeStates(this, mStartAnimStateName);

    //�����摜�擾
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, GetTypeName(), mStartAnimStateName);

    mIsExplosion = false;
    mInitialize = true;
}

bool Bomb::IsInitialize()
{
    return mInitialize;
}

void Bomb::Update(const float deltaTime)
{
    auto scene = SCENE_M.GetCurrentScene();
    if (!scene || !mInitialize) { return; }
    
    //�������Ă��鎞�̂ݏՓ˔���
    if(mIsExplosion){
        currentTime -=deltaTime;
        auto enemies = COLLISION_M.GetNearCharacters(GetWorldPosition2D(), mExplosionRange, mIgnoreCharas);
        if (enemies.size() != 0 || ACTOR_M.IsSpawnBoss()) {
            auto circle = Collision::Circle(GetWorldPosition2D(),mExplosionRange);
            DEBUG_HELPER.DrawCollision(circle);

            //�G�����̓����蔻��ƃ_���[�W����
            for(auto&e:enemies){
                HitCheck(deltaTime,e,circle,mKnockBackStrength,mAttack,mHasContinuousDamage);
            }

            //�{�X�̓����蔻��ƃ_���[�W����
            if (ACTOR_M.IsSpawnBoss()) {
                HitCheck(deltaTime,ACTOR_M.GetCurrentBossEnemy(),circle,mKnockBackStrength,mAttack,mHasContinuousDamage);
            }
        }
        
        //���݂̃A�j���[�V�����摜�X�V
        Singleton<GraphicManager>::get_instance().updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName(),mExplosionAnimSpeed);

        //�������I��������A�N�e�B�u�ɂ���
        if(currentTime <= 0.0f){
            SetActive(false);
            return;
        }
        return;
    }

    //��������
    if(currentTime <= 0.0f){
        mIsExplosion = true;
        //�A�j���[�V�����摜�X�V
        ANIM_M.Update(this);
        //����炷
        SOUND_M.PlaySoundByID(soundName);
        //�摜�̃A�N�V�����X�e�[�g��ʏ킩�甚���ɑJ��
        GRAPHIC_M.TransitionToAction(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, GetTypeName(), GetCurrentAnimState()->GetActionName());
        //�������Ă��鎞�Ԃ�ݒ�
        currentTime = mExplosionTime;
        return;
    }

    currentTime -=deltaTime;

    //���݂̃A�j���[�V�����摜�X�V
    Singleton<GraphicManager>::get_instance().updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName());
}

void Bomb::Draw(const float deltaTime)
{
    auto map = SCENE_M.GetCurrentScene()->GetMap();
    auto localPosi = map->GetLocalFromWorldPosition(GetWorldPosition2D());
    if (mVisible && mCurrent_gHandle == -2) {
        int x = static_cast<int>(localPosi.x);
        int y = static_cast<int>(localPosi.y);
        int r = 20;
        if(mIsExplosion){
            r = static_cast<int>(mExplosionRange);
        }

        if (mOwnerType == CharacterType::PLAYER) {
            DrawCircle(x, y, r, GetColor(200, 200, 200), TRUE);
        }
        else {
            DrawCircle(x, y, r, GetColor(150, 0, 0), TRUE);
        }

        return;
    }

    float x = localPosi.x + mDrawOffset.x;
    float y = localPosi.y + mDrawOffset.y;

    Vector2D<float> drawExtendSize;
   
    if(mIsExplosion){//�������A�����̃T�C�Y�ɐ؂�ւ���
        drawExtendSize = mDrawExtendSize;
    }else{//�ʏ펞�A��p�T�C�Y�g�p
        drawExtendSize = mBombSize;
    }
    
    //�`��֌W
    float width = mDrawSize.x * drawExtendSize.x;
    float height = mDrawSize.y * drawExtendSize.y;
    Vector2D<float>leftTopDrawPosi = Vector2D<float>(x, y);
    Vector2D<float>rightDownDrawPosi = Vector2D<float>(x + width, y + height);

    if (mVisible && mCurrent_gHandle != -1) {
        float angle = Vector2D<float>::GetRadiansFromDegrees(GetRotation());
        DrawRotaGraph(x, y, drawExtendSize.x, angle, mCurrent_gHandle, TRUE, FALSE);
    }
}

void Bomb::SetStatus(Vector2D<float>bombSize,Vector2D<float>explosionSize, float explosionAnimSpeed, float attack, float knockBack, float timeToExplode, float explosionTime, float explosionRange, bool hasContinuousDamage)
{
    //�X�e�[�^�X�ݒ�
    mBombSize = bombSize;
    mDrawExtendSize = explosionSize;
    mExplosionAnimSpeed = explosionAnimSpeed;
    mAttack = attack;
    mKnockBackStrength = knockBack;
    currentTime = timeToExplode;
    mExplosionTime = explosionTime;
    mExplosionRange = explosionRange;
    mHasContinuousDamage = hasContinuousDamage;
}

void Bomb::ApplyDrawOffset(Vector2D<float> offset)
{
    mDrawOffset = offset;
}

bool Bomb::IsExplosion() const
{
    return mIsExplosion;
}

bool Bomb::IsDamage(std::shared_ptr<CharacterBase>& enemy)
{
    //���i�q�b�g����
    auto it = mHitCountEnemies.find(enemy->GetName());
    if (enemy->GetActorType() == CharacterType::ENEMY || enemy->GetActorType() == CharacterType::BOSS) {
        //���߂ăq�b�g�����G
        if (it == mHitCountEnemies.end()) {
            return true;
        }

        //���G���Ԍo�߂������ǂ���
        return it->second <= 0.0f;
    }

    return false;
}

<<<<<<< HEAD
void Bomb::HitCheck(float deltaTime,std::shared_ptr<CharacterBase> enemy,Collision::Circle& circle, float knockBackStrength, float attack,bool hasContinuousDamage)
{
    //����
    if (!enemy->GetCollision()->HandleCollision(circle)) {
=======
void Bomb::HitCheck(float deltaTime,std::shared_ptr<CharacterBase> enemy, const Collision::Circle& circle, float knockBackStrength, float attack,bool hasContinuousDamage)
{
    //����
    if (!Collision::IsColliding(circle, enemy->GetCollision())) {
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
        return;
    }

    if (IsDamage(enemy)) {//���i�q�b�g�̃N�[���^�C���`�F�b�N
        //�_���[�W����
        float targetRadian = (float)Vector2D<float>::GetLookAtRadian(GetWorldPosition2D(), enemy->GetWorldPosition2D());
        enemy->TakeDamage(attack, Vector2D<float>(cos(targetRadian), sin(targetRadian)), knockBackStrength,0.4f);
        mHitCountEnemies[enemy->GetName()] = mHitCoolTime;
    }else{
        if(hasContinuousDamage){//�����_���[�W�^�̂݃N�[���^�C���X�V
            mHitCountEnemies[enemy->GetName()] -= deltaTime;
        }
    }
}
