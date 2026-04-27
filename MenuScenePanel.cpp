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
	//項目の大きさ設定
	Vector2D<int>selectRectSize(width / 3.0f, height / 12.0f);
	//中心位置計算
	int centerX = width / 2;
	int centerY = height / 2;

	//タイトル文字の影
	Vector2D<int>textPosi(centerX - 140, centerY - GetFontSize() - 10 - (selectRectSize.y * 2.8));
	auto titleShadow = std::make_shared<UIText>("DX SURVIVER", CreateFontToHandle("Arial", 40, -1, DX_FONTTYPE_ANTIALIASING), UI::red);
	titleShadow->SetLocalPosition(textPosi);
	AddUIElement(titleShadow);

	//タイトル文字
	auto title = std::make_shared<UIText>("DX SURVIVER", CreateFontToHandle("Arial", 40, -1, DX_FONTTYPE_ANTIALIASING), UI::white);
	title->SetLocalPosition(Vector2D<float>(textPosi.x - 2.0f, textPosi.y - 2.0f));
	AddUIElement(title);

	//ゲームを始める
	int selectCenterY = centerY - selectRectSize.y * 2;
	UI::Rect selectRect(Vector2D<int>(centerX - (selectRectSize.x / 2), selectCenterY - (selectRectSize.y / 2)), selectRectSize, UI::gray, true);
	auto selectUI = std::make_shared<SelectUIRect>(selectRect, UI::red, true);
	selectUI->SetText(std::make_shared<UIText>("始める", CreateFontToHandle("ＭＳゴシック", 30, 3), UI::white));
	AddUIElement(selectUI);

	//操作説明
	UI::Rect selectRect2(Vector2D<int>(centerX - (selectRectSize.x / 2), centerY - (selectRectSize.y / 2)), selectRectSize, UI::gray, true);
	auto selectUI2 = std::make_shared<SelectUIRect>(selectRect2, UI::red, true);
	selectUI2->SetText(std::make_shared<UIText>("操作説明", CreateFontToHandle("ＭＳゴシック", 30, 3), UI::white));
	AddUIElement(selectUI2);

	//クレジット
	int selectCenterY3 = centerY + selectRectSize.y * 2;
	UI::Rect selectRect3(Vector2D<int>(centerX - (selectRectSize.x / 2), selectCenterY3 - (selectRectSize.y / 2)), selectRectSize, UI::gray, true);
	auto selectUI3 = std::make_shared<SelectUIRect>(selectRect3, UI::red, true);
	selectUI3->SetText(std::make_shared<UIText>("クレジット", CreateFontToHandle("ＭＳゴシック", 30, 3), UI::white));
	AddUIElement(selectUI3);

	//やめる
	int selectCenterY4 = centerY + selectRectSize.y * 4;
	UI::Rect selectRect4(Vector2D<int>(centerX - (selectRectSize.x / 2), selectCenterY4 - (selectRectSize.y / 2)), selectRectSize, UI::gray, true);
	auto selectUI4 = std::make_shared<SelectUIRect>(selectRect4, UI::red, true);
	selectUI4->SetText(std::make_shared<UIText>("やめる", CreateFontToHandle("ＭＳゴシック", 30, 3), UI::white));
	AddUIElement(selectUI4);

	//操作説明
	mControllDescription = std::make_shared<UIControllDescription>(Vector2D<int>(centerX - (selectRectSize.x / 2) + 70, selectCenterY - (selectRectSize.y / 2)));
	mControllDescription->SetVisibilty(false);

	//クレジット
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
	//表示するか
	if (!IsVisible()) {
		return;
	}
	//上かWキーが押されたとき
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_UP) || Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_W)) {
		//操作説明が表示中か
		if (mControllDescription->IsVisible()) { return; }
		//クレジット表示中か
		if (mCredit->IsVisible()) { return; }

		//カーソルが一番上なら、一番下に移動
		if (mSelectIndex <= 0) {
			mSelectIndex = mSelectRect.size() - 1;
		}
		else {//カーソルを上げる
			mSelectIndex--;
		}
	}

	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_DOWN) ||
		Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_S)) {
		//操作説明が表示中か
		if (mControllDescription->IsVisible()) { return; }
		//クレジット表示中か
		if(mCredit->IsVisible()){return;}
		//カーソルが一番下なら、一番上に移動
		if (mSelectIndex >= mSelectRect.size() - 1) {
			mSelectIndex = 0;
		}
		else {//カーソルを下げる
			mSelectIndex++;
		}
	}
	//スペースが押されたとき
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_SPACE)) {
		//操作説明が表示中なら、操作説明を閉じる
		if (mControllDescription->IsVisible()) {
			ViewControllDescription(false);
			return;
		}
		//クレジットを表示中ならクレジットの表示を閉じる
		if (mCredit->IsVisible()) {
			ViewCredit(false);
			return;
		}

		switch (mSelectIndex)
		{
		case 0:
			//ゲームを始める
			SCENE_M.ChangeScene(SceneState::NEXTSCENE);
			break;
		case 1:
			//操作説明
			ViewControllDescription(true);
			break;
		case 2:
			//クレジット
			ViewCredit(true);
			break;
		case 3:
			//ゲームをやめる
			SCENE_M.ExitGame();
			break;

		default:
			SCENE_M.ExitGame();
			break;
		}
	}

	//テキスト描画
	for (auto& x : mUIList) {
		x->Update(deltaTime);
	}
	//項目描画
	for (int i = 0; i < mSelectRect.size(); i++) {

		mSelectRect[i]->mIsSelect = i == mSelectIndex;
		mSelectRect[i]->Update(deltaTime);
	}
}

void MenuScenePanel::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }

	//操作説明のみ表示
	if (mControllDescription->IsVisible()) {
		mControllDescription->Draw(deltaTime);
		return;
	}

	//クレジットのみ表示
	if (mCredit->IsVisible()) {
		mCredit->Draw(deltaTime);
		return;
	}
	//テキスト描画
	for (auto& x : mUIList) {
		x->Draw(deltaTime);
	}
	//大枠描画
	for (auto& x : rects) {
		DrawBox(x.mLeftTop.x, x.mLeftTop.y, x.RightBottom().x, x.RightBottom().y, x.mColor, x.mFillFlag);
	}
	//項目描画
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
	//操作説明表示、非表示
	if (IsOpen) {
		mControllDescription->SetVisibilty(true);
	}
	else {
		mControllDescription->SetVisibilty(false);
	}
}

void MenuScenePanel::ViewCredit(bool IsOpen)
{
	//クレジット説明表示、非表示
	if (IsOpen) {
		mCredit->SetVisibilty(true);
	}
	else {
		mCredit->SetVisibilty(false);
	}
}
