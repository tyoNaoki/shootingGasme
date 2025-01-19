#pragma once
#include "LevelUpBonus.h"
class LevelUpPlayerMeleeWidthRange :
    public LevelUpBonus
{
public:
    //‹ßÚUŒ‚‚Ì”ÍˆÍ‹­‰»Aà–¾‚ğİ’è
    LevelUpPlayerMeleeWidthRange(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //‹­‰»Œø‰Ê‚ğ•t—^
    void Apply() const override;

private:
    //‹­‰»’l
    float mLevelUpValue;
};

