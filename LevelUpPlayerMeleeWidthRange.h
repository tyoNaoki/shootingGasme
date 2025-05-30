#pragma once
#include "LevelUpBonus.h"
class LevelUpPlayerMeleeWidthRange :
    public LevelUpBonus
{
public:
    //近接攻撃の範囲強化、説明を設定
    LevelUpPlayerMeleeWidthRange(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //強化効果を付与
    void Apply() const override;

private:
    //強化値
    float mLevelUpValue;
};

