#include "BombFactory.h"
#include "Bomb.h"
#include "GraphicManager.h"

std::shared_ptr<Actor> BombFactory::CreateObject(std::string name)
{
    Singleton<GraphicManager>::get_instance().CreateGraphic(name);
    return std::make_shared<Bomb>(Vector2D<float>(0, 0));
}

int BombFactory::CreateID(std::string name)
{
    int id = mBomb_Index;
    mBomb_Index++;
    return id;
}
