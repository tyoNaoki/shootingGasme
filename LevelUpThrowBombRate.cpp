#include "LevelUpThrowBombRate.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpThrowBombRate::LevelUpThrowBombRate(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpThrowBombRate::Apply() const
{
	auto status = std::dynamic_pointer_cast<BombWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));
	
	if (!status) { return; }
	//ボムの投げる間隔を短くする
	//武器強化
	status->mThrowRate.AddValue(-mLevelUpValue);

	//最低値になった場合、選択報酬リストから除く
	if (status->mThrowRate.CanPowerUp(status->mThrowRate.GetValue() - mLevelUpValue)) {
		STATUS.RemoveReward(GetName());
	}
	//武器ステータス更新
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
