#pragma once
#include "LevelUpBonus.h"
class LevelUpGetReflectionGun :
    public LevelUpBonus
{
public:
    //反射銃を入手、説明を設定
    LevelUpGetReflectionGun(std::string tag,RewardType type,std::string name, std::string description);
    //プレイヤーが反射武器を入手する
    void Apply() const override;
};

