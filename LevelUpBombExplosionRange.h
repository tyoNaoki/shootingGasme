#pragma once
#include "LevelUpBonus.h"
class LevelUpBombExplosionRange :
    public LevelUpBonus
{
public:
    //���e����̔����͈͂������A������ݒ�
    LevelUpBombExplosionRange(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //�������ʂ�t�^����
    void Apply() const override;

private:
    //�����l
    float mLevelUpValue;
};

