#include "LevelUpBonus.h"
#include "StatusManager.h"
#include "ActorManager.h"

LevelUpBonus::LevelUpBonus(std::string tag, RewardType type, std::string name, std::string description):mTag(tag),mType(type),mName(name),mDescription(description)
{
}

std::string LevelUpBonus::GetTag() const
{
	return mTag;
}

RewardType LevelUpBonus::GetType() const
{
	return mType;
}

std::string LevelUpBonus::GetName() const
{
	return mName;
}

std::string LevelUpBonus::GetDescription() const
{
	return mDescription;
}

void LevelUpBonus::UpdateCurrentPlayer()
{
	//���x���X�V����
    auto status = STATUS.GetCurrentPlayerStatus();
    status.level ++;
	//�v���C���[�̃X�e�[�^�X�X�V
    STATUS.UpdatePlayerStatus(status);
	//���x���X�V
	ACTOR_M.PlayerLevelUp();
}

