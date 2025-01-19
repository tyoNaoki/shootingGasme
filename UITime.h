#pragma once
#include "UIBase.h"
#include "ActorManager.h"

class GameScene;
class PlayerCharacter;

class UITime :
    public UIBase
{
public:
    UITime(int fontHandle, unsigned int color,bool isReverse);

    ~UITime();
    //�X�V�A�`��
    void Update(float deltaTime)override;
    void Draw(float deltaTime)override;

private:
    //��,�b�ɕϊ��A�擾
    std::string GetMinutes(float gameTime);

    std::shared_ptr<GameScene>mScene;

    std::shared_ptr<PlayerCharacter>player;
    //���݂̎���
    float mCurrentTime = 0.0f;
    //true���A�N���A�܂ł̎��ԕ\���Bfalse���A�Q�[�����̌o�ߎ��ԕ\��
    bool IsReverse = false;
    
    int mFontHandle;
    unsigned int mColor = -1;
};

