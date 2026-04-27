#pragma once
#include "LevelUpBonus.h"
class LevelUpGunBulletSpeed :
    public LevelUpBonus
{
public:
    //e‚Ì’e‘¬‚Ì‹­‰»Aà–¾‚ğİ’è
    LevelUpGunBulletSpeed(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //‹­‰»Œø‰Ê‚ğ•t—^
    void Apply() const override;

private:
    //‹­‰»’l
    float mLevelUpValue;
};

