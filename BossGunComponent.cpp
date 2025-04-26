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

	//自分から敵までの角度取得,設定
	float targetRot = (float)Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), player->GetWorldPosition2D());
	owner->SetRotation(targetRot);

	if (mCurrentTime > 0) {
		mCurrentTime -= deltaTime;
		return;
	}
	//まだ発射していないなら、弾を発射
	if (!mIsFire) {
		FireBullet(owner, player);
		//クールタイム設定
		mCurrentTime = mFireTime;
		//発射フラグをたてる
		mIsFire = true;
	}else{
		//非発射状態に設定
		mCurrentTime = 0.0f;
		mIsFire = false;

		//弾を発射し終えたことを通知
		owner->FinishState();
	}

	
}

void BossGunComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//銃のステータス設定
	auto gunStatus = std::dynamic_pointer_cast<GunWeaponStatus>(status);
	mAttack = gunStatus->mAttack;
	mSpeed = gunStatus->mBulletSpeed;
	mIsHoming = true;
	mHomingTime = gunStatus->mHomingTime;
	mFireTime = 3.0f;
}

void BossGunComponent::FireBullet(std::shared_ptr<Actor>owner, std::shared_ptr<CharacterBase> homingTarget)
{
	//自分から敵までの角度取得
	float targetRot = (float)Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), homingTarget->GetWorldPosition2D());
	owner->SetRotation(targetRot);

	//発射音を鳴らす
	SOUND_M.PlaySoundByID(soundName);

	//非アクティブの弾を取得、スポーン
	auto bullet = ACTOR_M.GetBullet("BossBullet", owner->GetActorType(), owner->GetLocalPosition2D(), owner->GetWorldPosition2D(), targetRot, true);
	//アクティブ状態に設定
	if (!bullet) {
		DEBUG_HELPER.Add("BossBullet is generate error", 1.0f);
	}
	//弾のステータス設定
	bullet->SetStatus(mSpeed, mAttack,mHomingTime,homingTarget);
}
