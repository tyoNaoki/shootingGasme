#pragma once
#include "LevelUpBonus.h"

class LevelUpGunShotRate :
    public LevelUpBonus
{
public:
    //銃の発射速度強化の値、説明を設定
    LevelUpGunShotRate(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //強化効果を付与
    void Apply() const override;

private:
    //強化値
    float mLevelUpValue;
};

