#include "UILevelText.h"
#include "StatusManager.h"
#include "PlayerCharacter.h"

void UILevelText::Update(float deltaTime)
{
	SetText("Lv" + std::to_string(STATUS.GetCurrentPlayerStatus().level));
}
