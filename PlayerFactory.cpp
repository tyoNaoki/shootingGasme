#include "PlayerFactory.h"
#include "PlayerCharacter.h"
#include "Collision.h"
#include "DebugHelper.h"
#include "GraphicManager.h"

std::shared_ptr<CharacterBase> PlayerFactory::CreateObject(std::string name)
{
    //コリジョン大きさ設定
    Collision::Rect rect(Vector2D<float>(0,0),Vector2D<float>(64,64));

    if(name != "PlayerCharacter"){
        return nullptr;
    }

    //画像作成
    Singleton<GraphicManager>::get_instance().CreateGraphic(name);

    return std::make_shared<PlayerCharacter>(rect);
}

int PlayerFactory::CreateID(std::string name)
{
    //プレイヤーじゃないなら、エラー数値を返す
    if(name != "PlayerCharacter"){
        return -1;
    }
    //識別IDを発行
    int chara_id = mChara_Index;
    mChara_Index++;

    return chara_id;
}
