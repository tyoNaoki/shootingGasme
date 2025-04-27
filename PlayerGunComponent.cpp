#include "PlayerGunComponent.h"
#include "CollisionManager.h"
#include "WeaponStatus.h"
#include "SoundManager.h"
#include "ActorManager.h"

PlayerGunComponent::PlayerGunComponent(std::shared_ptr<CharacterBase> owner):WeaponComponent(owner)
{
	//初期化
	mComponentName = "PlayerGunComponent";
	Init();
}

void PlayerGunComponent::Init()
{
	//ステータス設定
	SetStatus(STATUS.GetCurrentWeaponStatus("PlayerGunComponent"));
}

void PlayerGunComponent::Update(const float deltaTime)
{
	auto owner = GetWeaponOwner();

	if(!IsEnable()||!owner->IsActive() ||owner->IsDead()){return;}

	//クールタイム
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
	//ステータス設定
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
	//射撃クールタイムあり
	if(mCurrentTime > 0)
	{
		return false;
	}

	//周囲に敵がいるかどうか
	std::vector<std::weak_ptr<CharacterBase>>charas{ owner };
	auto nearEnemies = COLLISION_M.GetNearCharacters(owner->GetWorldPosition2D(), mLockOnRange, charas);
	if (nearEnemies.size() == 0 && !ACTOR_M.IsSpawnBoss()) {
		return false;
	}

	//一番近い敵をセット
	target = FindNearEnemy(owner,nearEnemies);
	return true;
}

void PlayerGunComponent::Shoot(const std::shared_ptr<CharacterBase>owner,const std::shared_ptr<CharacterBase>target)
{
	if (!owner||!target) { return; }

	//自分と狙う対象までの角度を取得
	float targetRot = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), target->GetWorldPosition2D());
	//弾スポーン
	auto bullet = ACTOR_M.GetBullet(mBulletName.c_str(), owner->GetActorType(), owner->GetLocalPosition2D(), owner->GetWorldPosition2D(), targetRot, true);
	//弾をアクティブにする
	if (!bullet) {
		DEBUG_HELPER.Add("Bullet is generate error", 1.0f);
		return;
	}
	//ステータス設定
	bullet->SetStatus(mBulletSpeed, mAttack, mIsPenetration, mPenetrationMaxCount);
	//音を鳴らす
	SOUND_M.PlaySoundByID(mFireSound);
	//クールタイム設定
	mCurrentTime = mShotRate;
}

std::shared_ptr<CharacterBase> PlayerGunComponent::FindNearEnemy(const std::shared_ptr<CharacterBase>owner,const std::vector<std::shared_ptr<CharacterBase>>nearEnemies)
{
	std::shared_ptr<CharacterBase>target;
	float minDistance = -1.0f;
	//周囲の敵との距離
	for (auto& x : nearEnemies) {
		float distance = (x->GetWorldPosition2D() - owner->GetWorldPosition2D()).Length_Square();
		//一番近い敵を対象に決定
		if (minDistance > distance || minDistance == -1.0f) {
			minDistance = distance;
			target = x;
		}
	}

	//ボスがスポーンしているか
	if (ACTOR_M.IsSpawnBoss()) {
		auto boss = ACTOR_M.GetCurrentBossEnemy();
		//ボスとの距離取得
		float distance = (float)(boss->GetWorldPosition2D() - owner->GetWorldPosition2D()).Length_Square();
		//ボスが一番近い場合
		if (minDistance > distance || minDistance == -1.0f) {
			target = boss;
		}
	}

	return target;
}
