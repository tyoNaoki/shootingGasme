#pragma once
#include "LevelUpBonus.h"
class LevelUpPlayerMelee :
    public LevelUpBonus
{
public:
    //近接攻撃の攻撃力強化値、説明を設定
    LevelUpPlayerMelee(std::string tag,RewardType type, float plusValue, std::string name, std::string description);
    //強化効果を付与する
    void Apply() const override;

private:
    //強化値
    float mPlusValue;
};

