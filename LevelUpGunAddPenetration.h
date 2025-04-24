#pragma once
#include "LevelUpBonus.h"
class LevelUpGunAddPenetration :
    public LevelUpBonus
{
public:
    //銃の貫通回数の強化値、説明を設定
    LevelUpGunAddPenetration(std::string tag, RewardType type, int levelUpValue, std::string name, std::string description);
    //強化効果を付与する
    void Apply() const override;

private:
    //強化値
    int mLevelUpValue;
};

