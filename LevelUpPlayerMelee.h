#pragma once
#include "LevelUpBonus.h"
class LevelUpPlayerMelee :
    public LevelUpBonus
{
public:
    //‹ßÚUŒ‚‚ÌUŒ‚—Í‹­‰»’lAà–¾‚ğİ’è
    LevelUpPlayerMelee(std::string tag,RewardType type, float plusValue, std::string name, std::string description);
    //‹­‰»Œø‰Ê‚ğ•t—^‚·‚é
    void Apply() const override;

private:
    //‹­‰»’l
    float mPlusValue;
};

