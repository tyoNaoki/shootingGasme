#include "LevelUpBombExplosionRange.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpBombExplosionRange::LevelUpBombExplosionRange(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpBombExplosionRange::Apply() const
{
	auto status = std::dynamic_pointer_cast<BombWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));

	//武器強化
	status->mExplosionRange += mLevelUpValue;
	//レベルアップ時の報酬選択から除く
	STATUS.RemoveReward(GetName());

	//武器ステータス更新
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
