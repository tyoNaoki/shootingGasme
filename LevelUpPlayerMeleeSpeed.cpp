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

	//�ߐڂ̐U��Ԋu��Z������
	status->mSwingSpeed -= mLevelUpValue;

	//�Œ�l�ɂȂ����ꍇ�A�I���V���X�g���珜��
	if (status->mSwingSpeed <= status->mMinSwingSpeed) {
		STATUS.RemoveReward(GetName());
	}
	//����X�e�[�^�X�X�V
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
