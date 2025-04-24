#include "Game.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "ActorManager.h"
#include "PlayerFactory.h"
#include "GraphicManager.h"
#include "GameSystemManager.h"
#include "FpsControll.h"
#include "DebugHelper.h"

Game::Game(){
	
}

bool Game::Initialize(){

	SetAlwaysRunFlag(TRUE); // ウィンドウが非アクティブ状態のときも処理を続行する
	SetOutApplicationLogValidFlag(FALSE); //Log.txtを生成しないように設定
	ChangeWindowMode(TRUE); // ウィンドウモードに設定
	SetGraphMode(1920, 1080, 32); //画面サイズ指定
	SetWaitVSyncFlag(0);

	if (!Singleton<GraphicManager>::get_instance().Init()) return false;

	DxLib_Init(); // DXライブラリ初期化処理

	SetDrawScreen(DX_SCREEN_BACK); // 描画先を裏画面にする

	return true;
}

void Game::GameLoop(){

	//必要なマネージャー作成
	InputManager& input = Singleton<InputManager>::get_instance();
	GameSystemManager& g_sm = Singleton<GameSystemManager>::get_instance();

	//タイトル表示
	GRAPHIC_M.CreateGraphic("Title");

	while (mIsRunning)
	{
<<<<<<< HEAD
		if(ProcessMessage()!=0)
		{
=======
		if(ProcessMessage()!=0){
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
			mIsRunning = false;
			continue;
		}

		input.Update(); //入力処理
		g_sm.UpdateDeltaTime();//デルタタイム計算
		FpsControll_Update();//FPSを60で固定

		ClearDrawScreen();

		float deltaTime = g_sm.GetDeltaTime();
		
<<<<<<< HEAD
		if(mIsDebug)
		{
=======
		if(mIsDebug){
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
			DEBUG_HELPER.Update(deltaTime); //コリジョンやデバッグメッセージを更新、描画
		}

		mIsRunning = Singleton<SceneManager>::get_instance().Run(deltaTime);
		FpsControll_Draw(); //FPS表示

		ScreenFlip();

		// 
		//待機
		FpsControll_Wait();
	}
}

void Game::GameEnd()
{
	SingletonFinalizer::finalize(); //シングルトンマネージャー終了処理
}

