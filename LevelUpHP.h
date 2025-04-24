#pragma once
#include "LevelUpBonus.h"
class LevelUpHP :
    public LevelUpBonus
{
public:
    //HP‚Ì‘‰Á—ÊAà–¾‚ğİ’è
    LevelUpHP(RewardType type,float plusValue,std::string name,std::string description);
    //•ñVŒø‰Ê‚ğ•t—^
    void Apply() const override;

private:
    //HP‚Ì‘‰Á—Ê
    float mPlusValue;
};

