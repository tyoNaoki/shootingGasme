#pragma once
#include "LevelUpBonus.h"
class LevelUpGetThrowBomb :
    public LevelUpBonus
{
public:
    //���e��������A������ݒ�
    LevelUpGetThrowBomb(std::string tag, RewardType type, std::string name, std::string description);
    //�v���C���[�����e�������肷��
    void Apply() const override;
};

