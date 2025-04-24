#pragma once
#include<map>
#include "UIBase.h"

struct StatusInfo {
	int level;
	std::string text;
	float value;
};

class StatusInfoPanel :
    public UIBase
{
public:
	StatusInfoPanel(const int width, const int height,int font,int color);
	~StatusInfoPanel();

	void Update(float deltaTime) override;
	void Draw(float deltaTime) override;

	//�X�e�[�^�X�̕\�L��ݒ�
	void SetStatusInfo();

private:
	StatusInfoPanel() = default;
	//�v���C���[�̕���̃_���[�W�ƍU���Ԋu����DPS�v�Z
	float CalculateDPS(float damage,float interval);

	//�\�L�ꗗ
	std::vector<std::string>infoList;

	UI::Rect mPanel;
	Vector2D<int>mCenter;

	int mFont;
	int mColor;
};

