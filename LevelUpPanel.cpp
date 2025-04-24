#include "LevelUpPanel.h"
#include "LevelUpBonus.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "UILevelUpReward.h"
#include "LevelUpBonus.h"
#include "UIText.h"
#include "StatusInfoPanel.h"

LevelUpPanel::LevelUpPanel(const int width,const int height,const int rewardMaxCount)
{
	mLevelUpPanelSize = Vector2D<int>(width / 2.0f, height / 1.5f);
	mSelectRectSize = Vector2D<int>(width / 3.0f, height / 9.0f);

	mCenter = Vector2D<int>(width / 2, height / 2);

	//レベルアップテキスト
	auto levelUpText = std::make_shared<UIText>("レベルアップ", CreateFontToHandle("ＭＳゴシック", 30, 2), UI::white);
	mTextPosi = Vector2D<int>(mCenter.x - (levelUpText->GetTextWidth() - 10), mCenter.y - GetFontSize() - 10 - (mSelectRectSize.y * 3.5));
	levelUpText->SetLocalPosition(mTextPosi);
	AddUIElement(levelUpText);

	//現在のステータスを表示するテキスト
	mStatusInfoPanel = std::make_shared<StatusInfoPanel>(mLevelUpPanelSize.x/2.5, mLevelUpPanelSize.y, CreateFontToHandle("ＭＳゴシック", 25, 2),UI::white);
	UI::Rect uiLevelUpRect(Vector2D<int>(),mSelectRectSize,UI::gray,true);
	mRewards.resize(rewardMaxCount);
	for(int i=0;i<rewardMaxCount;i++){
		mRewards[i] = std::make_shared<UILevelUpReward>(uiLevelUpRect, UI::lightShade, false, CreateFontToHandle("ＭＳゴシック", 26, 2), CreateFontToHandle("ＭＳゴシック", 20, 2));
	}
}

void LevelUpPanel::SetStatusInfoPanel()
{
	mStatusInfoPanel->SetStatusInfo();
}

void LevelUpPanel::SetLevelUpReward(const std::vector<std::shared_ptr<LevelUpBonus>>& rewards)
{
	//初期化&場所決定
	for(int i = 0;i<rewards.size();i++){
		mRewards[i]->SetReward(rewards[i]);
		mRewards[i]->mIsSelect = false;
		int centerY = mTextPosi.y + 30 + mSelectRectSize.y * (1+i*1) + (i*20);
		auto leftTopPosition = Vector2D<int>(mCenter.x - (mSelectRectSize.x / 2),centerY - (mSelectRectSize.y / 2));
		mRewards[i]->SetLocalPosition(leftTopPosition);
	}

	selectIndex = 0;
	rewardCount = rewards.size();
	mRewards[selectIndex]->mIsSelect = true;
}

void LevelUpPanel::Update(float deltaTime)
{
	if (!IsVisible()) { return; }

	//上、Wキーが押されたとき
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_UP) || Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_W)) {
		if (selectIndex <= 0) {//一番上まで来ていた場合、カーソルを一番下にする
			mRewards[0]->mIsSelect = false;
			selectIndex = rewardCount -1;
			mRewards[selectIndex]->mIsSelect = true;
		}
		else {//カーソルを１上げる
			mRewards[selectIndex]->mIsSelect = false;
			selectIndex--;
			mRewards[selectIndex]->mIsSelect = true;
		}
	}
	//下、Ｓキーが押されたとき
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_DOWN) ||
		Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_S)) {
		if (selectIndex >= rewardCount - 1) {//一番下まで来ていたとき、カーソルを一番上にあげる
			mRewards[rewardCount - 1]->mIsSelect = false;
			selectIndex = 0;
			mRewards[selectIndex]->mIsSelect = true;
		}
		else {//カーソルを1下げる
			mRewards[selectIndex]->mIsSelect = false;
			selectIndex++;
			mRewards[selectIndex]->mIsSelect = true;
		}
	}

	//スペースキーが押されたとき
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_SPACE)) {
		//決定
		SelectedReward();
	}
}

void LevelUpPanel::Draw(float deltaTime)
{
	//文字を描画
	for(auto&x:mUIList){
		x->Draw(deltaTime);
	}
	//選択可能な報酬を描画
	for(int i=0;i<rewardCount;i++){
		mRewards[i]->Draw(deltaTime);
	}
	//現在のステータスを表示
	mStatusInfoPanel->Draw(deltaTime);
}

void LevelUpPanel::SelectedReward() const
{
	//報酬の効果をプレイヤーに付与
	if (selectIndex >= 0 && selectIndex < mRewards.size()) {
		mRewards[selectIndex]->ApplyReward();
		//報酬を選択し終えたことを通知する
		SCENE_M.OnLevelUpRewardSelected();
	}
}

void LevelUpPanel::AddUIElement(std::shared_ptr<UIBase> ui)
{
	mUIList.push_back(ui);
}
