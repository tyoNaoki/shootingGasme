#pragma once
#include "UIBar.h"
class UIExperienceBar :
    public UIBar
{
public:
    UIExperienceBar(float width, float height, float value, float maxValue, int barColor, int rectColor);

    void Update(float deltaTime) override;
};

