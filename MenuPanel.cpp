#include "MenuPanel.h"
#include "UIText.h"
#include "InputManager.h"
#include "SelectUIRect.h"
#include "UIControllDescription.h"
#include "UISoundSetting.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "DxLib.h"
#include "UIBase.h"

MenuPanel::MenuPanel(int width,int height)
{
	//�I���ł��鍀�ڂ̑傫���ݒ�
	Vector2D<int>selectRectSize(width / 3.0f, height / 12.0f);

	//���j���[�p�l���̒��S�ʒu�v�Z
	int centerX = width / 2;
	int centerY = height / 2;
	//�|�[�Y�e�L�X�g
	auto pauseText = std::make_shared<UIText>("�|�[�Y��", CreateFontToHandle("�l�r�S�V�b�N", 30, 2), UI::white);
	Vector2D<int>textPosi(centerX - (pauseText->GetTextWidth()-10), centerY - GetFontSize()-10 - (selectRectSize.y*2.8));
	pauseText->SetLocalPosition(textPosi);
	AddUIElement(pauseText);

	//������
	int selectCenterY = centerY - selectRectSize.y*2;
	UI::Rect selectRect(Vector2D<int>(centerX - (selectRectSize.x / 2), selectCenterY - (selectRectSize.y / 2)), selectRectSize,UI::gray,true);
	auto selectUI = std::make_shared<SelectUIRect>(selectRect,UI::red,true);
	selectUI->SetText(std::make_shared<UIText>("������", CreateFontToHandle("�l�r�S�V�b�N", 30, 3), UI::white));
	AddUIElement(selectUI);

	//�������
	UI::Rect selectRect2(Vector2D<int>(centerX - (selectRectSize.x / 2), centerY - (selectRectSize.y / 2)), selectRectSize, UI::gray, true);
	auto selectUI2 = std::make_shared<SelectUIRect>(selectRect2, UI::red, true);
	selectUI2->SetText(std::make_shared<UIText>("�������", CreateFontToHandle("�l�r�S�V�b�N", 30, 3), UI::white));
	AddUIElement(selectUI2);

	//�������
	int selectCenterY3 = centerY + selectRectSize.y * 2;
	UI::Rect selectRect3(Vector2D<int>(centerX - (selectRectSize.x / 2), selectCenterY3 - (selectRectSize.y / 2)), selectRectSize, UI::gray, true);
	auto selectUI3 = std::make_shared<SelectUIRect>(selectRect3, UI::red, true);
	selectUI3->SetText(std::make_shared<UIText>("���ʐݒ�", CreateFontToHandle("�l�r�S�V�b�N", 30, 3), UI::white));
	AddUIElement(selectUI3);

	//��߂�
	int selectCenterY4 = centerY + selectRectSize.y * 4;
	UI::Rect selectRect4(Vector2D<int>(centerX - (selectRectSize.x / 2), selectCenterY4 - (selectRectSize.y / 2)), selectRectSize, UI::gray, true);
	auto selectUI4 = std::make_shared<SelectUIRect>(selectRect4, UI::red, true);
	selectUI4->SetText(std::make_shared<UIText>("��߂�", CreateFontToHandle("�l�r�S�V�b�N", 30, 3), UI::white));
	AddUIElement(selectUI4);
	//�������
	mControllDescription = std::make_shared<UIControllDescription>(Vector2D<int>(centerX - (selectRectSize.x / 2) + 70, selectCenterY - (selectRectSize.y / 2)));
	mControllDescription->SetVisibilty(false);
	//���ʐݒ�
	mSoundSetting = std::make_shared<UISoundSetting>(Vector2D<int>(centerX - (selectRectSize.x / 2) + 100, selectCenterY - (selectRectSize.y / 2)+120));
	mSoundSetting->SetVisibilty(false);
}

MenuPanel::~MenuPanel()
{
	rects.clear();
	mUIList.clear();
}

