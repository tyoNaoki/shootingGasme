#pragma once
#include "LevelUpBonus.h"
class LevelUpPlayerMeleeWidthRange :
    public LevelUpBonus
{
public:
    //�ߐڍU���͈̔͋����A������ݒ�
    LevelUpPlayerMeleeWidthRange(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //�������ʂ�t�^
    void Apply() const override;

private:
    //�����l
    float mLevelUpValue;
};

