#include "NormalEnemyGunComponent.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"

NormalEnemyGunComponent::NormalEnemyGunComponent(std::shared_ptr<CharacterBase> owner) :WeaponComponent(owner)
{
	//������
	mComponentName = "NormalEnemyGunComponent";
	Init();
}

void NormalEnemyGunComponent::Init()
{
	//�X�e�[�^�X�ݒ�
	SetStatus(STATUS.GetCurrentWeaponStatus(mComponentName));
	mLockOnRange *= mLockOnRange;
}

void NormalEnemyGunComponent::Update(const float deltaTime)
{
	auto owner = GetWeaponOwner();
	auto player = ACTOR_M.GetCurrentPlayer();

	if (!owner->IsActive()||!player ||!player->IsActive()) { return; }
	//�N�[���^�C����������Ă��邩
	if (mCurrentTime > 0) {
		mCurrentTime -= deltaTime;
		return;
	}
	//�v���C���[�܂ł̋����擾
	auto length = (player->GetWorldPosition2D()-owner->GetWorldPosition2D()).Length_Square();
	//�˒������O���ǂ���
	if(length > mLockOnRange){return;}
	//�v���C���[�܂ł̊p�x�擾
	float targetRot = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), player->GetWorldPosition2D());

	//�e�𔭎�
	FireBullet(owner,targetRot);

	//�o�[�X�g�ˌ���p����
	if(mBurstCount+1<mMaxBurstCount){
		mBurstCount++;
		mCurrentTime = mBurstRate; //�o�[�X�g�Ԃ̑ҋ@����
	}else{
		mCurrentTime = mShotRate + distribution(generator); // �o�[�X�g�I����̑ҋ@����+�����ŏ����U�炷
		mBurstCount = 0; // �o�[�X�g�J�E���g�����Z�b�g
	}
}

void NormalEnemyGunComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//�X�e�[�^�X�ݒ�
	auto gunStatus = std::dynamic_pointer_cast<GunWeaponStatus>(status);
	mAttack = gunStatus->mAttack;
	mShotRate = gunStatus->mShotRate.GetValue();
	mBulletSpeed = gunStatus->mBulletSpeed;
	mLockOnRange = gunStatus->mLockOnRange;
}

void NormalEnemyGunComponent::FireBullet(std::shared_ptr<Actor> owner, float targetRot)
{
	//�e�X�|�[��
	auto bullet = ACTOR_M.GetBullet(mBulletName.c_str(), owner->GetActorType(), owner->GetLocalPosition2D(), owner->GetWorldPosition2D(), targetRot, true);
	//�e���A�N�e�B�u��
	if (!bullet) {
		DEBUG_HELPER.Add("EnemyBullet is generate error", 1.0f);
		return;
	}
	//�e�̃X�e�[�^�X�ݒ�
	bullet->SetStatus(mBulletSpeed, mAttack, false, 0);
}
