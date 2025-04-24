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

	//���틭��
	status->mExplosionRange += mLevelUpValue;
	//���x���A�b�v���̕�V�I�����珜��
	STATUS.RemoveReward(GetName());

	//����X�e�[�^�X�X�V
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
