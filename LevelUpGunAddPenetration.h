#pragma once
#include "LevelUpBonus.h"
class LevelUpGunAddPenetration :
    public LevelUpBonus
{
public:
    //�e�̊ђʉ񐔂̋����l�A������ݒ�
    LevelUpGunAddPenetration(std::string tag, RewardType type, int levelUpValue, std::string name, std::string description);
    //�������ʂ�t�^����
    void Apply() const override;

private:
    //�����l
    int mLevelUpValue;
};

