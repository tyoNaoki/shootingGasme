#include "LevelUpGetThrowBomb.h"
#include "StatusManager.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"

LevelUpGetThrowBomb::LevelUpGetThrowBomb(std::string tag, RewardType type, std::string name, std::string description):LevelUpBonus(tag, type, name, description)
{
}

void LevelUpGetThrowBomb::Apply() const
{
	//爆弾武器使用可能にする
	if (auto player = ACTOR_M.GetCurrentPlayer()) {
		player->EnablePlayerComponent(GetTag());
	}
	//武器ステータス更新
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
