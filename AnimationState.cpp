#include "AnimationState.h"
#include "Bomb.h"
#include "GraphicManager.h"
#include "ActorManager.h"

AnimationManager::AnimationManager()
{
    //各アクション登録
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
    //アクションの条件分岐判定
    auto state = actor->GetCurrentAnimState();
    state->HandleInput(actor);
}

void AnimationManager::TransitionToState(Actor* actor, const std::string& actionName)
{
    //アクションが存在するか
    if (mAllState.find(actionName) != mAllState.end()) {
        actor->SetAnimationState(mAllState[actionName]);
    }
}

AnimationManager::~AnimationManager()
{
}

void IdleState::HandleInput(Actor* actor)
{
    //移動中なら移動ステートに遷移
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

    //動いていない場合、Idleに遷移
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
    //死亡アニメーションが終了したことを死亡キャラに通知
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
    //ボスのステートが攻撃ならアクションステートを攻撃に遷移
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
    //ボスが攻撃していないなら、Idleに遷移
    if (ACTOR_M.GetCurrentBossEnemy()->GetCurrentState() != BossState::Attack) {
        ANIM_M.TransitionToState(actor, "BossIdle");
        return;
    }
}

std::string BossAttackState::GetActionName() const
{
    //攻撃状態なら、現在使っている攻撃の名前取得
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

    //爆発したなら、爆発ステートに遷移
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
