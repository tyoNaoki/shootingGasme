#include "LevelUpBombAttack.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpBombAttack::LevelUpBombAttack(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpBombAttack::Apply() const
{
	auto status = std::dynamic_pointer_cast<BombWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));

	//���틭��
	status->mAttack *= 1.0+(mLevelUpValue/100);
	//�X�e�[�^�X�X�V
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
