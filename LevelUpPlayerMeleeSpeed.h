#pragma once
#include "LevelUpBonus.h"
class LevelUpPlayerMeleeSpeed :
    public LevelUpBonus
{
public:
    //�ߐڍU�����x�̋����A������ݒ�
    LevelUpPlayerMeleeSpeed(std::string tag,RewardType type, float levelUpValue, std::string name, std::string description);
    //�������ʂ�t�^����
    void Apply() const override;

private:
    //�����l
    float mLevelUpValue;
};

