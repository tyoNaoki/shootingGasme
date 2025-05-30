#pragma once
#include "LevelUpBonus.h"
class LevelUpBombExplosionRange :
    public LevelUpBonus
{
public:
    //爆弾武器の爆発範囲を強化、説明を設定
    LevelUpBombExplosionRange(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //強化効果を付与する
    void Apply() const override;

private:
    //強化値
    float mLevelUpValue;
};

