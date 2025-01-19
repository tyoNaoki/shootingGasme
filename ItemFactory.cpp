#include "ItemFactory.h"
#include "GraphicManager.h"
#include "StatusManager.h"
#include "LevelUpItem.h"
#include "HealItem.h"

std::shared_ptr<Actor> ItemFactory::CreateObject(std::string name)
{
    //�e�A�C�e�����Ƃ̍쐬����
    if(name == "LevelUpItem"){
        Singleton<GraphicManager>::get_instance().CreateGraphic(name);
        return std::make_shared<LevelUpItem>();
    }
    if(name == "HealItem"){
        Singleton<GraphicManager>::get_instance().CreateGraphic(name);
        return std::make_shared<HealItem>();
    }

    return nullptr;
}

int ItemFactory::CreateID(std::string name)
{
    //����ID���A�C�e�����Ƃɔ��s
    auto it = mItem_Indexs.find(name);
    if(it != mItem_Indexs.end()){
        int item_id = it->second;
        it->second++;
        return item_id;
    }
    
    //������Ȃ��ꍇ���A�G���[���l��Ԃ�
    return -1;
}

ItemFactory::ItemFactory()
{
    mItem_Indexs["LevelUpItem"] = 0;
    mItem_Indexs["HealItem"] = 0;
}

ItemFactory::~ItemFactory()
{
    mItem_Indexs.clear();
}
