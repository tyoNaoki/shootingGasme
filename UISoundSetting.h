#pragma once
#include "UIBase.h"
struct SoundSettingText {
    std::string text;
    int settingNum;
};

class UISoundSetting :
    public UIBase
{
public:
    UISoundSetting(Vector2D<float>localPosition);

    ~UISoundSetting();
    //更新、描画
    virtual void Update(float deltaTime)override;
    virtual void Draw(float deltaTime)override;
    //色
    unsigned int mColor = -1;
    //選択時の色
    unsigned int mSelectColor = -1;

private:
    //選択されている項目の音量調整
    void ChangeSoundVolume(int selectIndex,int addVolume);

    std::vector<SoundSettingText> mTexts;
    std::vector<UI::Rect> mRects;
    int mFontHandle;
    //現在選択されている項目
    int mSelectIndex = 0;
};

