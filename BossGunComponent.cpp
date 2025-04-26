#include "BossGunComponent.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"
#include "WeaponStatus.h"
#include "BossBase.h"

BossGunComponent::BossGunComponent(std::shared_ptr<CharacterBase> owner) : WeaponComponent(owner)
{
	mComponentName = "BossGunComponent";
	mBossOwner = std::dynamic_pointer_cast<BossBase>(owner);
	Init();
}

void BossGunComponent::Init()
{
	SetStatus(STATUS.GetCurrentWeaponStatus(mComponentName));
}

void BossGunComponent::Update(const float deltaTime)
{
	auto owner = mBossOwner.lock();
	auto player = ACTOR_M.GetCurrentPlayer();

	if (!owner->IsActive() || !player || !player->IsActive()) { return; }

	//��������G�܂ł̊p�x�擾,�ݒ�
	float targetRot = (float)Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), player->GetWorldPosition2D());
	owner->SetRotation(targetRot);

	if (mCurrentTime > 0) {
		mCurrentTime -= deltaTime;
		return;
	}
	//�܂����˂��Ă��Ȃ��Ȃ�A�e�𔭎�
	if (!mIsFire) {
		FireBullet(owner, player);
		//�N�[���^�C���ݒ�
		mCurrentTime = mFireTime;
		//���˃t���O�����Ă�
		mIsFire = true;
	}else{
		//�񔭎ˏ�Ԃɐݒ�
		mCurrentTime = 0.0f;
		mIsFire = false;

		//�e�𔭎˂��I�������Ƃ�ʒm
		owner->FinishState();
	}

	
}

void BossGunComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//�e�̃X�e�[�^�X�ݒ�
	auto gunStatus = std::dynamic_pointer_cast<GunWeaponStatus>(status);
	mAttack = gunStatus->mAttack;
	mSpeed = gunStatus->mBulletSpeed;
	mIsHoming = true;
	mHomingTime = gunStatus->mHomingTime;
	mFireTime = 3.0f;
}

void BossGunComponent::FireBullet(std::shared_ptr<Actor>owner, std::shared_ptr<CharacterBase> homingTarget)
{
	//��������G�܂ł̊p�x�擾
	float targetRot = (float)Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), homingTarget->GetWorldPosition2D());
	owner->SetRotation(targetRot);

	//���ˉ���炷
	SOUND_M.PlaySoundByID(soundName);

	//��A�N�e�B�u�̒e���擾�A�X�|�[��
	auto bullet = ACTOR_M.GetBullet("BossBullet", owner->GetActorType(), owner->GetLocalPosition2D(), owner->GetWorldPosition2D(), targetRot, true);
	//�A�N�e�B�u��Ԃɐݒ�
	if (!bullet) {
		DEBUG_HELPER.Add("BossBullet is generate error", 1.0f);
	}
	//�e�̃X�e�[�^�X�ݒ�
	bullet->SetStatus(mSpeed, mAttack,mHomingTime,homingTarget);
}
