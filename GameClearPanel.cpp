#include "GameClearPanel.h"
#include "UIText.h"
#include "StatusManager.h"

GameClearPanel::GameClearPanel(const int width,const int height)
{
	Vector2D<float>textCenter(static_cast<float>(width / 2 - 120), static_cast<float>(height / 2) + 50);

	//�Q�[���N���A�e�L�X�g
	auto gameClear = std::make_shared<UIText>("GameClear", CreateFontToHandle("�l�r�S�V�b�N", 70, -1, DX_FONTTYPE_ANTIALIASING), UI::green);
	gameClear->SetLocalPosition(textCenter - Vector2D<float>(60,200));

	//�v���C���[�̃��x���\��
	auto levelText = std::make_shared<UIText>("", CreateFontToHandle("�l�r�S�V�b�N", 30, -1, DX_FONTTYPE_ANTIALIASING), UI::green);
	levelText->SetLocalPosition(textCenter - Vector2D<float>(75, 90));

	//�v���C���[�̃X�R�A�\��
	auto scoreText = std::make_shared<UIText>("", CreateFontToHandle("�l�r�S�V�b�N", 30, -1, DX_FONTTYPE_ANTIALIASING), UI::green);
	scoreText->SetLocalPosition(textCenter - Vector2D<float>(75, -10));

	//�G��|�������e�L�X�g
	auto eliminatedText = std::make_shared<UIText>("�G��|������", CreateFontToHandle("�l�r�S�V�b�N", 40, -1, DX_FONTTYPE_ANTIALIASING), UI::green);
	eliminatedText->SetLocalPosition(textCenter + Vector2D<float>(-30, 120));

	//�G���G��|�������\��
	auto eliminatedNormalEnemyText = std::make_shared<UIText>("", CreateFontToHandle("�l�r�S�V�b�N", 30, -1, DX_FONTTYPE_ANTIALIASING), UI::green);
	eliminatedNormalEnemyText->SetLocalPosition(textCenter + Vector2D<float>(0, 200));
	
	//�{�X��|�������\��
	auto eliminatedBossEnemyText = std::make_shared<UIText>("", CreateFontToHandle("�l�r�S�V�b�N", 30, -1, DX_FONTTYPE_ANTIALIASING), UI::green);
	eliminatedBossEnemyText->SetLocalPosition(textCenter + Vector2D<float>(0, 250));

	AddUIElement(gameClear);
	AddUIElement(levelText);
	AddUIElement(scoreText);
	AddUIElement(eliminatedText);
	AddUIElement(eliminatedNormalEnemyText);
	AddUIElement(eliminatedBossEnemyText);
}

GameClearPanel::~GameClearPanel()
{
	mUIList.clear();
}

void GameClearPanel::Update(float deltaTime)
{
	if (!IsVisible()) { return; }
	mUIList[1]->SetText("�v���C���[�̃��x�� : " + std::to_string(STATUS.GetPlayerCurrentLevel()));
	mUIList[2]->SetText("�v���C���[�̃X�R�A : " + std::to_string(STATUS.GetScore()));
	mUIList[4]->SetText("�G�� �F " + std::to_string(STATUS.GetCount(CharacterType::ENEMY)) + "��");
	mUIList[5]->SetText("�{�X :  " + std::to_string(STATUS.GetCount(CharacterType::BOSS)) + "��");

	for (auto& x : mUIList) {
		x->Update(deltaTime);
	}
}

void GameClearPanel::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }

	for (auto& x : mUIList) {
		x->Draw(deltaTime);
	}
}

void GameClearPanel::AddUIElement(std::shared_ptr<UIText> ui)
{
	mUIList.push_back(ui);
}
