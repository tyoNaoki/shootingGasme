#include "NormalEnemyGunComponent.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"

NormalEnemyGunComponent::NormalEnemyGunComponent(std::shared_ptr<CharacterBase> owner) :WeaponComponent(owner)
{
	//初期化
	mComponentName = "NormalEnemyGunComponent";
	Init();
}

void NormalEnemyGunComponent::Init()
{
	//ステータス設定
	SetStatus(STATUS.GetCurrentWeaponStatus(mComponentName));
	mLockOnRange *= mLockOnRange;
}

void NormalEnemyGunComponent::Update(const float deltaTime)
{
	auto owner = GetWeaponOwner();
	auto player = ACTOR_M.GetCurrentPlayer();

	if (!owner->IsActive()||!player ||!player->IsActive()) { return; }
	//クールタイムがおわっているか
	if (mCurrentTime > 0) {
		mCurrentTime -= deltaTime;
		return;
	}
	//プレイヤーまでの距離取得
	auto length = (player->GetWorldPosition2D()-owner->GetWorldPosition2D()).Length_Square();
	//射程距離外かどうか
	if(length > mLockOnRange){return;}
	//プレイヤーまでの角度取得
	float targetRot = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), player->GetWorldPosition2D());

	//弾を発射
	FireBullet(owner,targetRot);

	//バースト射撃専用処理
	if(mBurstCount+1<mMaxBurstCount){
		mBurstCount++;
		mCurrentTime = mBurstRate; //バースト間の待機時間
	}else{
		mCurrentTime = mShotRate + distribution(generator); // バースト終了後の待機時間+乱数で少し散らす
		mBurstCount = 0; // バーストカウントをリセット
	}
}

void NormalEnemyGunComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//ステータス設定
	auto gunStatus = std::dynamic_pointer_cast<GunWeaponStatus>(status);
	mAttack = gunStatus->mAttack;
	mShotRate = gunStatus->mShotRate.GetValue();
	mBulletSpeed = gunStatus->mBulletSpeed;
	mLockOnRange = gunStatus->mLockOnRange;
}

void NormalEnemyGunComponent::FireBullet(std::shared_ptr<Actor> owner, float targetRot)
{
	//弾スポーン
	auto bullet = ACTOR_M.GetBullet(mBulletName.c_str(), owner->GetActorType(), owner->GetLocalPosition2D(), owner->GetWorldPosition2D(), targetRot, true);
	//弾をアクティブに
	if (!bullet) {
		DEBUG_HELPER.Add("EnemyBullet is generate error", 1.0f);
		return;
	}
	//弾のステータス設定
	bullet->SetStatus(mBulletSpeed, mAttack, false, 0);
}
