#include "LevelUpGunShotRate.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpGunShotRate::LevelUpGunShotRate(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpGunShotRate::Apply() const
{
	auto status = std::dynamic_pointer_cast<GunWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));
	//����U���͋���
	if (!status) { return; }
	//���˂܂ł̊Ԋu��Z������
	status->mShotRate.SetValue(status->mShotRate.GetValue() - mLevelUpValue);

	//�Œ�l�ɂȂ����ꍇ�A�I���V���X�g���珜��
	if (!status->mShotRate.CanPowerUp(status->mShotRate.GetValue() - mLevelUpValue)) {
		STATUS.RemoveReward(GetName());
	}

	//����X�e�[�^�X�X�V
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
