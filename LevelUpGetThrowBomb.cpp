#include "LevelUpGetThrowBomb.h"
#include "StatusManager.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"

LevelUpGetThrowBomb::LevelUpGetThrowBomb(std::string tag, RewardType type, std::string name, std::string description):LevelUpBonus(tag, type, name, description)
{
}

void LevelUpGetThrowBomb::Apply() const
{
	//���e����g�p�\�ɂ���
	if (auto player = ACTOR_M.GetCurrentPlayer()) {
		player->EnablePlayerComponent(GetTag());
	}
	//����X�e�[�^�X�X�V
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
