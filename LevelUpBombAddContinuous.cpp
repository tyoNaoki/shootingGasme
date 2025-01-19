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

	//�������������ă_���[�W��^����t���O�����Ă�
	status->mHasContinuousDamage = true;
	//��x�̔����̈З͕ύX
	status->mAttack = status->mAttack/(status->mDefaultAttack/newAttack);
	//�m�b�N�o�b�N��0�ɕύX
	status->mKnockBackStrength = 0.0f;
	//�������Ԃ�傫���l�ɐݒ�
	status->mExplosionTime = newExplosionTime;

	//���x���A�b�v���̕�V�I�����珜��
	STATUS.RemoveReward(GetName());

	//����X�e�[�^�X�X�V
	STATUS.UpdateWeaponStatus(GetTag(), status);
	ACTOR_M.PlayerWeaponLevelUp(GetTag());
}
