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
	//���틭��
	status->mWidthRange = mLevelUpValue;
	//��V�I�����珜��
	STATUS.RemoveReward(GetName());
	//����X�e�[�^�X�X�V
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
