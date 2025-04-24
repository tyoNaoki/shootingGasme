#include "BossEnemyA.h"
#include "BossMoveComponent.h"
#include "BossTackleComponent.h"
#include "BossLaserComponent.h"
#include "BossGunComponent.h"
#include "GraphicManager.h"
#include <random>

<<<<<<< HEAD
BossEnemyA::BossEnemyA():BossBase()
=======
BossEnemyA::BossEnemyA(Collision::Rect rect):BossBase(rect)
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
{
    mDrawOffset = Vector2D<float>(-240, -240);
    mDrawExtendSize = Vector2D<float>(5,5);
}

void BossEnemyA::Init(CharacterType ct, std::string name, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    BossBase::Init(ct,name,id,localPosition,worldPosition,rotation,isVisible);

    //�Փ˔��菜�O�p
    std::shared_ptr<CharacterBase> self = std::static_pointer_cast<CharacterBase>(shared_from_this());
    //�R���|�[�l���g�ǉ�
<<<<<<< HEAD
    GetComponentManager().AddComponent<BossMoveComponent>(0,self);
    GetComponentManager().AddComponent<BossTackleComponent>(1,self);
    GetComponentManager().AddComponent<BossLaserComponent>(2,self);
    GetComponentManager().AddComponent<BossGunComponent>(3, self);
=======
    AddComponent(std::make_shared<BossMoveComponent>(self), 0 );
    AddComponent(std::make_shared<BossTackleComponent>(self), 1);
    AddComponent(std::make_shared<BossLaserComponent>(self),2);
    AddComponent(std::make_shared<BossGunComponent>(self), 3);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
}

void BossEnemyA::Update(float deltaTime)
{
    auto scene = Singleton<SceneManager>::get_instance().GetCurrentScene();
    if (!scene) { return; }

    //���x�擾
    Vector2D<float> cur_vel = GetVelocity();

    //�����Ă��邩�ǂ���
    bool isMove = IsMoving();

    switch (GetCurrentState())
    {
    case BossState::Move://�ړ�
<<<<<<< HEAD
        GetComponentManager().GetComponent<BossMoveComponent>()->Update(deltaTime);
        //�ړ����Ɏ����ɐG�ꂽ�v���C���[�Ƀ^�b�N���_���[�W��^����
        GetComponentManager().GetComponent<BossTackleComponent>()->Update(deltaTime);
=======
        GetComponent("BossMoveComponent")->Update(deltaTime);
        //�ړ����Ɏ����ɐG�ꂽ�v���C���[�Ƀ^�b�N���_���[�W��^����
        GetComponent("BossTackleComponent")->Update(deltaTime);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
        break;
    case BossState::Attack://�U��
        mAttackComponent->Update(deltaTime);
        break;
    case BossState::Wait://��~

        //70%�Œe����
        if(IsChooseLeft(0.7f)){
<<<<<<< HEAD
            mAttackComponent = GetComponentManager().GetComponent<BossGunComponent>();
            
        }else{//30%�Ń��[�U�[
            mAttackComponent = GetComponentManager().GetComponent<BossLaserComponent>();
=======
            mAttackComponent = GetComponent("BossGunComponent");
            
        }else{//30%�Ń��[�U�[
            mAttackComponent = GetComponent("BossLaserComponent");
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
        }
        //���݂̍U���̖��O�擾
        mWeaponName = mAttackComponent->GetName();
        //��~��Ԃ̏I������
        FinishState();
        break;
    case BossState::Dead:
        FinishState();
        break;
    default:
        break;
    }

    //���݂̕`��p�x�擾
    float currentRotation = GetDrawRotation();
    DEBUG_HELPER.Add("boss Rotation is "+std::to_string(currentRotation));

    bool isReverse = false;
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

    mReverse = isReverse;

    auto newPosi = GetWorldPosition2D();
    auto localPosi = scene->GetMap()->GetLocalFromWorldPosition(newPosi);

    //���W�X�V
    SetLocalPosition2D(localPosi);
    SetWorldPosition2D(newPosi);

<<<<<<< HEAD
    //DEBUG_HELPER.DrawCollision(GetCollision());
=======
    DEBUG_HELPER.DrawCollision(GetCollision());
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420

    std::string preAction = GetCurrentAnimState()->GetActionName();
    ANIM_M.Update(this);

    if (preAction != GetCurrentAnimState()->GetActionName()) {
        GRAPHIC_M.TransitionToAction(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, GetTypeName(), GetCurrentAnimState()->GetActionName());
    }else {
        //���݂̃A�j���[�V�����摜�X�V
        GRAPHIC_M.updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName());
    }
}

void BossEnemyA::FinishState()
{
    switch (GetCurrentState())
    {
    case BossState::Move://�ړ�
        //��U���̏�Ŏ~�܂�
        ChangeState(BossState::Wait);
        break;
    case BossState::Attack://�U��
        //���݂̍U���R���|�[�l���g�ϐ���NULL�ŏ�����
        mAttackComponent = nullptr;
        //�ړ���ԂɑJ��
        ChangeState(BossState::Move);
        break;
    case BossState::Wait://��~��
        //�U�����ݒ肳��Ă���Ȃ�A�U����ԂɑJ��
        if(mAttackComponent){
            ChangeState(BossState::Attack);
            break;
        }
        //�ړ��ɑJ��
        ChangeState(BossState::Move);
        break;
    case BossState::Dead:
        
        break;
    default:
        break;
    }
}

bool BossEnemyA::IsChooseLeft(float leftWeight)
{
    if(0.0>leftWeight||1.0<leftWeight ){
        leftWeight = 0.0f;
    }
    // 0.0����1.0�܂ł͈̔͂ŗ����𐶐�����G���W��
    std::random_device rd; std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f); // �����𐶐����Ċm���Ɣ�r 
    float randomValue = distribution(generator);
    return randomValue < leftWeight;
}
