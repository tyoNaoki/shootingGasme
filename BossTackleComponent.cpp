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

	//�N�[���^�C����������Ă��邩
	if(mCurrentTime > 0){
		mCurrentTime -=deltaTime;
		return;
	}

	//�v���C���[�Ƃ̏Փ˔���
	bool isHit = COLLISION_M.IsCollidingCharacter(owner, mPlayer);
	if(isHit){
		//��������v���C���[�܂ł̊p�x���m�b�N�o�b�N�Ƃ��āA�v���C���[�ɗ^����
		mPlayer->TakeDamage(mTackleDamage);
		float targetRadian = Vector2D<float>::GetLookAtRadian(owner->GetWorldPosition2D(), mPlayer->GetWorldPosition2D());
		mPlayer->AddKnockBack(Vector2D<float>(cos(targetRadian), sin(targetRadian)), mTackleKnockBack, 0.5f);
		mCurrentTime = mTackleCoolTime;
	}
}

void BossTackleComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//�X�e�[�^�X�ݒ�
	auto tackleStatus = std::dynamic_pointer_cast<TackleWeaponStatus>(status);
	mTackleDamage = tackleStatus->mAttack;
	mTackleKnockBack = tackleStatus->mTackleKnockBack;
}
