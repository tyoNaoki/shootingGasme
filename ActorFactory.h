#pragma once
#include "Actor.h"
#include <string>
#include "Singleton.h"

class ActorFactory
{
public:
    virtual std::shared_ptr<Actor>CreateObject(std::string name) = 0;
    virtual int CreateID(std::string name) = 0;
<<<<<<< HEAD
    virtual std::shared_ptr<Actor>CreateObject(){return nullptr;};
=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
};

