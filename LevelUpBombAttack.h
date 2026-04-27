#pragma once
#include "LevelUpBonus.h"
class LevelUpBombAttack :
    public LevelUpBonus
{
public:
    //爆弾の爆発ダメージ強化値、報酬の基本情報などを設定
    LevelUpBombAttack(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //強化効果を付与する
    void Apply() const override;

private:
    //強化値
    float mLevelUpValue;
};

