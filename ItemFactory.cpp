#include "ItemFactory.h"
#include "GraphicManager.h"
#include "StatusManager.h"
#include "LevelUpItem.h"
#include "HealItem.h"

std::shared_ptr<Actor> ItemFactory::CreateObject(std::string name)
{
    //各アイテムごとの作成処理
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
    //識別IDをアイテムごとに発行
    auto it = mItem_Indexs.find(name);
    if(it != mItem_Indexs.end()){
        int item_id = it->second;
        it->second++;
        return item_id;
    }
    
    //見つからない場合お、エラー数値を返す
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
