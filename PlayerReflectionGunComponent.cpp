#include "PlayerReflectionGunComponent.h"
#include "CollisionManager.h"
#include "StatusManager.h"
#include "ActorManager.h"

PlayerReflectionGunComponent::PlayerReflectionGunComponent(std::shared_ptr<CharacterBase> owner) :WeaponComponent(owner)
{
	//初期化
	mComponentName = "PlayerReflectionGunComponent";
	Init();
}

void PlayerReflectionGunComponent::Init()
{
	//ステータス設定
	SetStatus(STATUS.GetCurrentWeaponStatus(mComponentName));
}

void PlayerReflectionGunComponent::Update(const float deltaTime)
{
	auto owner = GetWeaponOwner();

	if (!IsEnable() || !owner->IsActive() || owner->IsDead()) { return; }

	//クールタイムが終わっているか
	if (mCurrentTime > 0) {
		mCurrentTime -= deltaTime;
		return;
	}

	//周囲の探索する範囲からオーナーを除く
	std::vector<std::weak_ptr<CharacterBase>>charas{ owner };
	//周囲に敵がいるかどうか
	auto nearEnemies = COLLISION_M.GetNearCharacters(owner->GetWorldPosition2D(),mLockOnRange, charas);
	if (nearEnemies.size() == 0 && !ACTOR_M.IsSpawnBoss()) {
		return;
	}

	float minDistance = -1.0f;
	std::shared_ptr<CharacterBase>target;
	//周囲の敵との距離計算
	for (auto& x : nearEnemies) {
		float distance = (x->GetWorldPosition2D() - owner->GetWorldPosition2D()).Length_Square();
		//一番近い敵を対象に設定
		if (minDistance > distance || minDistance == -1.0f) {
			minDistance = distance;
			target = x;
		}
	}
	//ボスがスポーンされているか
	if (ACTOR_M.IsSpawnBoss()) {
		auto boss = ACTOR_M.GetCurrentBossEnemy();
		//ボスとの距離計算
		float distance = (boss->GetWorldPosition2D() - owner->GetWorldPosition2D()).Length_Square();
		//ボスが一番近い場合
		if (minDistance > distance || minDistance == -1.0f) {
			target = boss;
		}
	}

	if (!target) { return; }
	//対象までの角度を発射角度として取得
	float targetRot = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), target->GetWorldPosition2D());
	//弾を発射
	//弾をアクティブにする
	auto bullet = ACTOR_M.GetBullet(mBulletName.c_str(), owner->GetActorType(), owner->GetLocalPosition2D(), owner->GetWorldPosition2D(), targetRot, true);

	if(!bullet) {
		DEBUG_HELPER.Add("Bullet is generate error", 1.0f);
		return;
	}
	
	//ステータス設定
	bullet->SetStatus(mBulletSpeed, mAttack, mBulletLifeTime, mReflectionCount);
	//音を鳴らす
	SOUND_M.PlaySoundByID(mFireSound);
	//クールタイム設定
	mCurrentTime = mShotRate;
}

void PlayerReflectionGunComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//ステータス設定
	auto gunStatus = std::dynamic_pointer_cast<GunWeaponStatus>(status);
	mAttack = gunStatus->mAttack;
	mShotRate = gunStatus->mShotRate;
	mReflectionCount = gunStatus->mReflectionMaxCount;
	mBulletLifeTime = 10.0f;
	mLockOnRange = gunStatus->mLockOnRange;
	mBulletSpeed = gunStatus->mBulletSpeed;
}
