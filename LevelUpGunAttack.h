#pragma once
#include "LevelUpBonus.h"
class LevelUpGunAttack :
    public LevelUpBonus
{
public:
    //�e�̍U���͂̋����A������ݒ�
    LevelUpGunAttack(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //�������ʂ�t�^����
    void Apply() const override;

private:
    //�����l
    float mLevelUpValue;
};

