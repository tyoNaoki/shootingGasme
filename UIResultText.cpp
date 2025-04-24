#include "UIResultText.h"

UIResultText::UIResultText(int fontHandle, unsigned int color) :UIText("", fontHandle,color)
{
	results.resize(3);
}
