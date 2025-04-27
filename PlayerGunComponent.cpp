#include "PlayerGunComponent.h"
#include "CollisionManager.h"
#include "WeaponStatus.h"
#include "SoundManager.h"
#include "ActorManager.h"

PlayerGunComponent::PlayerGunComponent(std::shared_ptr<CharacterBase> owner):WeaponComponent(owner)
{
	//������
	mComponentName = "PlayerGunComponent";
	Init();
}

void PlayerGunComponent::Init()
{
	//�X�e�[�^�X�ݒ�
	SetStatus(STATUS.GetCurrentWeaponStatus("PlayerGunComponent"));
}

void PlayerGunComponent::Update(const float deltaTime)
{
	auto owner = GetWeaponOwner();

	if(!IsEnable()||!owner->IsActive() ||owner->IsDead()){return;}

	//�N�[���^�C��
	if (mCurrentTime > 0) 
	{
		mCurrentTime -= deltaTime;
	}

	std::shared_ptr<CharacterBase>target;
	if(CanShoot(owner,target))
	{
		Shoot(owner,target);
	}
}

void PlayerGunComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//�X�e�[�^�X�ݒ�
	auto gunStatus = std::dynamic_pointer_cast<GunWeaponStatus>(status);
	mAttack = gunStatus->mAttack;
	mShotRate = gunStatus->mShotRate.GetValue();
	mBulletSpeed = gunStatus->mBulletSpeed;
	mLockOnRange = gunStatus->mLockOnRange;
	mIsPenetration = gunStatus->mIsPenetration;
	mPenetrationMaxCount = gunStatus->mPenetrationMaxCount;
}

bool PlayerGunComponent::CanShoot(const std::shared_ptr<CharacterBase>owner,std::shared_ptr<CharacterBase>&target)
{
	//�ˌ��N�[���^�C������
	if(mCurrentTime > 0)
	{
		return false;
	}

	//���͂ɓG�����邩�ǂ���
	std::vector<std::weak_ptr<CharacterBase>>charas{ owner };
	auto nearEnemies = COLLISION_M.GetNearCharacters(owner->GetWorldPosition2D(), mLockOnRange, charas);
	if (nearEnemies.size() == 0 && !ACTOR_M.IsSpawnBoss()) {
		return false;
	}

	//��ԋ߂��G���Z�b�g
	target = FindNearEnemy(owner,nearEnemies);
	return true;
}

void PlayerGunComponent::Shoot(const std::shared_ptr<CharacterBase>owner,const std::shared_ptr<CharacterBase>target)
{
	if (!owner||!target) { return; }

	//�����Ƒ_���Ώۂ܂ł̊p�x���擾
	float targetRot = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), target->GetWorldPosition2D());
	//�e�X�|�[��
	auto bullet = ACTOR_M.GetBullet(mBulletName.c_str(), owner->GetActorType(), owner->GetLocalPosition2D(), owner->GetWorldPosition2D(), targetRot, true);
	//�e���A�N�e�B�u�ɂ���
	if (!bullet) {
		DEBUG_HELPER.Add("Bullet is generate error", 1.0f);
		return;
	}
	//�X�e�[�^�X�ݒ�
	bullet->SetStatus(mBulletSpeed, mAttack, mIsPenetration, mPenetrationMaxCount);
	//����炷
	SOUND_M.PlaySoundByID(mFireSound);
	//�N�[���^�C���ݒ�
	mCurrentTime = mShotRate;
}

std::shared_ptr<CharacterBase> PlayerGunComponent::FindNearEnemy(const std::shared_ptr<CharacterBase>owner,const std::vector<std::shared_ptr<CharacterBase>>nearEnemies)
{
	std::shared_ptr<CharacterBase>target;
	float minDistance = -1.0f;
	//���͂̓G�Ƃ̋���
	for (auto& x : nearEnemies) {
		float distance = (x->GetWorldPosition2D() - owner->GetWorldPosition2D()).Length_Square();
		//��ԋ߂��G��ΏۂɌ���
		if (minDistance > distance || minDistance == -1.0f) {
			minDistance = distance;
			target = x;
		}
	}

	//�{�X���X�|�[�����Ă��邩
	if (ACTOR_M.IsSpawnBoss()) {
		auto boss = ACTOR_M.GetCurrentBossEnemy();
		//�{�X�Ƃ̋����擾
		float distance = (float)(boss->GetWorldPosition2D() - owner->GetWorldPosition2D()).Length_Square();
		//�{�X����ԋ߂��ꍇ
		if (minDistance > distance || minDistance == -1.0f) {
			target = boss;
		}
	}

	return target;
}
