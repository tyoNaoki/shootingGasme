#include "UISoundSetting.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "DxLib.h"

UISoundSetting::UISoundSetting(Vector2D<float> localPosition)
{
	//���W�A�傫���ݒ�
	mLocalPosition = localPosition;
    auto rectPosi = Vector2D<float>(localPosition.x - 30, localPosition.y - 30);
    auto size = Vector2D<int>(500, 180);
    mRects.push_back(UI::Rect(rectPosi, size, UI::gray, true));
	//�e�L�X�g�ݒ�
    mFontHandle = CreateFontToHandle("�l�r�S�V�b�N", 30, 2);
	mTexts.push_back({"�Q�[������ : ",0});
	mTexts.push_back({"�G�t�F�N�g���� : ",0});
	//�F�ݒ�
	mColor = UI::black;
	mSelectColor = UI::red;
}

UISoundSetting::~UISoundSetting()
{
	DeleteFontToHandle(mFontHandle);
}

void UISoundSetting::Update(float deltaTime)
{
    if (!IsVisible()) {
        return;
    }
	//BGM�ƌ��ʉ��̉��ʂ��擾
	mTexts[0].settingNum = SOUND_M.GetBGMVolume();
	mTexts[1].settingNum = SOUND_M.GetActionSoundVolume();
	//�ォW�L�[�������ꂽ�Ƃ�
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_UP) || Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_W)) {
		//��ԏ�Ȃ�A��ԉ��ɃJ�[�\���𓮂���
		if (mSelectIndex == 0) {
			mSelectIndex = 1;
		}else {//�J�[�\������ԏ�ɂ���
			mSelectIndex = 0;
		}
	}

	//�����r�L�[�������ꂽ�Ƃ�
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_DOWN) ||
		Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_S)) {

		if (mSelectIndex == 1) {//�J�[�\�������Ȃ�A��ɓ�����
			mSelectIndex = 0;
		}else {//�J�[�\���ɉ��ɓ�����
			mSelectIndex = 1;
		}
	}
	//�����`�L�[�������ꂽ�Ƃ�
	if (Singleton<InputManager>::get_instance().IsPushingKey(KEY_INPUT_LEFT) ||
		Singleton<InputManager>::get_instance().IsPushingKey(KEY_INPUT_A)) {
		//���݂̑I������Ă��鉹�ʍ��ڂ̉��ʂ��P������
		ChangeSoundVolume(mSelectIndex,-1);
	}

	//�E���c�L�[�������ꂽ�Ƃ�
	if (Singleton<InputManager>::get_instance().IsPushingKey(KEY_INPUT_RIGHT) ||
		Singleton<InputManager>::get_instance().IsPushingKey(KEY_INPUT_D)) {
		//���݂̑I������Ă��鉹�ʍ��ڂ̉��ʂ��P�グ��
		ChangeSoundVolume(mSelectIndex, 1);
	}
}

void UISoundSetting::Draw(float deltaTime)
{
	//�\�����邩
	if (!IsVisible()) {
		return;
	}
	//�g�`��
	for (auto& x : mRects) {
		DrawBox(x.mLeftTop.x, x.mLeftTop.y, x.RightBottom().x, x.RightBottom().y, x.mColor, x.mFillFlag);
	}
	//�e�L�X�g�`��
	const float textSpace = 50;
	const float textSize = 30;
	for (int i = 0; i < mTexts.size(); i++) {
		std::string text = mTexts[i].text + std::to_string(mTexts[i].settingNum);
		float drawPosiY = mLocalPosition.y + i * textSpace + i * textSize;
		int color = mSelectIndex == i ? mSelectColor : mColor;
		DrawStringToHandle(mLocalPosition.x, drawPosiY, text.c_str(), color, mFontHandle);
	}
}

void UISoundSetting::ChangeSoundVolume(int selectIndex, int addVolume)
{
	//�I������ԏ�Ȃ�BGM�̉��ʂ𒲐�
	if(selectIndex == 0){
		if(!SOUND_M.CheckBGMPlaying()){
			return;
		}
		int volume = SOUND_M.GetBGMVolume() + addVolume;

		if(volume<0||volume>100){return;}

		SOUND_M.ChangeBGMVolume(volume);
		return;
	}else{//�I������ԉ��Ȃ���ʉ��̉��ʂ𒲐�
		int volume = SOUND_M.GetActionSoundVolume() + addVolume;

		if (volume < 0 || volume>100) { return; }

		SOUND_M.ChangeActionSoundVolume(volume);
		return;
	}
}
