#pragma once
#include "LevelUpBonus.h"
class LevelUpReflectionShotRateUp :
    public LevelUpBonus
{
public:
    LevelUpReflectionShotRateUp(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //•ñV‚ÌŒø‰Ê‚ğ•Ší‚É•t—^
    void Apply() const override;

private:
    //‹­‰»’l
    float mLevelUpValue;
};

