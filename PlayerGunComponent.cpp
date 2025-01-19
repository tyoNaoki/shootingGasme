#include "PlayerGunComponent.h"
#include "CollisionManager.h"
#include "WeaponStatus.h"
#include "SoundManager.h"
#include "ActorManager.h"

PlayerGunComponent::PlayerGunComponent(std::shared_ptr<CharacterBase> owner):Component(owner)
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
	auto owner = GetOwner();

	if(!IsEnable()||!owner->IsActive() ||owner->IsDead()){return;}
	//�N�[���^�C�����I����Ă��邩
	if(mCurrentTime > 0){
		mCurrentTime -=deltaTime;
		return;
	}

	//���͂ɓG�����邩�ǂ���
	std::vector<std::weak_ptr<CharacterBase>>charas{ owner };
	auto nearEnemies = COLLISION_M.GetNearCharacters(owner->GetWorldPosition2D(),mLockOnRange, charas);
	if(nearEnemies.size()==0&&!ACTOR_M.IsSpawnBoss()){
		return;
	}

	float minDistance = -1.0f;
	std::shared_ptr<CharacterBase>target;
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
	if(ACTOR_M.IsSpawnBoss()){
		auto boss = ACTOR_M.GetCurrentBossEnemy();
		//�{�X�Ƃ̋����擾
		float distance = (boss->GetWorldPosition2D() - owner->GetWorldPosition2D()).Length_Square();
		//�{�X����ԋ߂��ꍇ
		if(minDistance > distance || minDistance == -1.0f){
			target = boss;
		}
	}

	if(!target){return;}
	//�����Ƒ_���Ώۂ܂ł̊p�x���擾
	float targetRot = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), target->GetWorldPosition2D());
	//�e�X�|�[��
	auto bullet = ACTOR_M.GetBullet(mBulletName,owner->GetActorType(),owner->GetLocalPosition2D(),owner->GetWorldPosition2D(),targetRot,true);
	//�e���A�N�e�B�u�ɂ���
	if(!ACTOR_M.RegistObject(bullet)){
		DEBUG_HELPER.Add("Bullet is generate error",1.0f);
		return;
	}
	//�X�e�[�^�X�ݒ�
	bullet->SetStatus(mBulletSpeed, mAttack,mIsPenetration,mPenetrationMaxCount);
	//����炷
	SOUND_M.PlaySoundByID(mFireSound);
	//�N�[���^�C���ݒ�
	mCurrentTime = mShotRate;
}

void PlayerGunComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//�X�e�[�^�X�ݒ�
	auto gunStatus = std::dynamic_pointer_cast<GunWeaponStatus>(status);
	mAttack = gunStatus->mAttack;
	mShotRate = gunStatus->mShotRate;
	mBulletSpeed = gunStatus->mBulletSpeed;
	mLockOnRange = gunStatus->mLockOnRange;
	mIsPenetration = gunStatus->mIsPenetration;
	mPenetrationMaxCount = gunStatus->mPenetrationMaxCount;
}
