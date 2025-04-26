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
    if(mBullet_Index.find(name) == mBullet_Index.end()) return nullptr;

    Singleton<GraphicManager>::get_instance().CreateGraphic(name);

    //äeíeÇ≤Ç∆ÇÃçÏê¨èàóù
    if (name == "PlayerBullet") {
        auto bullet = std::make_shared<Bullet>(Vector2D<float>(0, 0), Vector2D<float>(1.0, 1.0));
        bullet->SetCollision(std::make_shared<Collision::Circle>(Vector2D<float>(), 13));
        return bullet;
    }

    if (name == "PlayerReflectionBullet") {
        auto bullet = std::make_shared<Bullet>(Vector2D<float>(0, 0), Vector2D<float>(1.0, 1.0));
        bullet->SetCollision(std::make_shared<Collision::Circle>(Vector2D<float>(), 26));
        return bullet;
    }

    if (name == "EnemyBullet") {
        auto bullet = std::make_shared<Bullet>(Vector2D<float>(0, 0), Vector2D<float>(1.5, 1.5));
        bullet->SetCollision(std::make_shared<Collision::Circle>(Vector2D<float>(), 11));
        return bullet;
    }

    if (name == "BossBullet") {
        auto bullet = std::make_shared<Bullet>(Vector2D<float>(0, 0), Vector2D<float>(15, 15));
        bullet->SetCollision(std::make_shared<Collision::Circle>(Vector2D<float>(), 65));
        return bullet;
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
