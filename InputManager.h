#pragma once
#include "Singleton.h"

class InputManager
{
friend class Singleton<InputManager>;

public:
	// �L�[���͏�Ԃ��X�V����
	void Update();

	// �w�肵���L�[��������Ă���t���[�������擾����
	int GetPushKeyFrame(int keyID) const;

	// �w�肵���L�[�����߂ɉ����ꂽ�����擾����
	bool IsPushKeyNow(int keyID) const;

	// �w�肵���L�[�����݉�����Ă��邩���擾����
	bool IsPushingKey(int keyID) const;

private:
	InputManager() = default;
	~InputManager() = default;

	int mKey[256] = {}; // �S�ẴL�[��������Ă���t���[����
};

