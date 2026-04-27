#pragma once
#include "Actor.h"
#include <string>
#include "Singleton.h"

class ActorFactory
{
public:
    virtual std::shared_ptr<Actor>CreateObject(std::string name) = 0;
    virtual int CreateID(std::string name) = 0;
    virtual std::shared_ptr<Actor>CreateObject(){return nullptr;};
};

