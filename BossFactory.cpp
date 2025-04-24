#include "BossFactory.h"
#include "BossEnemyA.h"
#include "GraphicManager.h"

std::shared_ptr<CharacterBase> BossFactory::CreateObject(std::string name)
{
    //ƒRƒŠƒWƒ‡ƒ“İ’è
<<<<<<< HEAD
    Collision::Rect rect();
=======
    Collision::Rect rect(Vector2D<float>(0, 0), Vector2D<float>(240,240));
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420

    if (name != "BossEnemyA") {
        return nullptr;
    }

    //‰æ‘œì¬
    Singleton<GraphicManager>::get_instance().CreateGraphic(name);
<<<<<<< HEAD
    auto boss = std::make_shared<BossEnemyA>();
    boss->SetCollision(std::make_shared<Collision::Rect>(Vector2D<float>(0, 0), Vector2D<float>(240, 240)));
    return boss;
=======

    return std::make_shared<BossEnemyA>(rect);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
}

int BossFactory::CreateID(std::string name)
{
    if (name != "BossEnemyA") {
        return -1;
    }
    int id = mChara_Index;
    mChara_Index++;
    return id;
}
