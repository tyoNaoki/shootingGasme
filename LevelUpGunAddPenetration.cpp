#include "LevelUpGunAddPenetration.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpGunAddPenetration::LevelUpGunAddPenetration(std::string tag, RewardType type, int levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpGunAddPenetration::Apply() const
{
	auto status = std::dynamic_pointer_cast<GunWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));

	//武器強化
	//最大貫通回数増加
	status->mIsPenetration = true;
	status->mPenetrationMaxCount += mLevelUpValue;
	//貫通回数強化値が最大に達したら、報酬選択から除く
	if(status->mPenetrationMaxCount >=status->mMaxPenetrationMaxCount){
		STATUS.RemoveReward(GetName());
	}
	//武器ステータス更新
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
