#pragma once
#include "LevelUpBonus.h"
class LevelUpGunAttack :
    public LevelUpBonus
{
public:
    //e‚ÌUŒ‚—Í‚Ì‹­‰»Aà–¾‚ğİ’è
    LevelUpGunAttack(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //‹­‰»Œø‰Ê‚ğ•t—^‚·‚é
    void Apply() const override;

private:
    //‹­‰»’l
    float mLevelUpValue;
};

