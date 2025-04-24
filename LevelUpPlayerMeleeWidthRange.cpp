#include "LevelUpPlayerMeleeWidthRange.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpPlayerMeleeWidthRange::LevelUpPlayerMeleeWidthRange(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpPlayerMeleeWidthRange::Apply() const
{
	auto status = std::dynamic_pointer_cast<MeleeWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));
	//武器強化
	status->mWidthRange = mLevelUpValue;
	//報酬選択から除く
	STATUS.RemoveReward(GetName());
	//武器ステータス更新
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
