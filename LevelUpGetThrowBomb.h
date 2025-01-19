#pragma once
#include "LevelUpBonus.h"
class LevelUpGetThrowBomb :
    public LevelUpBonus
{
public:
    //爆弾武器を入手、説明を設定
    LevelUpGetThrowBomb(std::string tag, RewardType type, std::string name, std::string description);
    //プレイヤーが爆弾武器を入手する
    void Apply() const override;
};

