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

	//クールタイム終わっていないなら
	if(mCurrentTime>0){
		mCurrentTime-=deltaTime;

		//アラート状態なら、アラート音を鳴らす。レーザーならレーザー発射音
		if(IsPlaySound(deltaTime)){
			CurrentPlaySound();
		}
		//アラート状態
		if(mCurState == BossLaserState::Alert){
			auto lookRot = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), mPlayer->GetWorldPosition2D());
			owner->SetRotation(lookRot);
			//アラートの時、現在の画像をすこし点滅させる
			if(IsPlayAlertBlend(deltaTime)){
				owner->SetAlert(true);
			}else{
				owner->SetAlert(false);
			}
		}
		return;
	}

	//クールタイムが終わっている
	float targetRot = 0.0f;
	switch (mCurState)
	{
	//発射前の警告準備
	case BossLaserState::SetUp:
		mCurrentTime = mLaserAlerTime;
		//警告状態に遷移
		mCurState = BossLaserState::Alert;
		StartAlertSound();
		break;
	//警告状態の時
	case BossLaserState::Alert:
		//アラートを切る
		owner->SetAlert(false);
		//自分からプレイヤーまでの角度取得
		targetRot = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), mPlayer->GetWorldPosition2D());
		//レーザー発射
		FireLaser(GetOwner(), targetRot);
		//クールタイム設定
		mCurrentTime = mFireTime;
		//レーザー発射状態に遷移
		mCurState = BossLaserState::FireLaser;
		StartLaserSound();
		
		break;
	//レーザー発射状態
	case BossLaserState::FireLaser:
		//レーザー発射終了
		FinishLaser();
		mCurrentTime = 0.0f;
		//発射前の状態に遷移
		mCurState = BossLaserState::SetUp;

		//レーザー発射が終了したことをオーナーに通知
		owner->FinishState();
		//音を止める
		CurrentStopSound();
		break;
	default:
		break;
	}
}

void BossLaserComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//ステータス設定
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
	//インターバルすぎているか
	mSoundTime += deltaTime;
	if(mSoundTime >= mSoundInterval){
		mSoundTime -= mSoundInterval;
		return true;
	}
	return false;
}

bool BossLaserComponent::IsPlayAlertBlend(float deltaTime)
{
	//アラートのインターバル過ぎているか
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
	//各レーザー毎に計算
	for(int i=0;i<mLaserCount;i++){
		float laserAngle = targetRot;
		if(i%2 == 0){
			//0,2,4 →0,1,2
			if(i!=0){
				laserAngle += (int)(i/2) * mLaserSpreadAngle;
			}
		}else{
			//1,3 →1,2
			laserAngle -= (int)(i/2+1) * mLaserSpreadAngle;
		}
		//レーザー座標計算
		auto laserLoc = owner->GetLocalPosition2D() + mLaserPositionOffset;
		//レーザースポーン
		auto laser = ACTOR_M.GetLaser("Laser", owner, owner->GetLocalPosition2D() + mLaserPositionOffset, owner->GetWorldPosition2D() + mLaserPositionOffset, laserAngle, true);
		//レーザーのステータス設定
		laser->SetStatus(mLaserWidthSize, mLaserSpeed,mLaserDamage,mLaserKnockBack,mLaserKnockBackTime);
		//レーザーをアクティブにする
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
	//全アクティブレーザーを非アクティブにする
	for(auto&x:lasers){
		x->SetActive(false);
	}
	lasers.clear();
}
