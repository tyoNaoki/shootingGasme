#include "UISoundSetting.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "DxLib.h"

UISoundSetting::UISoundSetting(Vector2D<float> localPosition)
{
	//座標、大きさ設定
	mLocalPosition = localPosition;
    auto rectPosi = Vector2D<float>(localPosition.x - 30, localPosition.y - 30);
    auto size = Vector2D<int>(500, 180);
    mRects.push_back(UI::Rect(rectPosi, size, UI::gray, true));
	//テキスト設定
    mFontHandle = CreateFontToHandle("ＭＳゴシック", 30, 2);
	mTexts.push_back({"ゲーム音量 : ",0});
	mTexts.push_back({"エフェクト音量 : ",0});
	//色設定
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
	//BGMと効果音の音量を取得
	mTexts[0].settingNum = SOUND_M.GetBGMVolume();
	mTexts[1].settingNum = SOUND_M.GetActionSoundVolume();
	//上かWキーが押されたとき
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_UP) || Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_W)) {
		//一番上なら、一番下にカーソルを動かす
		if (mSelectIndex == 0) {
			mSelectIndex = 1;
		}else {//カーソルを一番上にする
			mSelectIndex = 0;
		}
	}

	//下かＳキーが押されたとき
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_DOWN) ||
		Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_S)) {

		if (mSelectIndex == 1) {//カーソルが下なら、上に動かす
			mSelectIndex = 0;
		}else {//カーソルに下に動かす
			mSelectIndex = 1;
		}
	}
	//左かＡキーが押されたとき
	if (Singleton<InputManager>::get_instance().IsPushingKey(KEY_INPUT_LEFT) ||
		Singleton<InputManager>::get_instance().IsPushingKey(KEY_INPUT_A)) {
		//現在の選択されている音量項目の音量を１下げる
		ChangeSoundVolume(mSelectIndex,-1);
	}

	//右かＤキーが押されたとき
	if (Singleton<InputManager>::get_instance().IsPushingKey(KEY_INPUT_RIGHT) ||
		Singleton<InputManager>::get_instance().IsPushingKey(KEY_INPUT_D)) {
		//現在の選択されている音量項目の音量を１上げる
		ChangeSoundVolume(mSelectIndex, 1);
	}
}

void UISoundSetting::Draw(float deltaTime)
{
	//表示するか
	if (!IsVisible()) {
		return;
	}
	//枠描画
	for (auto& x : mRects) {
		DrawBox(x.mLeftTop.x, x.mLeftTop.y, x.RightBottom().x, x.RightBottom().y, x.mColor, x.mFillFlag);
	}
	//テキスト描画
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
	//選択が一番上ならBGMの音量を調整
	if(selectIndex == 0){
		if(!SOUND_M.CheckBGMPlaying()){
			return;
		}
		int volume = SOUND_M.GetBGMVolume() + addVolume;

		if(volume<0||volume>100){return;}

		SOUND_M.ChangeBGMVolume(volume);
		return;
	}else{//選択が一番下なら効果音の音量を調整
		int volume = SOUND_M.GetActionSoundVolume() + addVolume;

		if (volume < 0 || volume>100) { return; }

		SOUND_M.ChangeActionSoundVolume(volume);
		return;
	}
}
