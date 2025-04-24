#include "EnemyFactory.h"
#include "NormalEnemyA.h"
#include "NormalEnemyB.h"
#include "NormalEnemyC.h"
#include "GraphicManager.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"
#include "DebugHelper.h"
#include "Map.h"

std::shared_ptr<CharacterBase> EnemyFactory::CreateObject(std::string name)
{
    //32*32の子リジョン設定

<<<<<<< HEAD
    if(mChara_Index.find(name) == mChara_Index.end())return nullptr;

    Singleton<GraphicManager>::get_instance().CreateGraphic(name);

    if(name == "NormalEnemyA"){
        auto enemy = std::make_shared<NormalEnemyA>();
        enemy->SetCollision(std::make_shared<Collision::Rect>(Vector2D<float>(0, 0), Vector2D<float>(48, 48)));
        return enemy;
    }
    if (name == "NormalEnemyB") {
        auto enemy = std::make_shared<NormalEnemyB>();
        enemy->SetCollision(std::make_shared<Collision::Rect>(Vector2D<float>(0, 0), Vector2D<float>(48, 48)));
        return enemy;
    }

    if (name == "NormalEnemyC") {
        auto enemy = std::make_shared<NormalEnemyC>();
        enemy->SetCollision(std::make_shared<Collision::Rect>(Vector2D<float>(0, 0), Vector2D<float>(96, 96)));
        return enemy;
=======
    if(name == "NormalEnemyA"){
        Collision::Rect rect(Vector2D<float>(0, 0), Vector2D<float>(48, 48));
        Singleton<GraphicManager>::get_instance().CreateGraphic(name);
        return std::make_shared<NormalEnemyA>(rect);
    }
    if (name == "NormalEnemyB") {
        Collision::Rect rect(Vector2D<float>(0, 0), Vector2D<float>(48, 48));
        Singleton<GraphicManager>::get_instance().CreateGraphic(name);
        return std::make_shared<NormalEnemyB>(rect);
    }

    if (name == "NormalEnemyC") {
        Collision::Rect rect(Vector2D<float>(0, 0), Vector2D<float>(96, 96));
        Singleton<GraphicManager>::get_instance().CreateGraphic(name);
        return std::make_shared<NormalEnemyC>(rect);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
    }

    return nullptr;
}

int EnemyFactory::CreateID(std::string name)
{
    auto it = mChara_Index.find(name);
    if(it != mChara_Index.end()){
        int chara_id = it->second;
        it->second++;
        return chara_id;
    }

    return -1;
}

Vector2D<float> EnemyFactory::GetRandomSpawnPosition(const float radius,std::shared_ptr<Map> spawnMap)
{
    int width,height;
    GetWindowSize(&width,&height);
    int maxCount = 10;
    auto player = Singleton<ActorManager>::get_instance().GetCurrentPlayer();
    if (!player){return Vector2D<float>();}
    auto centerLocation = player->GetWorldPosition2D();

    auto &dh = Singleton<DebugHelper>::get_instance();

    for(int count = 0;count < maxCount;count++){

        // 0から2πまでのランダムな角度を生成
        double angle = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;

        // 0からradiusまでのランダムな距離を生成 
        float distanceX = static_cast<float>(rand()) / RAND_MAX * radius + width * 0.8;
        float distanceY = static_cast<float>(rand()) / RAND_MAX * radius + height* 0.8;

        // 極座標からデカルト座標への変換 
        float x = centerLocation.x + static_cast<float>(distanceX * cos(angle));
        float y = centerLocation.y + static_cast<float>(distanceY * sin(angle));
        
        Vector2D<float>spawnPosition(x, y);
        //std::string message = "spawnPosition is " + spawnPosition.ToString();
        //dh.Add(message,1.0f);
        if (spawnMap->GetTileOnWorldLocation(spawnPosition) == TileType::GROUND) {
            return spawnPosition;
        }
    }

    return Vector2D<float>();
}

EnemyFactory::EnemyFactory()
{
    mChara_Index["NormalEnemyA"] = 0;
    mChara_Index["NormalEnemyB"] = 0;
    mChara_Index["NormalEnemyC"] = 0;
}

EnemyFactory::~EnemyFactory()
{
    mChara_Index.clear();
}
