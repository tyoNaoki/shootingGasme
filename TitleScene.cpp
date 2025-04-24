#include "TitleScene.h"
#include "UIManager.h"
#include "InputManager.h"
#include "DebugHelper.h"
#include "GraphicManager.h"

TitleScene::TitleScene()
{
	//�����ݒ�
	mSceneName = "Title";
	mPauseEnabled = false;

}

TitleScene::~TitleScene()
{
}

bool TitleScene::Initialize()
{
	if(mInitialze){return true;}

	//��ʃT�C�Y�擾
	int height,width;
	GetWindowSize(&width,&height);

	//�^�C�g����ʐ���
	auto titleCanvas = std::make_shared<Canvas>();
	auto title = std::make_shared<TitlePanel>(width,height);
	titleCanvas->AddElement(title);
	titleCanvas->SetVisibilty(true);

	UI_M.AddCanvas("Title", titleCanvas);
	GRAPHIC_M.ReadHandle(mHandle,"Title","Title");

	//�X�^�[�g
	ChangeScene(SceneState::PLAYING);

	mInitialze = true;
	return true;
}

void TitleScene::Update(const float deltaTime)
{
	auto &input = Singleton<InputManager>::get_instance();
	//�X�y�[�X�������ꂽ�Ƃ�
	if(input.IsPushingKey(KEY_INPUT_SPACE)){
		//���̃V�[���ɑJ��
		ChangeScene(SceneState::NEXTSCENE);
	}
	//�G�X�P�[�v�AP�������ꂽ�Ƃ�
	if (input.IsPushingKey(KEY_INPUT_ESCAPE)|| input.IsPushingKey(KEY_INPUT_P)) {
		//�Q�[���I��
		ChangeScene(SceneState::FINISH);
	}
	//UI�X�V
	UI_M.Update(deltaTime);
}

void TitleScene::Render(const float deltaTime)
{
	//��ʃT�C�Y�擾
	int width, height;
	GetWindowSize(&width, &height);

	//�^�C�g���摜�`��
	if(mHandle != -1){
		DrawExtendGraph(0, 0, width, height, mHandle, true);
	}
	//UI�`��
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
