#include "LevelUpHP.h"
#include "StatusManager.h"

LevelUpHP::LevelUpHP(RewardType type, float plusValue, std::string name, std::string description):LevelUpBonus("None", type, name, description), mPlusValue(plusValue)
{
}

void LevelUpHP::Apply() const
{
	//HPëùâ¡
	auto status = STATUS.GetCurrentPlayerStatus();
	status.hp += mPlusValue;
	STATUS.UpdatePlayerStatus(status);
}

