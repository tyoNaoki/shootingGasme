#pragma once
#include "LevelUpBonus.h"
class LevelUpThrowBombRate :
    public LevelUpBonus
{
public:
    //”š’e‚Ì“Š‚°‚éŠÔŠu‚Ì•ñVŒø‰Ê‰Šú‰»
    LevelUpThrowBombRate(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //•ñV‚ÌŒø‰Ê‚ğ•Ší‚É•t—^
    void Apply() const override;

private:
    //‹­‰»’l
    float mLevelUpValue;
};

