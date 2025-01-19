#pragma once
#include "UIBase.h"
#include "StatusManager.h"

class UIText;


class UILevelUpReward :
    public UIBase
{
public:
    UILevelUpReward(UI::Rect rect, unsigned int selectColor, bool isFlash,int nameFontHandle,int descriptionFontHandle);

    ~UILevelUpReward();
    //�X�V�A�`��
    void Update(float deltaTime)override;
    void Draw(float deltaTime)override;
    //��V�̌��ʂ�ݒ�
    void SetReward(std::shared_ptr<LevelUpBonus>reward);
    //�`��ʒu�ݒ�
    void SetLocalPosition(Vector2D<float>localPosition)override;
    //��V���v���C���[�ɕt�^����
    void ApplyReward();

    bool mIsSelect = false;

private:
    //�I������Ƃ��̘g�g��
    UI::Rect mRect;
    //��V�̌���
    std::shared_ptr<LevelUpBonus>mReward;
    //�F��e�L�X�g�Ȃǂ̃n���h��
    unsigned int mSelectColor;
    unsigned int mDefaultColor;
    int mNameFontHandle;
    int mDesFontHandle;
    //�����̓_��
    bool mIsFlash;
    float flashTime = 0.2f;
    float currentTime = 0.0f;
};

