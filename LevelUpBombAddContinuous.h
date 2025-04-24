#pragma once
#include "LevelUpBonus.h"
class LevelUpBombAddContinuous :
    public LevelUpBonus
{
public:
    //���e�Ɏ����_���[�W�^��t�^������ʁA��V�̊�b����ݒ�
    LevelUpBombAddContinuous(std::string tag, RewardType type, float changeAttackValue,float explosionTime, std::string name, std::string description);
    //��V��t�^����
    void Apply() const override;
private:
    //���e�̎����_���[�W�^�̂P��_���[�W
    float newAttack;
    //���e�̔�����������
    float newExplosionTime;
};

