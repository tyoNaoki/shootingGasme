#pragma once
#include "Scene.h"
#include "Vector2D.h"

class TitleScene :
    public Scene
{
public:
	TitleScene();

	~TitleScene();
	//初期化
	bool Initialize() override;
	//更新、描画
	void Update(const float deltaTime) override;
	void Render(const float deltaTime) override;
	//現在のマップ取得
	std::shared_ptr<Map>GetMap() const override;
	//終了処理
	void End() override;

private:
	bool mInitialze = false;

	int mHandle = -1;
};

