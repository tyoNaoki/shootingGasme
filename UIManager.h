#pragma once
#include "Singleton.h"
#include "UIBase.h"
#include "UIText.h"
#include "HPBase.h"
#include "UITime.h"
#include "MenuPanel.h"
#include "HUD.h"
#include "GameClearPanel.h"
#include "GameOverPanel.h"
#include "TitlePanel.h"
#include "MenuScenePanel.h"
#include "LevelUpPanel.h"
#include "Canvas.h"
#include <unordered_map>
#include <vector>

#define UI_M Singleton<UIManager>::get_instance()

class UIManager
{
friend class Singleton<UIManager>;

public:
	//キャンバス追加
	void AddCanvas(std::string name,const std::shared_ptr<Canvas>);
	//対象の名前のキャンバスを取得
	std::shared_ptr<Canvas>GetCanvas(std::string name);
	//更新、描画
	void Update(float deltaTime);
	void Draw(float deltaTime);

	//キャンバスの削除
	void ClearCanvas();
	bool RemoveCanvas(std::string name);
	//キャンバスの表示、非表示
	void SetVisibiltyToCanvas(std::string name,bool isVisibilty);

private:
	~UIManager();

	std::unordered_map<std::string,std::shared_ptr<Canvas>>mCanvasList;
};

