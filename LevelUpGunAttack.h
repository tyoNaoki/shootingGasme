#pragma once
#include "LevelUpBonus.h"
class LevelUpGunAttack :
    public LevelUpBonus
{
public:
    //銃の攻撃力の強化、説明を設定
    LevelUpGunAttack(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //強化効果を付与する
    void Apply() const override;

private:
    //強化値
    float mLevelUpValue;
};

