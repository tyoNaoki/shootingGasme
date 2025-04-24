#include "BossLaserComponent.h"
#include "BossBase.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"
#include "StatusManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"

<<<<<<< HEAD
BossLaserComponent::BossLaserComponent(std::shared_ptr<CharacterBase> owner) : WeaponComponent(owner)
=======
BossLaserComponent::BossLaserComponent(std::shared_ptr<CharacterBase> owner):Component(owner)
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
{
	mComponentName = "BossLaserComponent";
	mBossOwner = std::dynamic_pointer_cast<BossBase>(owner);
	mPlayer = ACTOR_M.GetCurrentPlayer();
	SetStatus(STATUS.GetCurrentWeaponStatus(mComponentName));
	mCurState = BossLaserState::SetUp;
}

void BossLaserComponent::Update(const float deltaTime)
{
	auto owner = mBossOwner.lock();

	if (!owner || !owner->IsActive() || owner->IsDead() ||
		!mPlayer || !mPlayer->IsActive() || mPlayer->IsDead()) {
		return;
	}

	//�N�[���^�C���I����Ă��Ȃ��Ȃ�
	if(mCurrentTime>0){
		mCurrentTime-=deltaTime;

		//�A���[�g��ԂȂ�A�A���[�g����炷�B���[�U�[�Ȃ烌�[�U�[���ˉ�
		if(IsPlaySound(deltaTime)){
			CurrentPlaySound();
		}
		//�A���[�g���
		if(mCurState == BossLaserState::Alert){
			auto lookRot = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), mPlayer->GetWorldPosition2D());
			owner->SetRotation(lookRot);
			//�A���[�g�̎��A���݂̉摜���������_�ł�����
			if(IsPlayAlertBlend(deltaTime)){
				owner->SetAlert(true);
			}else{
				owner->SetAlert(false);
			}
		}
		return;
	}

	//�N�[���^�C�����I����Ă���
	float targetRot = 0.0f;
	switch (mCurState)
	{
	//���ˑO�̌x������
	case BossLaserState::SetUp:
		mCurrentTime = mLaserAlerTime;
		//�x����ԂɑJ��
		mCurState = BossLaserState::Alert;
		StartAlertSound();
		break;
	//�x����Ԃ̎�
	case BossLaserState::Alert:
		//�A���[�g��؂�
		owner->SetAlert(false);
		//��������v���C���[�܂ł̊p�x�擾
		targetRot = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), mPlayer->GetWorldPosition2D());
		//���[�U�[����
		FireLaser(GetOwner(), targetRot);
		//�N�[���^�C���ݒ�
		mCurrentTime = mFireTime;
		//���[�U�[���ˏ�ԂɑJ��
		mCurState = BossLaserState::FireLaser;
		StartLaserSound();
		
		break;
	//���[�U�[���ˏ��
	case BossLaserState::FireLaser:
		//���[�U�[���ˏI��
		FinishLaser();
		mCurrentTime = 0.0f;
		//���ˑO�̏�ԂɑJ��
		mCurState = BossLaserState::SetUp;

		//���[�U�[���˂��I���������Ƃ��I�[�i�[�ɒʒm
		owner->FinishState();
		//�����~�߂�
		CurrentStopSound();
		break;
	default:
		break;
	}
}

void BossLaserComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//�X�e�[�^�X�ݒ�
	auto laserStatus = std::dynamic_pointer_cast<LaserWeaponStatus>(status);
	mLaserDamage = laserStatus->mAttack;
	mLaserSpeed = laserStatus->mSpeed;
	mLaserWidthSize = laserStatus->mWidthSize;
	mLaserSpreadAngle = laserStatus->mSpreadAngle;
	mLaserCount = laserStatus->mLaserCount;
	mLaserKnockBack = laserStatus->mKnockBack;
	mLaserKnockBackTime = laserStatus->mKnockBackTime;
	mFireTime = laserStatus->mFireTime;
}

bool BossLaserComponent::IsPlaySound(float deltaTime)
{
	//�C���^�[�o�������Ă��邩
	mSoundTime += deltaTime;
	if(mSoundTime >= mSoundInterval){
		mSoundTime -= mSoundInterval;
		return true;
	}
	return false;
}

bool BossLaserComponent::IsPlayAlertBlend(float deltaTime)
{
	//�A���[�g�̃C���^�[�o���߂��Ă��邩
	mAlertBlendTime += deltaTime;
	if (mAlertBlendTime >= mAlertBlendStartInterval) {
		
		if(mAlertBlendTime >= mAlertBlendFinishInterval){
			mAlertBlendTime -= mAlertBlendFinishInterval;
			return false;
		}
		return true;
	}
	return false;
}

void BossLaserComponent::CurrentPlaySound()
{
	if(mPlaySound != "None"){
		SOUND_M.PlaySoundByID(mPlaySound);
	}
}

void BossLaserComponent::CurrentStopSound()
{
	mPlaySound = "None";
	mSoundTime = 0.0f;
	mSoundInterval = 0.0f;
}

void BossLaserComponent::StartAlertSound()
{
	mPlaySound = "laserAlertSound";
	mSoundTime = 0.0f;
	mSoundInterval = 0.5f;
}

void BossLaserComponent::StartLaserSound()
{
	mPlaySound = "laserFireSound";
	mSoundTime = 0.0f;
	mSoundInterval = 0.1f;
}

void BossLaserComponent::FireLaser(std::shared_ptr<CharacterBase> owner, float targetRot)
{
	lasers.clear();
	lasers.resize(mLaserCount);
	//�e���[�U�[���Ɍv�Z
	for(int i=0;i<mLaserCount;i++){
		float laserAngle = targetRot;
		if(i%2 == 0){
			//0,2,4 ��0,1,2
			if(i!=0){
				laserAngle += (int)(i/2) * mLaserSpreadAngle;
			}
		}else{
			//1,3 ��1,2
			laserAngle -= (int)(i/2+1) * mLaserSpreadAngle;
		}
		//���[�U�[���W�v�Z
		auto laserLoc = owner->GetLocalPosition2D() + mLaserPositionOffset;
		//���[�U�[�X�|�[��
		auto laser = ACTOR_M.GetLaser("Laser", owner, owner->GetLocalPosition2D() + mLaserPositionOffset, owner->GetWorldPosition2D() + mLaserPositionOffset, laserAngle, true);
		//���[�U�[�̃X�e�[�^�X�ݒ�
		laser->SetStatus(mLaserWidthSize, mLaserSpeed,mLaserDamage,mLaserKnockBack,mLaserKnockBackTime);
		//���[�U�[���A�N�e�B�u�ɂ���
<<<<<<< HEAD
		if (!laser) {
=======
		if (!ACTOR_M.RegistObject(laser)) {
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
			DEBUG_HELPER.Add("EnemyLaser is generate error", 1.0f);
			return;
		}

		lasers[i] = laser;
	}
}

void BossLaserComponent::FinishLaser()
{
	//�S�A�N�e�B�u���[�U�[���A�N�e�B�u�ɂ���
	for(auto&x:lasers){
		x->SetActive(false);
	}
	lasers.clear();
}
