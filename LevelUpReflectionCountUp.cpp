#include "LevelUpReflectionCountUp.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpReflectionCountUp::LevelUpReflectionCountUp(std::string tag, RewardType type, int levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpReflectionCountUp::Apply() const
{
	auto status = std::dynamic_pointer_cast<GunWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));

	//武器強化
	status->mReflectionMaxCount += mLevelUpValue;
	//反射強化回数が最大値に達した場合、報酬選択から除く
	if (status->mReflectionMaxCount >= status->mMaxReflectionMaxCount) {
		STATUS.RemoveReward(GetName());
	}
	//武器ステータス更新
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
