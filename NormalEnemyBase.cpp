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
    //������
    Initialize(ct, name, id, localPosition, worldPosition, rotation, isVisible);

    //�X�e�[�^�X�ݒ�
    auto status = STATUS.GetCurrentEnemyStatus(name);
    mLevel = status.level;
    mHP = status.hp;
    mMaxHP = status.hp;
    mMoveSpeed = status.moveSpeed;
    mScore = status.score;

    //�Փ˔��菜�O�p
    std::shared_ptr<NormalEnemyBase> self = std::static_pointer_cast<NormalEnemyBase>(shared_from_this());
    mCharaToIgnores = { self,ACTOR_M.GetCurrentPlayer()};

    //�R���|�[�l���g�ǉ�
    AddComponent(std::make_shared<EnemyChaseMoveComponent>(self), 0);
    AddComponent(std::make_shared<NormalEnemyAttackComponent>(self), 1);

    //�R���W�������W�X�V
    SetWorldPosition2D(worldPosition);
    //�m�b�N�o�b�N������
    InitKnockBack();
    //�A�j���[�V����������
    ANIM_M.InitializeStates(this, mStartAnimStateName);
    //�A�j���[�V����������
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,name,mStartAnimStateName);

    mInitialize = true;
}

void NormalEnemyBase::Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    if (id == -1) {
        mInitialize = false;
        return;
    }
    //������
    Initialize(GetActorType(), GetTypeName(), id, localPosition, worldPosition, rotation, isVisible);

    //�X�e�[�^�X�Đݒ�
    auto status = STATUS.GetCurrentEnemyStatus(GetTypeName());
    mLevel = status.level;
    mHP = status.hp;
    mMaxHP = status.hp;
    mMoveSpeed = status.moveSpeed;
    mScore = status.score;

    //�R���|�[�l���g������
    InitComponents();

    //�R���W�������W�X�V
    SetWorldPosition2D(worldPosition);
    //�m�b�N�o�b�N������
    InitKnockBack();
    //�A�j���[�V����������
    ANIM_M.InitializeStates(this, mStartAnimStateName);
    //�A�j���[�V����������
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,GetTypeName(), mStartAnimStateName);

    mIsDead = false;
    mInitialize = true;
}

void NormalEnemyBase::Update(const float deltaTime)
{
    auto scene = Singleton<SceneManager>::get_instance().GetCurrentScene();
    if (!scene) { return; }

    //����ł���ꍇ
    if (mIsDead) {
        DEBUG_HELPER.DrawCollision(GetCollision());
        SetLocalPosition2D(GetLocalFromWorldPosi(GetWorldPosition2D()));
        //�A�j���[�V�����X�V
        ANIM_M.Update(this);
        //���݂̃A�j���[�V�����摜�X�V
        Singleton<GraphicManager>::get_instance().updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName());
        return;
    }

    //�R���|�[�l���g�X�V
    CharacterBase::Update(deltaTime);

    //���x�擾
    Vector2D<float> cur_vel = GetVelocity();

    //�����Ă��邩�ǂ���
    bool isMove = IsMoving();

    //���݂̕`��p�x�擾
    float currentRotation = GetDrawRotation();

    //���������ǂ���
    bool isReverse = false;

    //�A�j���[�V�����摜�^�C�v�X�V(�ʏ���s�A���]���s)
    if (isMove) {
        if (nearEqual(currentRotation, 90.0f) || nearEqual(currentRotation, 270.0f)) {
            //�p�x��90�x��270�x�ɋ߂����l�Ȃ�A�����͂��̂܂܂̌����ŌŒ�
            isReverse = mReverse;
        }
        //�������ɂ�����
        else if (currentRotation > 90.0f && currentRotation < 270.0f) {
            isReverse = true;
            //�p�x��90�x��270�x�ɋ߂����l�Ȃ�����͂��̂܂܂ŕԂ�
        }
        else {
            //�E����
            isReverse = false;
        }
        //�~�܂��Ă��鎞
    }
    else if (!isMove) {
        isReverse = mReverse;
    }

    mReverse = isReverse;

    //�Փˌ��m//
    auto newPosi = GetWorldPosition2D();
    auto newCollisionPosi = GetCollision();

    //�}�b�v�Ƃ̓����蔻��Ǘ�
    auto map = scene->GetMap();

    //�}�b�v�O�ɂ����ꍇ�A���ɖ߂�
    if (COLLISION_M.PushBackFromOut(newPosi, newCollisionPosi, map)) {
        std::string message = "Enemy is out";
        DEBUG_HELPER.Add(message);
    }

    //�R���W�����̈�Ԓ�����Ԃ������ɐݒ�
    float dis = newCollisionPosi.mSize.x >= newCollisionPosi.mSize.y ? newCollisionPosi.mSize.x : newCollisionPosi.mSize.y;
    auto nearActors = COLLISION_M.DetectionNearCharacters(newPosi, newCollisionPosi, dis, mCharaToIgnores);
    if (nearActors.size() > 0) {
        for (auto& x : nearActors) {
            //�Փˎ��A�X�V�O�̍��W�ւ̕����ɏՓ˕��A�V�������W��߂��ĕԂ�
            if (Collision::IsColliding(newCollisionPosi, x->GetCollision())) {
                auto rect2 = x->GetCollision();
                Collision::PushBackRect(newCollisionPosi, rect2);
                newPosi = newCollisionPosi.mLeftTop + (newCollisionPosi.mSize / 2);
            }
        }
    }

    auto localPosi = scene->GetMap()->GetLocalFromWorldPosition(newPosi);

    //���W�X�V
    SetLocalPosition2D(localPosi);
    SetWorldPosition2D(newPosi);

    std::string preActionName = GetCurrentAnimState()->GetActionName();

    DEBUG_HELPER.DrawCollision(GetCollision());

    //�A�j���[�V�����J�ڍX�V
    ANIM_M.Update(this);

    //���݂̃A�j���[�V�����摜�X�V
    //�V�A�j���[�V�����J��
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
