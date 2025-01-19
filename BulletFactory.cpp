#include "BulletFactory.h"
#include "GraphicManager.h"
#include "Bullet.h"

BulletFactory::BulletFactory()
{
    mBullet_Index["PlayerBullet"] = 0;
    mBullet_Index["PlayerReflectionBullet"] = 0;
    mBullet_Index["EnemyBullet"] = 0;
    mBullet_Index["BossBullet"] = 0;
}

BulletFactory::~BulletFactory()
{
    mBullet_Index.clear();
}

std::shared_ptr<Actor> BulletFactory::CreateObject(std::string name)
{
    //äeíeÇ≤Ç∆ÇÃçÏê¨èàóù
    if (name == "PlayerBullet") {
        Collision::Circle bulletCollision(Vector2D<float>(),13);
        Singleton<GraphicManager>::get_instance().CreateGraphic(name);
        return std::make_shared<Bullet>(bulletCollision,Vector2D<float>(0,0), Vector2D<float>(1.0, 1.0));
    }

    if (name == "PlayerReflectionBullet") {
        Collision::Circle bulletCollision(Vector2D<float>(), 26);
        Singleton<GraphicManager>::get_instance().CreateGraphic(name);
        return std::make_shared<Bullet>(bulletCollision, Vector2D<float>(0, 0), Vector2D<float>(1.0, 1.0));
    }

    if (name == "EnemyBullet") {
        Collision::Circle bulletCollision(Vector2D<float>(), 11);
        Singleton<GraphicManager>::get_instance().CreateGraphic(name);
        return std::make_shared<Bullet>(bulletCollision,Vector2D<float>(0,0),Vector2D<float>(1.5,1.5));
    }

    if (name == "BossBullet") {
        Collision::Circle bulletCollision(Vector2D<float>(), 65);
        Singleton<GraphicManager>::get_instance().CreateGraphic(name);
        return std::make_shared<Bullet>(bulletCollision, Vector2D<float>(0,0), Vector2D<float>(15, 15));
    }

    return nullptr;
}

int BulletFactory::CreateID(std::string name)
{
    //éØï IDÇíeñàÇ…î≠çsÇ∑ÇÈ
    auto it = mBullet_Index.find(name);
    if (it != mBullet_Index.end()) {
        int bullet_id = it->second;
        it->second++;
        return bullet_id;
    }

    return -1;
}
