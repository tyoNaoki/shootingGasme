#pragma once
#include "LevelUpBonus.h"
class LevelUpGetReflectionGun :
    public LevelUpBonus
{
public:
    //���ˏe�����A������ݒ�
    LevelUpGetReflectionGun(std::string tag,RewardType type,std::string name, std::string description);
    //�v���C���[�����˕������肷��
    void Apply() const override;
};

