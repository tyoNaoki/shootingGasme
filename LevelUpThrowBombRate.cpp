#include "LevelUpThrowBombRate.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpThrowBombRate::LevelUpThrowBombRate(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpThrowBombRate::Apply() const
{
	auto status = std::dynamic_pointer_cast<BombWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));
	
	if (!status) { return; }
	//�{���̓�����Ԋu��Z������
	//���틭��
	status->mThrowRate.AddValue(-mLevelUpValue);

	//�Œ�l�ɂȂ����ꍇ�A�I���V���X�g���珜��
	if (status->mThrowRate.CanPowerUp(status->mThrowRate.GetValue() - mLevelUpValue)) {
		STATUS.RemoveReward(GetName());
	}
	//����X�e�[�^�X�X�V
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
