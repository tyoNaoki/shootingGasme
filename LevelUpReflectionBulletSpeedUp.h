#pragma once
#include "LevelUpBonus.h"
class LevelUpReflectionBulletSpeedUp :
    public LevelUpBonus
{
public:
    //���ˏe�̒e���x�����ʁA������ݒ�
    LevelUpReflectionBulletSpeedUp(std::string tag, RewardType type, float levelUpValue, std::string name, std::string description);
    //��V���ʂ��v���C���[�ɕt�^����
    void Apply() const override;

private:
    //�����l
    float mLevelUpValue;
};

