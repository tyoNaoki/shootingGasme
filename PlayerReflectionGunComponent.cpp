#include "PlayerReflectionGunComponent.h"
#include "CollisionManager.h"
#include "StatusManager.h"
#include "ActorManager.h"

PlayerReflectionGunComponent::PlayerReflectionGunComponent(std::shared_ptr<CharacterBase> owner) :WeaponComponent(owner)
{
	//������
	mComponentName = "PlayerReflectionGunComponent";
	Init();
}

void PlayerReflectionGunComponent::Init()
{
	//�X�e�[�^�X�ݒ�
	SetStatus(STATUS.GetCurrentWeaponStatus(mComponentName));
}

void PlayerReflectionGunComponent::Update(const float deltaTime)
{
	auto owner = GetWeaponOwner();

	if (!IsEnable() || !owner->IsActive() || owner->IsDead()) { return; }

	//�N�[���^�C�����I����Ă��邩
	if (mCurrentTime > 0) {
		mCurrentTime -= deltaTime;
		return;
	}

	//���͂̒T������͈͂���I�[�i�[������
	std::vector<std::weak_ptr<CharacterBase>>charas{ owner };
	//���͂ɓG�����邩�ǂ���
	auto nearEnemies = COLLISION_M.GetNearCharacters(owner->GetWorldPosition2D(),mLockOnRange, charas);
	if (nearEnemies.size() == 0 && !ACTOR_M.IsSpawnBoss()) {
		return;
	}

	float minDistance = -1.0f;
	std::shared_ptr<CharacterBase>target;
	//���͂̓G�Ƃ̋����v�Z
	for (auto& x : nearEnemies) {
		float distance = (x->GetWorldPosition2D() - owner->GetWorldPosition2D()).Length_Square();
		//��ԋ߂��G��Ώۂɐݒ�
		if (minDistance > distance || minDistance == -1.0f) {
			minDistance = distance;
			target = x;
		}
	}
	//�{�X���X�|�[������Ă��邩
	if (ACTOR_M.IsSpawnBoss()) {
		auto boss = ACTOR_M.GetCurrentBossEnemy();
		//�{�X�Ƃ̋����v�Z
		float distance = (boss->GetWorldPosition2D() - owner->GetWorldPosition2D()).Length_Square();
		//�{�X����ԋ߂��ꍇ
		if (minDistance > distance || minDistance == -1.0f) {
			target = boss;
		}
	}

	if (!target) { return; }
	//�Ώۂ܂ł̊p�x�𔭎ˊp�x�Ƃ��Ď擾
	float targetRot = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), target->GetWorldPosition2D());
	//�e�𔭎�
	//�e���A�N�e�B�u�ɂ���
	auto bullet = ACTOR_M.GetBullet(mBulletName.c_str(), owner->GetActorType(), owner->GetLocalPosition2D(), owner->GetWorldPosition2D(), targetRot, true);

	if(!bullet) {
		DEBUG_HELPER.Add("Bullet is generate error", 1.0f);
		return;
	}
	
	//�X�e�[�^�X�ݒ�
	bullet->SetStatus(mBulletSpeed, mAttack, mBulletLifeTime, mReflectionCount);
	//����炷
	SOUND_M.PlaySoundByID(mFireSound);
	//�N�[���^�C���ݒ�
	mCurrentTime = mShotRate;
}

void PlayerReflectionGunComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//�X�e�[�^�X�ݒ�
	auto gunStatus = std::dynamic_pointer_cast<GunWeaponStatus>(status);
	mAttack = gunStatus->mAttack;
	mShotRate = gunStatus->mShotRate;
	mReflectionCount = gunStatus->mReflectionMaxCount;
	mBulletLifeTime = 10.0f;
	mLockOnRange = gunStatus->mLockOnRange;
	mBulletSpeed = gunStatus->mBulletSpeed;
}
