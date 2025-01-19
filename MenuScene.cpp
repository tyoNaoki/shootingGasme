#include "MenuScene.h"
#include "UIManager.h"
#include "InputManager.h"
#include "GraphicManager.h"
#include "DebugHelper.h"
#include "DxLib.h"

MenuScene::MenuScene()
{
    mSceneName = "Menu";
    mPauseEnabled = false;
}

MenuScene::~MenuScene()
{
}

bool MenuScene::Initialize()
{
    if (mInitialze) { return true; }
	//画面サイズ取得
	int height, width;
	GetWindowSize(&width, &height);

	//タイトル画面生成
	auto menuCanvas = std::make_shared<Canvas>();
	auto menuScenePanel = std::make_shared<MenuScenePanel>(width, height);
	menuCanvas->AddElement(menuScenePanel);
	menuCanvas->SetVisibilty(true);
	UI_M.AddCanvas("Menu", menuCanvas);

	//タイトル画像読み込み
	GRAPHIC_M.ReadHandle(mHandle, "Title", "Title");

	//スタート
	ChangeScene(SceneState::PLAYING);
	
	mInitialze = true;
	return true;
}

void MenuScene::Update(const float deltaTime)
{
	auto& input = Singleton<InputManager>::get_instance();

	//エスケープ、Pが押されたとき、ゲームを終了させる
	if (input.IsPushingKey(KEY_INPUT_ESCAPE) || input.IsPushingKey(KEY_INPUT_P)) {
		ChangeScene(SceneState::FINISH);
		return;
	}

	UI_M.Update(deltaTime);
}

void MenuScene::Render(const float deltaTime)
{
	//画面サイズ取得
	int width, height;
	GetWindowSize(&width, &height);
	//タイトル画像描画
	if (mHandle != -1) {
		DrawExtendGraph(0, 0, width, height, mHandle, true);
	}
	//項目関係を描画
	UI_M.Draw(deltaTime);
}

std::shared_ptr<Map> MenuScene::GetMap() const
{
    return nullptr;
}

void MenuScene::End()
{
    UI_M.ClearCanvas();
    mInitialze = false;
}
