#pragma once
#include "LevelUpBonus.h"
class LevelUpReflectionBulletSpeedUp :
    public LevelUpBonus
{
public:
    //反射銃の弾速度強化量、説明を設定
    LevelUpReflectionBulletSpeedUp(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //報酬効果をプレイヤーに付与する
    void Apply() const override;

private:
    //強化値
    float mLevelUpValue;
};

