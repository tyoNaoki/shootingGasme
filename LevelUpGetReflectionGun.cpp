#include "LevelUpGetReflectionGun.h"
#include "StatusManager.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"

LevelUpGetReflectionGun::LevelUpGetReflectionGun(std::string tag,RewardType type, std::string name, std::string description):LevelUpBonus(tag,type,name,description)
{
}

void LevelUpGetReflectionGun::Apply() const
{
	//反射武器を使用可能にする
	if (auto player = ACTOR_M.GetCurrentPlayer()) {
		player->EnablePlayerComponent(GetTag());
	}

	//武器ステータス更新
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
