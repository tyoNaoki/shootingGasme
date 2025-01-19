#include "TitlePanel.h"
#include "UIText.h"

TitlePanel::TitlePanel(int width, int height)
{
	//�^�C�g�������̉e
	Vector2D<float>center(static_cast<float>(width / 2-270), static_cast<float>(height / 2-100));
	auto titleShadow = std::make_shared<UIText>("DX SURVIVER", CreateFontToHandle("Arial", 80, -1, DX_FONTTYPE_ANTIALIASING), UI::red);
	titleShadow->SetLocalPosition(center);
	AddUIElement(titleShadow);

	//�^�C�g������
	auto title = std::make_shared<UIText>("DX SURVIVER", CreateFontToHandle("Arial", 80, -1, DX_FONTTYPE_ANTIALIASING), UI::white);
	title->SetLocalPosition(Vector2D<float>(center.x - 2.0f, center.y - 2.0f));
	AddUIElement(title);

	//�X�^�[�g����
	auto startTextPosi = Vector2D<float>(center.x + 70, center.y + 120);
	auto startText = std::make_shared<UIText>("Press SPACE to Start", CreateFontToHandle("Arial", 40, -1, DX_FONTTYPE_ANTIALIASING), UI::red);
	startText->SetLocalPosition(startTextPosi);
	AddUIElement(startText);

	//�X�^�[�g�����̉e
	auto startTextShadow = std::make_shared<UIText>("Press SPACE to Start", CreateFontToHandle("Arial", 40, -1, DX_FONTTYPE_ANTIALIASING), UI::white);
	startTextShadow->SetLocalPosition(Vector2D<float>(startTextPosi.x - 2, startTextPosi.y - 2));
	AddUIElement(startTextShadow);

	//��߂镶��
	auto exitTextPosi = Vector2D<float>(30, height - 50);
	auto exitText = std::make_shared<UIText>("ESCAPE / P : �Q�[������߂�", CreateFontToHandle("�l�r�S�V�b�N", 30, -1, DX_FONTTYPE_ANTIALIASING), UI::white);
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
	//�X�V
	for (auto& x : mUIList) {
		x->Update(deltaTime);
	}
}

void TitlePanel::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }
	//�`��
	for (auto& x : mUIList) {
		x->Draw(deltaTime);
	}
}

void TitlePanel::AddUIElement(std::shared_ptr<UIBase> ui)
{
	mUIList.push_back(ui);
}
