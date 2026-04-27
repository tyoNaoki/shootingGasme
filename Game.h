#pragma once
#include "DxLib.h"
#include "crtdbg.h"
#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK,__FILE__,__LINE__) 
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

class Scene;

class Game
{
private:
	// コピーコンストラクタを禁止する
	Game(const Game&) = delete;
	Game(Game&&) = delete;

	// コピー代入演算子を禁止する
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) = delete;

public:
	Game();
	bool Initialize(); //初期化
	void GameLoop(); //ゲーム更新
	void GameEnd(); //終了処理

private:
	bool mIsRunning = true;
	bool mIsDebug = false;
};

