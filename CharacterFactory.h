#pragma once
#include "CharacterBase.h"
#include <string>
#include "Singleton.h"

class CharacterFactory
{
public:
    virtual std::shared_ptr<CharacterBase>CreateObject(std::string name) = 0;
    virtual int CreateID(std::string name) = 0;
};

