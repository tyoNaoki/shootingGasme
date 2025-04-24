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
    //�X�V�A�`��
    virtual void Update(float deltaTime)override;
    virtual void Draw(float deltaTime)override;
    //�F
    unsigned int mColor = -1;
    //�I�����̐F
    unsigned int mSelectColor = -1;

private:
    //�I������Ă��鍀�ڂ̉��ʒ���
    void ChangeSoundVolume(int selectIndex,int addVolume);

    std::vector<SoundSettingText> mTexts;
    std::vector<UI::Rect> mRects;
    int mFontHandle;
    //���ݑI������Ă��鍀��
    int mSelectIndex = 0;
};

