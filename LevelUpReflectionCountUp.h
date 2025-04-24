#pragma once
#include "LevelUpBonus.h"
class LevelUpReflectionCountUp :
    public LevelUpBonus
{
public:
    //反射銃の反射回数の強化値を初期化
    LevelUpReflectionCountUp(std::string tag, RewardType type, int levelUpValue, std::string name, std::string description);
    //強化効果をプレイヤーに付与
    void Apply() const override;

private:
    //強化値
    int mLevelUpValue;
};

