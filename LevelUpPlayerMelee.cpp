#include "LevelUpPlayerMelee.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpPlayerMelee::LevelUpPlayerMelee(std::string tag,RewardType type, float plusValue, std::string name, std::string description):LevelUpBonus(tag,type,name,description),mPlusValue(plusValue)
{
}

void LevelUpPlayerMelee::Apply() const
{
	auto status = STATUS.GetCurrentWeaponStatus("PlayerMeleeComponent");
	//•ŠíUŒ‚—Í‹­‰»
	status->mAttack += mPlusValue;
	STATUS.UpdateWeaponStatus("PlayerMeleeComponent",status);
	ACTOR_M.PlayerWeaponLevelUp("PlayerMeleeComponent");
}
