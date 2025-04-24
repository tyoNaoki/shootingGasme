#include "MenuScenePanel.h"
#include "UIText.h"
#include "InputManager.h"
#include "SelectUIRect.h"
#include "UIControllDescription.h"
#include "UICredit.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "DxLib.h"
#include "UIBase.h"

MenuScenePanel::MenuScenePanel(int width, int height)
{
	//���ڂ̑傫���ݒ�
	Vector2D<int>selectRectSize(width / 3.0f, height / 12.0f);
	//���S�ʒu�v�Z
	int centerX = width / 2;
	int centerY = height / 2;

	//�^�C�g�������̉e
	Vector2D<int>textPosi(centerX - 140, centerY - GetFontSize() - 10 - (selectRectSize.y * 2.8));
	auto titleShadow = std::make_shared<UIText>("DX SURVIVER", CreateFontToHandle("Arial", 40, -1, DX_FONTTYPE_ANTIALIASING), UI::red);
	titleShadow->SetLocalPosition(textPosi);
	AddUIElement(titleShadow);

	//�^�C�g������
	auto title = std::make_shared<UIText>("DX SURVIVER", CreateFontToHandle("Arial", 40, -1, DX_FONTTYPE_ANTIALIASING), UI::white);
	title->SetLocalPosition(Vector2D<float>(textPosi.x - 2.0f, textPosi.y - 2.0f));
	AddUIElement(title);

	//�Q�[�����n�߂�
	int selectCenterY = centerY - selectRectSize.y * 2;
	UI::Rect selectRect(Vector2D<int>(centerX - (selectRectSize.x / 2), selectCenterY - (selectRectSize.y / 2)), selectRectSize, UI::gray, true);
	auto selectUI = std::make_shared<SelectUIRect>(selectRect, UI::red, true);
	selectUI->SetText(std::make_shared<UIText>("�n�߂�", CreateFontToHandle("�l�r�S�V�b�N", 30, 3), UI::white));
	AddUIElement(selectUI);

	//�������
	UI::Rect selectRect2(Vector2D<int>(centerX - (selectRectSize.x / 2), centerY - (selectRectSize.y / 2)), selectRectSize, UI::gray, true);
	auto selectUI2 = std::make_shared<SelectUIRect>(selectRect2, UI::red, true);
	selectUI2->SetText(std::make_shared<UIText>("�������", CreateFontToHandle("�l�r�S�V�b�N", 30, 3), UI::white));
	AddUIElement(selectUI2);

	//�N���W�b�g
	int selectCenterY3 = centerY + selectRectSize.y * 2;
	UI::Rect selectRect3(Vector2D<int>(centerX - (selectRectSize.x / 2), selectCenterY3 - (selectRectSize.y / 2)), selectRectSize, UI::gray, true);
	auto selectUI3 = std::make_shared<SelectUIRect>(selectRect3, UI::red, true);
	selectUI3->SetText(std::make_shared<UIText>("�N���W�b�g", CreateFontToHandle("�l�r�S�V�b�N", 30, 3), UI::white));
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

	//�N���W�b�g
	mCredit = std::make_shared<UICredit>(Vector2D<int>(centerX - (selectRectSize.x / 2) -300, selectCenterY - (selectRectSize.y / 2)));
	mCredit->SetVisibilty(false);
}

MenuScenePanel::~MenuScenePanel()
{
	rects.clear();
	mUIList.clear();
}

void MenuScenePanel::Update(float deltaTime)
{
	//�\�����邩
	if (!IsVisible()) {
		return;
	}
	//�ォW�L�[�������ꂽ�Ƃ�
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_UP) || Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_W)) {
		//����������\������
		if (mControllDescription->IsVisible()) { return; }
		//�N���W�b�g�\������
		if (mCredit->IsVisible()) { return; }

		//�J�[�\������ԏ�Ȃ�A��ԉ��Ɉړ�
		if (mSelectIndex <= 0) {
			mSelectIndex = mSelectRect.size() - 1;
		}
		else {//�J�[�\�����グ��
			mSelectIndex--;
		}
	}

	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_DOWN) ||
		Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_S)) {
		//����������\������
		if (mControllDescription->IsVisible()) { return; }
		//�N���W�b�g�\������
		if(mCredit->IsVisible()){return;}
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
		//����������\�����Ȃ�A������������
		if (mControllDescription->IsVisible()) {
			ViewControllDescription(false);
			return;
		}
		//�N���W�b�g��\�����Ȃ�N���W�b�g�̕\�������
		if (mCredit->IsVisible()) {
			ViewCredit(false);
			return;
		}

		switch (mSelectIndex)
		{
		case 0:
			//�Q�[�����n�߂�
			SCENE_M.ChangeScene(SceneState::NEXTSCENE);
			break;
		case 1:
			//�������
			ViewControllDescription(true);
			break;
		case 2:
			//�N���W�b�g
			ViewCredit(true);
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

	//�e�L�X�g�`��
	for (auto& x : mUIList) {
		x->Update(deltaTime);
	}
	//���ڕ`��
	for (int i = 0; i < mSelectRect.size(); i++) {

		mSelectRect[i]->mIsSelect = i == mSelectIndex;
		mSelectRect[i]->Update(deltaTime);
	}
}

void MenuScenePanel::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }

	//��������̂ݕ\��
	if (mControllDescription->IsVisible()) {
		mControllDescription->Draw(deltaTime);
		return;
	}

	//�N���W�b�g�̂ݕ\��
	if (mCredit->IsVisible()) {
		mCredit->Draw(deltaTime);
		return;
	}
	//�e�L�X�g�`��
	for (auto& x : mUIList) {
		x->Draw(deltaTime);
	}
	//��g�`��
	for (auto& x : rects) {
		DrawBox(x.mLeftTop.x, x.mLeftTop.y, x.RightBottom().x, x.RightBottom().y, x.mColor, x.mFillFlag);
	}
	//���ڕ`��
	for (int i = 0; i < mSelectRect.size(); i++) {
		mSelectRect[i]->Draw(deltaTime);
	}
}

void MenuScenePanel::AddUIElement(std::shared_ptr<UIBase> ui)
{
	mUIList.push_back(ui);
}

void MenuScenePanel::AddUIElement(std::shared_ptr<SelectUIRect> ui)
{
	mSelectRect.push_back(ui);
}

void MenuScenePanel::AddRect(UI::Rect rect)
{
	rects.push_back(rect);
}

void MenuScenePanel::ViewControllDescription(bool IsOpen)
{
	//��������\���A��\��
	if (IsOpen) {
		mControllDescription->SetVisibilty(true);
	}
	else {
		mControllDescription->SetVisibilty(false);
	}
}

void MenuScenePanel::ViewCredit(bool IsOpen)
{
	//�N���W�b�g�����\���A��\��
	if (IsOpen) {
		mCredit->SetVisibilty(true);
	}
	else {
		mCredit->SetVisibilty(false);
	}
}
