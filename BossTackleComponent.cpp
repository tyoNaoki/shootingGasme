#include "BossTackleComponent.h"
#include "BossBase.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"
#include "StatusManager.h"
#include "CollisionManager.h"

BossTackleComponent::BossTackleComponent(std::shared_ptr<CharacterBase> owner):WeaponComponent(owner)
{
	mComponentName = "BossTackleComponent";
	mBossOwner = std::dynamic_pointer_cast<BossBase>(owner);
	mPlayer = ACTOR_M.GetCurrentPlayer();
	SetStatus(STATUS.GetCurrentWeaponStatus(mComponentName));
}

void BossTackleComponent::Update(const float deltaTime)
{
	auto owner = mBossOwner.lock();

	if (!owner || !owner->IsActive() || owner->IsDead()||
		!mPlayer || !mPlayer->IsActive() || mPlayer->IsDead()) { return; }

	//クールタイムがおわっているか
	if(mCurrentTime > 0){
		mCurrentTime -=deltaTime;
		return;
	}

	//プレイヤーとの衝突判定
	bool isHit = COLLISION_M.IsCollidingCharacter(owner, mPlayer);
	if(isHit){
		//自分からプレイヤーまでの角度をノックバックとして、プレイヤーに与える
		mPlayer->TakeDamage(mTackleDamage);
		float targetRadian = Vector2D<float>::GetLookAtRadian(owner->GetWorldPosition2D(), mPlayer->GetWorldPosition2D());
		mPlayer->AddKnockBack(Vector2D<float>(cos(targetRadian), sin(targetRadian)), mTackleKnockBack, 0.5f);
		mCurrentTime = mTackleCoolTime;
	}
}

void BossTackleComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//ステータス設定
	auto tackleStatus = std::dynamic_pointer_cast<TackleWeaponStatus>(status);
	mTackleDamage = tackleStatus->mAttack;
	mTackleKnockBack = tackleStatus->mTackleKnockBack;
}
