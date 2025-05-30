#pragma once
#include "LevelUpBonus.h"
class LevelUpBombAddContinuous :
    public LevelUpBonus
{
public:
    //爆弾に持続ダメージ型を付与する効果、報酬の基礎情報を設定
    LevelUpBombAddContinuous(std::string tag, RewardType type, float changeAttackValue,float explosionTime, std::string name, std::string description);
    //報酬を付与する
    void Apply() const override;
private:
    //爆弾の持続ダメージ型の１回ダメージ
    float newAttack;
    //爆弾の爆発持続時間
    float newExplosionTime;
};