void MenuPanel::Update(float deltaTime)
{
	if(!IsVisible()){
		return;
	}
	//���ʐݒ�\��
	if (mSoundSetting->IsVisible()){
		mSoundSetting->Update(deltaTime);
	}
	//��AW�L�[�������ꂽ�Ƃ�
	if(Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_UP)|| Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_W)){
		//����������\������
		if(mControllDescription->IsVisible()){ return; }
		//���ʐݒ蒆��
		if (mSoundSetting->IsVisible()) { return; }
		//�J�[�\������ԏ�Ȃ�A��ԉ��Ɉړ�
		if(mSelectIndex <= 0){
			mSelectIndex = mSelectRect.size()-1;
		}else{//�J�[�\�����グ��
			mSelectIndex--;
		}
	}

	if(Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_DOWN)||
		Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_S)){
		//����������\������
		if (mControllDescription->IsVisible()) { return; }
		//���ʐݒ蒆��
		if (mSoundSetting->IsVisible()) { return; }

		//�J�[�\������ԉ��Ȃ�A��ԏ�Ɉړ�
		if (mSelectIndex >= mSelectRect.size() - 1) {
			mSelectIndex = 0;
		}
		else {//�J�[�\����������
			mSelectIndex++;
		}
	}
	//�X�y�[�X�������ꂽ�Ƃ�
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_SPACE)) {

		//��������\�����Ȃ�A�\�������
		if(mControllDescription->IsVisible()){
			ViewControllDescription(false);
			return;
		}

		//���ʐݒ蒆�Ȃ�A���ʐݒ�����
		if (mSoundSetting->IsVisible()) {
			ViewSoundSetting(false);
			return;
		}

		switch (mSelectIndex)
		{
		case 0:
			//�|�[�Y����
			SCENE_M.GetCurrentScene()->SetPaused(false);
			break;
		case 1:
			//�������
			ViewControllDescription(true);
			break;
		case 2:
			//���ʐݒ�
			ViewSoundSetting(true);
			break;

		case 3:
			//�Q�[������߂�
			SCENE_M.ExitGame();
			break;
			
		default:
			SCENE_M.ExitGame();
			break;
		}
	}

	//�e�e�L�X�g�X�V����
	for (auto& x : mUIList) {
		x->Update(deltaTime);
	}
	//�e���ڍX�V����
	for (int i = 0; i < mSelectRect.size(); i++) {

		mSelectRect[i]->mIsSelect = i == mSelectIndex;
		mSelectRect[i]->Update(deltaTime);
	}
}

void MenuPanel::Draw(float deltaTime)
{
	if (!IsVisible()) {return;}

	//��������̂ݕ\��
	if(mControllDescription->IsVisible()){
		mControllDescription->Draw(deltaTime);
		return;
	}

	//���ʐݒ�̂ݕ\��
	if (mSoundSetting->IsVisible()) {
		mSoundSetting->Draw(deltaTime);
		return;
	}
	//�e�L�X�g�`��
	for(auto &x:mUIList){
		x->Draw(deltaTime);
	}
	//�g�`��
	for(auto &x:rects){
		DrawBox(x.mLeftTop.x, x.mLeftTop.y,x.RightBottom().x,x.RightBottom().y,x.mColor,x.mFillFlag);
	}
	//���ڕ`��
	for(int i =0;i<mSelectRect.size();i++){
		mSelectRect[i]->Draw(deltaTime);
	}

}

void MenuPanel::AddUIElement(std::shared_ptr<UIBase> ui)
{
	mUIList.push_back(ui);
}

void MenuPanel::AddUIElement(std::shared_ptr<SelectUIRect> ui)
{
	mSelectRect.push_back(ui);
}

void MenuPanel::AddRect(UI::Rect rect)
{
	rects.push_back(rect);
}

void MenuPanel::ViewControllDescription(bool IsOpen)
{
	//��������\����\��
	if(IsOpen){
		mControllDescription->SetVisibilty(true);
	}else{
		mControllDescription->SetVisibilty(false);
	}
}

void MenuPanel::ViewSoundSetting(bool IsOpen)
{
	//���ʐݒ�\����\��
	if (IsOpen) {
		mSoundSetting->SetVisibilty(true);
	}
	else {
		mSoundSetting->SetVisibilty(false);
	}
}
