#include "LevelUpReflectionBulletSpeedUp.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpReflectionBulletSpeedUp::LevelUpReflectionBulletSpeedUp(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpReflectionBulletSpeedUp::Apply() const
{
	auto status = std::dynamic_pointer_cast<GunWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));
	//���틭��
	status->mBulletSpeed += mLevelUpValue;

	//�ō��l�ɂȂ����ꍇ�A�I���V���X�g���珜��
	if (status->mBulletSpeed >= status->mMaxBulletSpeed) {
		STATUS.RemoveReward(GetName());
	}
	//����X�e�[�^�X�X�V
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
