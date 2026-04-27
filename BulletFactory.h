#pragma once
#include "ActorFactory.h"
#include <unordered_map>

class BulletFactory :
    public ActorFactory
{
public:
    BulletFactory();
    ~BulletFactory();
    std::shared_ptr<Actor>CreateObject(std::string name) override;
    virtual int CreateID(std::string name) override;
private:
    std::unordered_map<std::string, int>mBullet_Index;
};

