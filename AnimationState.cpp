#include "AnimationState.h"
#include "Bomb.h"
#include "GraphicManager.h"
#include "ActorManager.h"

AnimationManager::AnimationManager()
{
    //�e�A�N�V�����o�^
    mAllState["Idle"] = std::make_shared<IdleState>();
    mAllState["Walking"] = std::make_shared<WalkingState>();
    mAllState["Flight"] = std::make_shared<FlightState>();
    mAllState["Dead"] = std::make_shared<DeadState>();
    mAllState["Laser"] = std::make_shared<LaserState>();
    mAllState["Bomb"] = std::make_shared<BombState>();
    mAllState["Explosion"] = std::make_shared<ExplosionState>();
    mAllState["BossIdle"] = std::make_shared<BossIdleState>();
    mAllState["BossAttack"] = std::make_shared<BossAttackState>();
}

void AnimationManager::InitializeStates(Actor* actor,std::string actionName)
{
    actor->SetAnimationState(mAllState[actionName]);
}

void AnimationManager::Update(Actor* actor)
{
    //�A�N�V�����̏������򔻒�
    auto state = actor->GetCurrentAnimState();
    state->HandleInput(actor);
}

void AnimationManager::TransitionToState(Actor* actor, const std::string& actionName)
{
    //�A�N�V���������݂��邩
    if (mAllState.find(actionName) != mAllState.end()) {
        actor->SetAnimationState(mAllState[actionName]);
    }
}

AnimationManager::~AnimationManager()
{
}

void IdleState::HandleInput(Actor* actor)
{
    //�ړ����Ȃ�ړ��X�e�[�g�ɑJ��
    if(actor->IsMoving()){
        ANIM_M.TransitionToState(actor, "Walking");
    }
}

std::string IdleState::GetActionName() const
{
    return "Idle";
}

void WalkingState::HandleInput(Actor* actor)
{

    //�����Ă��Ȃ��ꍇ�AIdle�ɑJ��
    if (!actor->IsMoving()) {
        ANIM_M.TransitionToState(actor, "Idle");
        return;
    }
}

std::string WalkingState::GetActionName() const
{
    return "Walking";
}

void FlightState::HandleInput(Actor* actor){}

std::string FlightState::GetActionName() const
{
    return "Flight";
}

void DeadState::HandleInput(Actor* actor)
{
    //���S�A�j���[�V�������I���������Ƃ����S�L�����ɒʒm
    if(GRAPHIC_M.IsFinishAction(actor->GetCurrentAnimFrame(),actor->GetCurrentAnimFrameAdjust(),actor->GetTypeName(),GetActionName())){
        actor->FinishDeadAnimation();
    }
}

std::string DeadState::GetActionName() const
{
    return "Dead";
}

void BossIdleState::HandleInput(Actor* actor)
{
    if(actor->GetActorType() != CharacterType::BOSS){
        return;
    }
    //�{�X�̃X�e�[�g���U���Ȃ�A�N�V�����X�e�[�g���U���ɑJ��
    if(ACTOR_M.GetCurrentBossEnemy()->GetCurrentState()==BossState::Attack){
        ANIM_M.TransitionToState(actor, "BossAttack");
        return;
    }
}

std::string BossIdleState::GetActionName() const
{
    return "BossIdle";
}

void BossAttackState::HandleInput(Actor* actor)
{
    if (actor->GetActorType() != CharacterType::BOSS) {
        return;
    }
    //�{�X���U�����Ă��Ȃ��Ȃ�AIdle�ɑJ��
    if (ACTOR_M.GetCurrentBossEnemy()->GetCurrentState() != BossState::Attack) {
        ANIM_M.TransitionToState(actor, "BossIdle");
        return;
    }
}

std::string BossAttackState::GetActionName() const
{
    //�U����ԂȂ�A���ݎg���Ă���U���̖��O�擾
    if (ACTOR_M.GetCurrentBossEnemy()->GetCurrentState() == BossState::Attack) {
        return ACTOR_M.GetCurrentBossEnemy()->GetCurrentWeaponName();
    }
    return "EMPTY";
}

void LaserState::HandleInput(Actor* actor)
{
}

std::string LaserState::GetActionName()const
{
    return "Laser";
}

void BombState::HandleInput(Actor* actor)
{
    if(!bomb){
        bomb = dynamic_cast<Bomb*>(actor);
    }

    //���������Ȃ�A�����X�e�[�g�ɑJ��
    if(bomb&&bomb->IsExplosion()){
        ANIM_M.TransitionToState(actor, "Explosion");
    }
}

std::string BombState::GetActionName() const
{
    return "Bomb";
}

void ExplosionState::HandleInput(Actor* actor)
{
}

std::string ExplosionState::GetActionName() const
{
    return "Explosion";
}
