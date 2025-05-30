#pragma once
#include "LevelUpBonus.h"
class LevelUpGunBulletSpeed :
    public LevelUpBonus
{
public:
    //銃の弾速の強化、説明を設定
    LevelUpGunBulletSpeed(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //強化効果を付与
    void Apply() const override;

private:
    //強化値
    float mLevelUpValue;
};

