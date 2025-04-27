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
	//武器攻撃力強化
	if (!status) { return; }
	//発射までの間隔を短くする
	status->mShotRate.SetValue(status->mShotRate.GetValue() - mLevelUpValue);

	//最低値になった場合、選択報酬リストから除く
	if (!status->mShotRate.CanPowerUp(status->mShotRate.GetValue() - mLevelUpValue)) {
		STATUS.RemoveReward(GetName());
	}

	//武器ステータス更新
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
