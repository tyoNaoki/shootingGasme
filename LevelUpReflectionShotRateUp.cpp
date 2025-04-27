#include "LevelUpReflectionShotRateUp.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpReflectionShotRateUp::LevelUpReflectionShotRateUp(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpReflectionShotRateUp::Apply() const
{
	auto status = std::dynamic_pointer_cast<GunWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));
	//���틭��
	status->mShotRate.AddValue(-mLevelUpValue);

	//�Œ�l�ɂȂ����ꍇ�A�I���V���X�g���珜��
	if(status->mShotRate.CanPowerUp(status->mShotRate.GetValue() - mLevelUpValue)){
		STATUS.RemoveReward(GetName()); 
	}
	//����X�e�[�^�X�X�V
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
