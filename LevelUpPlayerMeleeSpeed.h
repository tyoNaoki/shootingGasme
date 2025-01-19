#pragma once
#include "LevelUpBonus.h"
class LevelUpPlayerMeleeSpeed :
    public LevelUpBonus
{
public:
    //近接攻撃速度の強化、説明を設定
    LevelUpPlayerMeleeSpeed(std::string tag,RewardType type, float levelUpValue, std::string name, std::string description);
    //強化効果を付与する
    void Apply() const override;

private:
    //強化値
    float mLevelUpValue;
};

