#pragma once
#include "LevelUpBonus.h"
class LevelUpGunBulletSpeed :
    public LevelUpBonus
{
public:
    //�e�̒e���̋����A������ݒ�
    LevelUpGunBulletSpeed(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //�������ʂ�t�^
    void Apply() const override;

private:
    //�����l
    float mLevelUpValue;
};

