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
	//��ʃT�C�Y�擾
	int height, width;
	GetWindowSize(&width, &height);

	//�^�C�g����ʐ���
	auto menuCanvas = std::make_shared<Canvas>();
	auto menuScenePanel = std::make_shared<MenuScenePanel>(width, height);
	menuCanvas->AddElement(menuScenePanel);
	menuCanvas->SetVisibilty(true);
	UI_M.AddCanvas("Menu", menuCanvas);

	//�^�C�g���摜�ǂݍ���
	GRAPHIC_M.ReadHandle(mHandle, "Title", "Title");

	//�X�^�[�g
	ChangeScene(SceneState::PLAYING);
	
	mInitialze = true;
	return true;
}

void MenuScene::Update(const float deltaTime)
{
	auto& input = Singleton<InputManager>::get_instance();

	//�G�X�P�[�v�AP�������ꂽ�Ƃ��A�Q�[�����I��������
	if (input.IsPushingKey(KEY_INPUT_ESCAPE) || input.IsPushingKey(KEY_INPUT_P)) {
		ChangeScene(SceneState::FINISH);
		return;
	}

	UI_M.Update(deltaTime);
}

void MenuScene::Render(const float deltaTime)
{
	//��ʃT�C�Y�擾
	int width, height;
	GetWindowSize(&width, &height);
	//�^�C�g���摜�`��
	if (mHandle != -1) {
		DrawExtendGraph(0, 0, width, height, mHandle, true);
	}
	//���ڊ֌W��`��
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
