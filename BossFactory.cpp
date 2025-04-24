#include "BossFactory.h"
#include "BossEnemyA.h"
#include "GraphicManager.h"

std::shared_ptr<CharacterBase> BossFactory::CreateObject(std::string name)
{
    //�R���W�����ݒ�
    Collision::Rect rect();

    if (name != "BossEnemyA") {
        return nullptr;
    }

    //�摜�쐬
    Singleton<GraphicManager>::get_instance().CreateGraphic(name);
    auto boss = std::make_shared<BossEnemyA>();
    boss->SetCollision(std::make_shared<Collision::Rect>(Vector2D<float>(0, 0), Vector2D<float>(240, 240)));
    return boss;
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
