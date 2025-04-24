#pragma once
#include "LevelUpBonus.h"

class LevelUpGunShotRate :
    public LevelUpBonus
{
public:
    //�e�̔��ˑ��x�����̒l�A������ݒ�
    LevelUpGunShotRate(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //�������ʂ�t�^
    void Apply() const override;

private:
    //�����l
    float mLevelUpValue;
};

