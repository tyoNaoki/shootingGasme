#pragma once
#include "LevelUpBonus.h"
class LevelUpHP :
    public LevelUpBonus
{
public:
    //HPÌÁÊAà¾ðÝè
    LevelUpHP(RewardType type,float plusValue,std::string name,std::string description);
    //ñVøÊðt^
    void Apply() const override;

private:
    //HPÌÁÊ
    float mPlusValue;
};

