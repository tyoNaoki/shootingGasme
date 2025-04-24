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
	//選択できる項目の大きさ設定
	Vector2D<int>selectRectSize(width / 3.0f, height / 12.0f);

	//メニューパネルの中心位置計算
	int centerX = width / 2;
	int centerY = height / 2;
	//ポーズテキスト
	auto pauseText = std::make_shared<UIText>("ポーズ中", CreateFontToHandle("ＭＳゴシック", 30, 2), UI::white);
	Vector2D<int>textPosi(centerX - (pauseText->GetTextWidth()-10), centerY - GetFontSize()-10 - (selectRectSize.y*2.8));
	pauseText->SetLocalPosition(textPosi);
	AddUIElement(pauseText);

	//続ける
	int selectCenterY = centerY - selectRectSize.y*2;
	UI::Rect selectRect(Vector2D<int>(centerX - (selectRectSize.x / 2), selectCenterY - (selectRectSize.y / 2)), selectRectSize,UI::gray,true);
	auto selectUI = std::make_shared<SelectUIRect>(selectRect,UI::red,true);
	selectUI->SetText(std::make_shared<UIText>("続ける", CreateFontToHandle("ＭＳゴシック", 30, 3), UI::white));
	AddUIElement(selectUI);

	//操作説明
	UI::Rect selectRect2(Vector2D<int>(centerX - (selectRectSize.x / 2), centerY - (selectRectSize.y / 2)), selectRectSize, UI::gray, true);
	auto selectUI2 = std::make_shared<SelectUIRect>(selectRect2, UI::red, true);
	selectUI2->SetText(std::make_shared<UIText>("操作説明", CreateFontToHandle("ＭＳゴシック", 30, 3), UI::white));
	AddUIElement(selectUI2);

	//操作説明
	int selectCenterY3 = centerY + selectRectSize.y * 2;
	UI::Rect selectRect3(Vector2D<int>(centerX - (selectRectSize.x / 2), selectCenterY3 - (selectRectSize.y / 2)), selectRectSize, UI::gray, true);
	auto selectUI3 = std::make_shared<SelectUIRect>(selectRect3, UI::red, true);
	selectUI3->SetText(std::make_shared<UIText>("音量設定", CreateFontToHandle("ＭＳゴシック", 30, 3), UI::white));
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
	//音量設定
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
	//音量設定表示
	if (mSoundSetting->IsVisible()){
		mSoundSetting->Update(deltaTime);
	}
	//上、Wキーが押されたとき
	if(Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_UP)|| Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_W)){
		//操作説明が表示中か
		if(mControllDescription->IsVisible()){ return; }
		//音量設定中か
		if (mSoundSetting->IsVisible()) { return; }
		//カーソルが一番上なら、一番下に移動
		if(mSelectIndex <= 0){
			mSelectIndex = mSelectRect.size()-1;
		}else{//カーソルを上げる
			mSelectIndex--;
		}
	}

	if(Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_DOWN)||
		Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_S)){
		//操作説明が表示中か
		if (mControllDescription->IsVisible()) { return; }
		//音量設定中か
		if (mSoundSetting->IsVisible()) { return; }

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

		//操作説明表示中なら、表示を閉じる
		if(mControllDescription->IsVisible()){
			ViewControllDescription(false);
			return;
		}

		//音量設定中なら、音量設定を閉じる
		if (mSoundSetting->IsVisible()) {
			ViewSoundSetting(false);
			return;
		}

		switch (mSelectIndex)
		{
		case 0:
			//ポーズ解除
			SCENE_M.GetCurrentScene()->SetPaused(false);
			break;
		case 1:
			//操作説明
			ViewControllDescription(true);
			break;
		case 2:
			//音量設定
			ViewSoundSetting(true);
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

	//各テキスト更新処理
	for (auto& x : mUIList) {
		x->Update(deltaTime);
	}
	//各項目更新処理
	for (int i = 0; i < mSelectRect.size(); i++) {

		mSelectRect[i]->mIsSelect = i == mSelectIndex;
		mSelectRect[i]->Update(deltaTime);
	}
}

void MenuPanel::Draw(float deltaTime)
{
	if (!IsVisible()) {return;}

	//操作説明のみ表示
	if(mControllDescription->IsVisible()){
		mControllDescription->Draw(deltaTime);
		return;
	}

	//音量設定のみ表示
	if (mSoundSetting->IsVisible()) {
		mSoundSetting->Draw(deltaTime);
		return;
	}
	//テキスト描画
	for(auto &x:mUIList){
		x->Draw(deltaTime);
	}
	//枠描画
	for(auto &x:rects){
		DrawBox(x.mLeftTop.x, x.mLeftTop.y,x.RightBottom().x,x.RightBottom().y,x.mColor,x.mFillFlag);
	}
	//項目描画
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
	//操作説明表示非表示
	if(IsOpen){
		mControllDescription->SetVisibilty(true);
	}else{
		mControllDescription->SetVisibilty(false);
	}
}

void MenuPanel::ViewSoundSetting(bool IsOpen)
{
	//音量設定表示非表示
	if (IsOpen) {
		mSoundSetting->SetVisibilty(true);
	}
	else {
		mSoundSetting->SetVisibilty(false);
	}
}
