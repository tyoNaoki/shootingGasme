#pragma once
#include "LevelUpBonus.h"
class LevelUpBombAddContinuous :
    public LevelUpBonus
{
public:
    //”š’e‚É‘±ƒ_ƒ[ƒWŒ^‚ğ•t—^‚·‚éŒø‰ÊA•ñV‚ÌŠî‘bî•ñ‚ğİ’è
    LevelUpBombAddContinuous(std::string tag, RewardType type, float changeAttackValue,float explosionTime, std::string name, std::string description);
    //•ñV‚ğ•t—^‚·‚é
    void Apply() const override;
private:
    //”š’e‚Ì‘±ƒ_ƒ[ƒWŒ^‚Ì‚P‰ñƒ_ƒ[ƒW
    float newAttack;
    //”š’e‚Ì”š”­‘±ŠÔ
    float newExplosionTime;
};

