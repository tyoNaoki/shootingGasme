#include "LevelUpGetReflectionGun.h"
#include "StatusManager.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"

LevelUpGetReflectionGun::LevelUpGetReflectionGun(std::string tag,RewardType type, std::string name, std::string description):LevelUpBonus(tag,type,name,description)
{
}

void LevelUpGetReflectionGun::Apply() const
{
	//���˕�����g�p�\�ɂ���
	if (auto player = ACTOR_M.GetCurrentPlayer()) {
		player->EnablePlayerComponent(GetTag());
	}

	//����X�e�[�^�X�X�V
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
