#pragma once
#include "ActorFactory.h"

// レーザー生成ファクトリクラス
class LaserFactory : public ActorFactory {
public:
    // オブジェクト生成関数
    std::shared_ptr<Actor> CreateObject(std::string name) override;
    // ID生成関数
    virtual int CreateID(std::string name) override;

private:
    int nextID = 0; // 次のID
};


