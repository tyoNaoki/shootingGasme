#pragma once
#include "LevelUpBonus.h"
class LevelUpBombExplosionRange :
    public LevelUpBonus
{
public:
    //”š’e•Ší‚Ì”š”­”ÍˆÍ‚ğ‹­‰»Aà–¾‚ğİ’è
    LevelUpBombExplosionRange(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //‹­‰»Œø‰Ê‚ğ•t—^‚·‚é
    void Apply() const override;

private:
    //‹­‰»’l
    float mLevelUpValue;
};

