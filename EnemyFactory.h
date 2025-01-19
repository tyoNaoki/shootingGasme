#pragma once
#include "CharacterFactory.h"
#include <unordered_map>

class Map;

class EnemyFactory :
    public CharacterFactory
{
    friend class Singleton<EnemyFactory>;
public:
    std::shared_ptr<CharacterBase>CreateObject(std::string name) override;
    int CreateID(std::string name)override;

    Vector2D<float> GetRandomSpawnPosition(const float radius,std::shared_ptr<Map>spawnMap);

private:
    EnemyFactory();
    ~EnemyFactory();

    std::unordered_map<std::string,int>mChara_Index;
};

