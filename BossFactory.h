#pragma once
#include "CharacterFactory.h"
class BossFactory :
    public CharacterFactory
{
    friend class Singleton<BossFactory>;

public:
    std::shared_ptr<CharacterBase>CreateObject(std::string name) override;
    int CreateID(std::string name) override;

private:
    BossFactory() = default;
    ~BossFactory() = default;

    int mChara_Index = 0;
};
<<<<<<< HEAD
=======

>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
