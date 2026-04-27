#pragma once
#include "UIBase.h"
#include <unordered_map>

class UIText;

class GameOverPanel :
    public UIBase
{
public:
	//ゲームオーバーテキスト、レベル、スコア、撃破数の表示パネルの作成処理
	GameOverPanel(const int width,const int height);
	
	~GameOverPanel();
	//更新、描画
	void Update(float deltaTime) override;
	void Draw(float deltaTime) override;

private:
	//UI追加
	void AddUIElement(std::shared_ptr<UIText>ui);
	//UIリスト
	std::vector<std::shared_ptr<UIText>>mUIList;
};

