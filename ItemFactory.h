#pragma once
#include "ActorFactory.h"
#include "ItemBase.h"
#include <unordered_map>

// アイテム生成ファクトリクラス
class ItemFactory : public ActorFactory {
    friend class Singleton<ItemFactory>;

public:
    // オブジェクト生成関数
    virtual std::shared_ptr<Actor> CreateObject(std::string name) override;
    // ID生成関数
    virtual int CreateID(std::string name) override;

private:
    //アイテムごとの識別IDのための配列作成
    ItemFactory();
    //ID消去
    ~ItemFactory();

    std::unordered_map<std::string, int> mItem_Indexs; // アイテムインデックスのマップ
};


