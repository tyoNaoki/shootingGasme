#pragma once
#include "LevelUpBonus.h"
class LevelUpPlayerMelee :
    public LevelUpBonus
{
public:
    //�ߐڍU���̍U���͋����l�A������ݒ�
    LevelUpPlayerMelee(std::string tag,RewardType type, float plusValue, std::string name, std::string description);
    //�������ʂ�t�^����
    void Apply() const override;

private:
    //�����l
    float mPlusValue;
};

