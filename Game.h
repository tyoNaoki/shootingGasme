#pragma once
#include "DxLib.h"
#include "crtdbg.h"
#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK,__FILE__,__LINE__) 
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

class Scene;

class Game
{
private:
	// �R�s�[�R���X�g���N�^���֎~����
	Game(const Game&) = delete;
	Game(Game&&) = delete;

	// �R�s�[������Z�q���֎~����
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) = delete;

public:
	Game();
	bool Initialize(); //������
	void GameLoop(); //�Q�[���X�V
	void GameEnd(); //�I������

private:
	bool mIsRunning = true;
	bool mIsDebug = false;
};

