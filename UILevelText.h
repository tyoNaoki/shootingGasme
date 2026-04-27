#pragma once
#include "UIText.h"
#include "ActorManager.h"

class PlayerCharacter;

class UILevelText :
    public UIText
{
public:
    UILevelText(const std::string& text, int fontHandle, unsigned int color) :UIText(text, fontHandle,color) {player = ACTOR_M.GetCurrentPlayer() ; }
    //çXêV
    void Update(float deltaTime)override;
private:
    std::shared_ptr<PlayerCharacter>player;
};

