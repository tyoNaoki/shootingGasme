#include "GameOverPanel.h"
#include "UIText.h"
#include "StatusManager.h"

GameOverPanel::GameOverPanel(const int width,const int height)
{
	Vector2D<float>textCenter(static_cast<float>(width / 2 - 120), static_cast<float>(height / 2) + 50);

	//ゲームオーバーテキスト
	auto gameOver = std::make_shared<UIText>("GameOver", CreateFontToHandle("ＭＳゴシック", 70, -1, DX_FONTTYPE_ANTIALIASING), UI::green);
	gameOver->SetLocalPosition(textCenter - Vector2D<float>(60, 200));

	//プレイヤーのレベル表示
	auto levelText = std::make_shared<UIText>("", CreateFontToHandle("ＭＳゴシック", 30, -1, DX_FONTTYPE_ANTIALIASING), UI::green);
	levelText->SetLocalPosition(textCenter - Vector2D<float>(75, 90));

	//プレイヤーのスコア表示
	auto scoreText = std::make_shared<UIText>("", CreateFontToHandle("ＭＳゴシック", 30, -1, DX_FONTTYPE_ANTIALIASING), UI::green);
	scoreText->SetLocalPosition(textCenter - Vector2D<float>(75, -10));

	//敵を倒した数テキスト
	auto eliminatedText = std::make_shared<UIText>("敵を倒した数", CreateFontToHandle("ＭＳゴシック",40, -1, DX_FONTTYPE_ANTIALIASING), UI::green);
	eliminatedText->SetLocalPosition(textCenter + Vector2D<float>(-30, 120));

	//雑魚敵を倒した数表示
	auto eliminatedNormalEnemyText = std::make_shared<UIText>("", CreateFontToHandle("ＭＳゴシック", 30, -1, DX_FONTTYPE_ANTIALIASING), UI::green);
	eliminatedNormalEnemyText->SetLocalPosition(textCenter + Vector2D<float>(0, 200));

	AddUIElement(gameOver);
	AddUIElement(levelText);
	AddUIElement(scoreText);
	AddUIElement(eliminatedText);
	AddUIElement(eliminatedNormalEnemyText);
}

GameOverPanel::~GameOverPanel()
{
	mUIList.clear();
}

void GameOverPanel::Update(float deltaTime)
{
	if (!IsVisible()) { return; }

	mUIList[1]->SetText("プレイヤーのレベル : " + std::to_string(STATUS.GetPlayerCurrentLevel()));
	mUIList[2]->SetText("プレイヤーのスコア : " + std::to_string(STATUS.GetScore()));
	mUIList[4]->SetText("雑魚 ： " + std::to_string(STATUS.GetCount(CharacterType::ENEMY)) + "体");

	for (auto& x : mUIList) {
		x->Update(deltaTime);
	}
}

void GameOverPanel::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }

	for (auto& x : mUIList) {
		x->Draw(deltaTime);
	}
}

void GameOverPanel::AddUIElement(std::shared_ptr<UIText> ui)
{
	mUIList.push_back(ui);
}
