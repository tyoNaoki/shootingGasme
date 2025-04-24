#include "PlayerFactory.h"
#include "PlayerCharacter.h"
#include "Collision.h"
#include "DebugHelper.h"
#include "GraphicManager.h"

std::shared_ptr<CharacterBase> PlayerFactory::CreateObject(std::string name)
{
<<<<<<< HEAD
    auto player = std::make_shared<PlayerCharacter>();
    //コリジョン大きさ設定
    player->SetCollision(std::make_shared<Collision::Rect>(Vector2D<float>(0,0),Vector2D<float>(64,64)));
=======
    //コリジョン大きさ設定
    Collision::Rect rect(Vector2D<float>(0,0),Vector2D<float>(64,64));
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420

    if(name != "PlayerCharacter"){
        return nullptr;
    }

    //画像作成
    Singleton<GraphicManager>::get_instance().CreateGraphic(name);

<<<<<<< HEAD
    return player;
=======
    return std::make_shared<PlayerCharacter>(rect);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
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
