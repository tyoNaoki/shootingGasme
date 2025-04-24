#include "LevelUpReflectionBulletSpeedUp.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpReflectionBulletSpeedUp::LevelUpReflectionBulletSpeedUp(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpReflectionBulletSpeedUp::Apply() const
{
	auto status = std::dynamic_pointer_cast<GunWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));
	//武器強化
	status->mBulletSpeed += mLevelUpValue;

	//最高値になった場合、選択報酬リストから除く
	if (status->mBulletSpeed >= status->mMaxBulletSpeed) {
		STATUS.RemoveReward(GetName());
	}
	//武器ステータス更新
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
