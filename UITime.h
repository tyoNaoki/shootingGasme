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
    //更新、描画
    void Update(float deltaTime)override;
    void Draw(float deltaTime)override;

private:
    //分,秒に変換、取得
    std::string GetMinutes(float gameTime);

    std::shared_ptr<GameScene>mScene;

    std::shared_ptr<PlayerCharacter>player;
    //現在の時間
    float mCurrentTime = 0.0f;
    //true時、クリアまでの時間表示。false時、ゲーム内の経過時間表示
    bool IsReverse = false;
    
    int mFontHandle;
    unsigned int mColor = -1;
};

