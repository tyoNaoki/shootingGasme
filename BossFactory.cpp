#include "BossFactory.h"
#include "BossEnemyA.h"
#include "GraphicManager.h"

std::shared_ptr<CharacterBase> BossFactory::CreateObject(std::string name)
{
    //ƒRƒŠƒWƒ‡ƒ“İ’è
    Collision::Rect rect(Vector2D<float>(0, 0), Vector2D<float>(240,240));

    if (name != "BossEnemyA") {
        return nullptr;
    }

    //‰æ‘œì¬
    Singleton<GraphicManager>::get_instance().CreateGraphic(name);

    return std::make_shared<BossEnemyA>(rect);
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
