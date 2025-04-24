#include "LevelUpBombAddContinuous.h"
#include "WeaponStatus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpBombAddContinuous::LevelUpBombAddContinuous(std::string tag, RewardType type, float changeAttackValue, float explosionTime, std::string name, std::string description):LevelUpBonus(tag,type,name,description),newAttack(changeAttackValue),newExplosionTime(explosionTime)
{
}

void LevelUpBombAddContinuous::Apply() const
{
	auto status = std::dynamic_pointer_cast<BombWeaponStatus>(STATUS.GetCurrentWeaponStatus(GetTag()));

	//爆発を持続してダメージを与えるフラグをたてる
	status->mHasContinuousDamage = true;
	//一度の爆発の威力変更
	status->mAttack = status->mAttack/(status->mDefaultAttack/newAttack);
	//ノックバックを0に変更
	status->mKnockBackStrength = 0.0f;
	//爆発時間を大きい値に設定
	status->mExplosionTime = newExplosionTime;

	//レベルアップ時の報酬選択から除く
	STATUS.RemoveReward(GetName());

	//武器ステータス更新
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
