#pragma once
#include "LevelUpBonus.h"
class LevelUpThrowBombRate :
    public LevelUpBonus
{
public:
    //���e�̓�����Ԋu�̕�V���ʏ�����
    LevelUpThrowBombRate(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //��V�̌��ʂ𕐊�ɕt�^
    void Apply() const override;

private:
    //�����l
    float mLevelUpValue;
};

