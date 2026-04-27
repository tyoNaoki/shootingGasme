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

	//ステータスの表記を設定
	void SetStatusInfo();

private:
	StatusInfoPanel() = default;
	//プレイヤーの武器のダメージと攻撃間隔からDPS計算
	float CalculateDPS(float damage,float interval);

	//表記一覧
	std::vector<std::string>infoList;

	UI::Rect mPanel;
	Vector2D<int>mCenter;

	int mFont;
	int mColor;
};

