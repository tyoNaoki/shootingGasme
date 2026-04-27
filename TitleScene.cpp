#include "TitleScene.h"
#include "UIManager.h"
#include "InputManager.h"
#include "DebugHelper.h"
#include "GraphicManager.h"

TitleScene::TitleScene()
{
	//初期設定
	mSceneName = "Title";
	mPauseEnabled = false;

}

TitleScene::~TitleScene()
{
}

bool TitleScene::Initialize()
{
	if(mInitialze){return true;}

	//画面サイズ取得
	int height,width;
	GetWindowSize(&width,&height);

	//タイトル画面生成
	auto titleCanvas = std::make_shared<Canvas>();
	auto title = std::make_shared<TitlePanel>(width,height);
	titleCanvas->AddElement(title);
	titleCanvas->SetVisibilty(true);

	UI_M.AddCanvas("Title", titleCanvas);
	GRAPHIC_M.ReadHandle(mHandle,"Title","Title");

	//スタート
	ChangeScene(SceneState::PLAYING);

	mInitialze = true;
	return true;
}

void TitleScene::Update(const float deltaTime)
{
	auto &input = Singleton<InputManager>::get_instance();
	//スペースが押されたとき
	if(input.IsPushingKey(KEY_INPUT_SPACE)){
		//次のシーンに遷移
		ChangeScene(SceneState::NEXTSCENE);
	}
	//エスケープ、Pが押されたとき
	if (input.IsPushingKey(KEY_INPUT_ESCAPE)|| input.IsPushingKey(KEY_INPUT_P)) {
		//ゲーム終了
		ChangeScene(SceneState::FINISH);
	}
	//UI更新
	UI_M.Update(deltaTime);
}

void TitleScene::Render(const float deltaTime)
{
	//画面サイズ取得
	int width, height;
	GetWindowSize(&width, &height);

	//タイトル画像描画
	if(mHandle != -1){
		DrawExtendGraph(0, 0, width, height, mHandle, true);
	}
	//UI描画
	UI_M.Draw(deltaTime);
}

std::shared_ptr<Map> TitleScene::GetMap() const
{
	return nullptr;
}

void TitleScene::End()
{
	UI_M.ClearCanvas();
	mInitialze = false;
}
