#include "PlayerFactory.h"
#include "PlayerCharacter.h"
#include "Collision.h"
#include "DebugHelper.h"
#include "GraphicManager.h"

std::shared_ptr<CharacterBase> PlayerFactory::CreateObject(std::string name)
{
    auto player = std::make_shared<PlayerCharacter>();
    //�R���W�����傫���ݒ�
    player->SetCollision(std::make_shared<Collision::Rect>(Vector2D<float>(0,0),Vector2D<float>(64,64)));

    if(name != "PlayerCharacter"){
        return nullptr;
    }

    //�摜�쐬
    Singleton<GraphicManager>::get_instance().CreateGraphic(name);

    return player;
}

int PlayerFactory::CreateID(std::string name)
{
    //�v���C���[����Ȃ��Ȃ�A�G���[���l��Ԃ�
    if(name != "PlayerCharacter"){
        return -1;
    }
    //����ID�𔭍s
    int chara_id = mChara_Index;
    mChara_Index++;

    return chara_id;
}
