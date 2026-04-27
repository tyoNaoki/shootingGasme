#include "TitlePanel.h"
#include "UIText.h"

TitlePanel::TitlePanel(int width, int height)
{
	//タイトル文字の影
	Vector2D<float>center(static_cast<float>(width / 2-270), static_cast<float>(height / 2-100));
	auto titleShadow = std::make_shared<UIText>("DX SURVIVER", CreateFontToHandle("Arial", 80, -1, DX_FONTTYPE_ANTIALIASING), UI::red);
	titleShadow->SetLocalPosition(center);
	AddUIElement(titleShadow);

	//タイトル文字
	auto title = std::make_shared<UIText>("DX SURVIVER", CreateFontToHandle("Arial", 80, -1, DX_FONTTYPE_ANTIALIASING), UI::white);
	title->SetLocalPosition(Vector2D<float>(center.x - 2.0f, center.y - 2.0f));
	AddUIElement(title);

	//スタート文字
	auto startTextPosi = Vector2D<float>(center.x + 70, center.y + 120);
	auto startText = std::make_shared<UIText>("Press SPACE to Start", CreateFontToHandle("Arial", 40, -1, DX_FONTTYPE_ANTIALIASING), UI::red);
	startText->SetLocalPosition(startTextPosi);
	AddUIElement(startText);

	//スタート文字の影
	auto startTextShadow = std::make_shared<UIText>("Press SPACE to Start", CreateFontToHandle("Arial", 40, -1, DX_FONTTYPE_ANTIALIASING), UI::white);
	startTextShadow->SetLocalPosition(Vector2D<float>(startTextPosi.x - 2, startTextPosi.y - 2));
	AddUIElement(startTextShadow);

	//やめる文字
	auto exitTextPosi = Vector2D<float>(30, height - 50);
	auto exitText = std::make_shared<UIText>("ESCAPE / P : ゲームをやめる", CreateFontToHandle("ＭＳゴシック", 30, -1, DX_FONTTYPE_ANTIALIASING), UI::white);
	exitText->SetLocalPosition(exitTextPosi);
	AddUIElement(exitText);
}

TitlePanel::~TitlePanel()
{
	mUIList.clear();
}

void TitlePanel::Update(float deltaTime)
{
	if (!IsVisible()) { return; }
	//更新
	for (auto& x : mUIList) {
		x->Update(deltaTime);
	}
}

void TitlePanel::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }
	//描画
	for (auto& x : mUIList) {
		x->Draw(deltaTime);
	}
}

void TitlePanel::AddUIElement(std::shared_ptr<UIBase> ui)
{
	mUIList.push_back(ui);
}
