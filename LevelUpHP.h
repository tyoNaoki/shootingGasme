#pragma once
#include "LevelUpBonus.h"
class LevelUpHP :
    public LevelUpBonus
{
public:
    //HP�̑����ʁA������ݒ�
    LevelUpHP(RewardType type,float plusValue,std::string name,std::string description);
    //��V���ʂ�t�^
    void Apply() const override;

private:
    //HP�̑�����
    float mPlusValue;
};

