#include "LevelUpPlayerMeleeSpeed.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpPlayerMeleeSpeed::LevelUpPlayerMeleeSpeed(std::string tag,RewardType type, float levelUpValue, std::string name, std::string description):LevelUpBonus(tag,type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpPlayerMeleeSpeed::Apply() const
{
	auto status = std::dynamic_pointer_cast<MeleeWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));

	//近接の振る間隔を短くする
	status->mSwingSpeed.SetValue(status->mSwingSpeed.GetValue() - mLevelUpValue);

	//最低値になった場合、選択報酬リストから除く
	if (!status->mSwingSpeed.CanPowerUp(status->mSwingSpeed.GetValue() - mLevelUpValue)) {
		STATUS.RemoveReward(GetName());
	}

	//武器ステータス更新
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
