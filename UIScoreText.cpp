#include "UIScoreText.h"
#include "StatusManager.h"

UIScoreText::~UIScoreText()
{
}

void UIScoreText::Update(float deltaTime)
{
	std::string scoreText = "SCORE : "+std::to_string(STATUS.GetScore());
	SetText(scoreText);
}
