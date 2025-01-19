#include "LaserFactory.h"
#include "GraphicManager.h"
#include "Laser.h"

std::shared_ptr<Actor> LaserFactory::CreateObject(std::string name)
{
    //�쐬����
    if (name == "Laser") {
        Singleton<GraphicManager>::get_instance().CreateGraphic(name);
        return std::make_shared<Laser>();
    }

    return nullptr;
}

int LaserFactory::CreateID(std::string name)
{
    //����ID���s
    int id = nextID;
    nextID++;
    return id;
}
