#include "LevelUpReflectionCountUp.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpReflectionCountUp::LevelUpReflectionCountUp(std::string tag, RewardType type, int levelUpValue, std::string name, std::string description) :LevelUpBonus(tag, type, name, description), mLevelUpValue(levelUpValue)
{
}

void LevelUpReflectionCountUp::Apply() const
{
	auto status = std::dynamic_pointer_cast<GunWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));

	//���틭��
	status->mReflectionMaxCount += mLevelUpValue;
	//���ˋ����񐔂��ő�l�ɒB�����ꍇ�A��V�I�����珜��
	if (status->mReflectionMaxCount >= status->mMaxReflectionMaxCount) {
		STATUS.RemoveReward(GetName());
	}
	//����X�e�[�^�X�X�V
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
