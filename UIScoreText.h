#pragma once
#include "UIText.h"
class UIScoreText :
    public UIText
{
public:
    UIScoreText(int fontHandle, unsigned int color) :UIText("0", fontHandle, color) { ; }

    ~UIScoreText();
    //�X�V
    void Update(float deltaTime)override;
};

