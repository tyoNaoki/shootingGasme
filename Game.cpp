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

	SetAlwaysRunFlag(TRUE); // �E�B���h�E����A�N�e�B�u��Ԃ̂Ƃ��������𑱍s����
	SetOutApplicationLogValidFlag(FALSE); //Log.txt�𐶐����Ȃ��悤�ɐݒ�
	ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ɐݒ�
	SetGraphMode(1920, 1080, 32); //��ʃT�C�Y�w��
	SetWaitVSyncFlag(0);

	if (!Singleton<GraphicManager>::get_instance().Init()) return false;

	DxLib_Init(); // DX���C�u��������������

	SetDrawScreen(DX_SCREEN_BACK); // �`���𗠉�ʂɂ���

	return true;
}

void Game::GameLoop(){

	//�K�v�ȃ}�l�[�W���[�쐬
	InputManager& input = Singleton<InputManager>::get_instance();
	GameSystemManager& g_sm = Singleton<GameSystemManager>::get_instance();

	//�^�C�g���\��
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

		input.Update(); //���͏���
		g_sm.UpdateDeltaTime();//�f���^�^�C���v�Z
		FpsControll_Update();//FPS��60�ŌŒ�

		ClearDrawScreen();

		float deltaTime = g_sm.GetDeltaTime();
		
<<<<<<< HEAD
		if(mIsDebug)
		{
=======
		if(mIsDebug){
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
			DEBUG_HELPER.Update(deltaTime); //�R���W������f�o�b�O���b�Z�[�W���X�V�A�`��
		}

		mIsRunning = Singleton<SceneManager>::get_instance().Run(deltaTime);
		FpsControll_Draw(); //FPS�\��

		ScreenFlip();

		// 
		//�ҋ@
		FpsControll_Wait();
	}
}

void Game::GameEnd()
{
	SingletonFinalizer::finalize(); //�V���O���g���}�l�[�W���[�I������
}

