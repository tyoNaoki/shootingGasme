#include "LevelUpGunAddPenetration.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpGunAddPenetration::LevelUpGunAddPenetration(std::string tag, RewardType type, int levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpGunAddPenetration::Apply() const
{
	auto status = std::dynamic_pointer_cast<GunWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));

	//���틭��
	//�ő�ђʉ񐔑���
	status->mIsPenetration = true;
	status->mPenetrationMaxCount += mLevelUpValue;
	//�ђʉ񐔋����l���ő�ɒB������A��V�I�����珜��
	if(status->mPenetrationMaxCount >=status->mMaxPenetrationMaxCount){
		STATUS.RemoveReward(GetName());
	}
	//����X�e�[�^�X�X�V
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
