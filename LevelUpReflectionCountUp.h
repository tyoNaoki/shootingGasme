#pragma once
#include "LevelUpBonus.h"
class LevelUpReflectionCountUp :
    public LevelUpBonus
{
public:
    //���ˏe�̔��ˉ񐔂̋����l��������
    LevelUpReflectionCountUp(std::string tag, RewardType type, int levelUpValue, std::string name, std::string description);
    //�������ʂ��v���C���[�ɕt�^
    void Apply() const override;

private:
    //�����l
    int mLevelUpValue;
};

