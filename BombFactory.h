#pragma once
#include "ActorFactory.h"
class BombFactory :
    public ActorFactory
{
public:
    BombFactory() = default;
    ~BombFactory() = default;
    std::shared_ptr<Actor>CreateObject(std::string name) override;
    virtual int CreateID(std::string name) override;

private:
    int mBomb_Index = 0;
};

