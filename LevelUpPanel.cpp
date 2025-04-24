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

	//���x���A�b�v�e�L�X�g
	auto levelUpText = std::make_shared<UIText>("���x���A�b�v", CreateFontToHandle("�l�r�S�V�b�N", 30, 2), UI::white);
	mTextPosi = Vector2D<int>(mCenter.x - (levelUpText->GetTextWidth() - 10), mCenter.y - GetFontSize() - 10 - (mSelectRectSize.y * 3.5));
	levelUpText->SetLocalPosition(mTextPosi);
	AddUIElement(levelUpText);

	//���݂̃X�e�[�^�X��\������e�L�X�g
	mStatusInfoPanel = std::make_shared<StatusInfoPanel>(mLevelUpPanelSize.x/2.5, mLevelUpPanelSize.y, CreateFontToHandle("�l�r�S�V�b�N", 25, 2),UI::white);
	UI::Rect uiLevelUpRect(Vector2D<int>(),mSelectRectSize,UI::gray,true);
	mRewards.resize(rewardMaxCount);
	for(int i=0;i<rewardMaxCount;i++){
		mRewards[i] = std::make_shared<UILevelUpReward>(uiLevelUpRect, UI::lightShade, false, CreateFontToHandle("�l�r�S�V�b�N", 26, 2), CreateFontToHandle("�l�r�S�V�b�N", 20, 2));
	}
}

void LevelUpPanel::SetStatusInfoPanel()
{
	mStatusInfoPanel->SetStatusInfo();
}

void LevelUpPanel::SetLevelUpReward(const std::vector<std::shared_ptr<LevelUpBonus>>& rewards)
{
	//������&�ꏊ����
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

	//��AW�L�[�������ꂽ�Ƃ�
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_UP) || Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_W)) {
		if (selectIndex <= 0) {//��ԏ�܂ŗ��Ă����ꍇ�A�J�[�\������ԉ��ɂ���
			mRewards[0]->mIsSelect = false;
			selectIndex = rewardCount -1;
			mRewards[selectIndex]->mIsSelect = true;
		}
		else {//�J�[�\�����P�グ��
			mRewards[selectIndex]->mIsSelect = false;
			selectIndex--;
			mRewards[selectIndex]->mIsSelect = true;
		}
	}
	//���A�r�L�[�������ꂽ�Ƃ�
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_DOWN) ||
		Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_S)) {
		if (selectIndex >= rewardCount - 1) {//��ԉ��܂ŗ��Ă����Ƃ��A�J�[�\������ԏ�ɂ�����
			mRewards[rewardCount - 1]->mIsSelect = false;
			selectIndex = 0;
			mRewards[selectIndex]->mIsSelect = true;
		}
		else {//�J�[�\����1������
			mRewards[selectIndex]->mIsSelect = false;
			selectIndex++;
			mRewards[selectIndex]->mIsSelect = true;
		}
	}

	//�X�y�[�X�L�[�������ꂽ�Ƃ�
	if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_SPACE)) {
		//����
		SelectedReward();
	}
}

void LevelUpPanel::Draw(float deltaTime)
{
	//������`��
	for(auto&x:mUIList){
		x->Draw(deltaTime);
	}
	//�I���\�ȕ�V��`��
	for(int i=0;i<rewardCount;i++){
		mRewards[i]->Draw(deltaTime);
	}
	//���݂̃X�e�[�^�X��\��
	mStatusInfoPanel->Draw(deltaTime);
}

void LevelUpPanel::SelectedReward() const
{
	//��V�̌��ʂ��v���C���[�ɕt�^
	if (selectIndex >= 0 && selectIndex < mRewards.size()) {
		mRewards[selectIndex]->ApplyReward();
		//��V��I�����I�������Ƃ�ʒm����
		SCENE_M.OnLevelUpRewardSelected();
	}
}

void LevelUpPanel::AddUIElement(std::shared_ptr<UIBase> ui)
{
	mUIList.push_back(ui);
}
