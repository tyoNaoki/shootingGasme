#pragma once
#include "UIBase.h"
#include <unordered_map>

class UIText;

class GameClearPanel :
    public UIBase
{
public:
	//ゲームクリアテキスト、レベル、スコア、撃破数の表示パネルの作成処理
	GameClearPanel(const int width,const int height);
	GameClearPanel() = default;
	~GameClearPanel();

	virtual void Update(float deltaTime) override;
	virtual void Draw(float deltaTime) override;

private:
	void AddUIElement(std::shared_ptr<UIText>ui);

	std::vector<std::shared_ptr<UIText>>mUIList;
};

