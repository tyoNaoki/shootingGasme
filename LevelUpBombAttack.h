#pragma once
#include "LevelUpBonus.h"
class LevelUpBombAttack :
    public LevelUpBonus
{
public:
    //���e�̔����_���[�W�����l�A��V�̊�{���Ȃǂ�ݒ�
    LevelUpBombAttack(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //�������ʂ�t�^����
    void Apply() const override;

private:
    //�����l
    float mLevelUpValue;
};

