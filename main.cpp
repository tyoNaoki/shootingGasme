#define _CRTDBG_MAP_ALLOC

#include "Game.h"
#include <crtdbg.h>

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |  _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(399);

	Game game;
	if (game.Initialize())		// �c�w���C�u��������������
	{
		game.GameLoop();		// �G���[���N�����璼���ɏI��
	}

	game.GameEnd();

	DxLib_End();

	//_CrtDumpMemoryLeaks();

	return 0;				// �\�t�g�̏I�� 
}