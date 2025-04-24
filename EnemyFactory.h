#pragma once
#include "CharacterFactory.h"
#include <unordered_map>

<<<<<<< HEAD
#define ENEMY_M Singleton<EnemyFactory>::get_instance()
=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
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

