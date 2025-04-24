#include "InputManager.h"
#include "DxLib.h"

void InputManager::Update()
{
	char tmpKey[256]; // ���݂̃L�[�̓��͏�Ԃ��i�[����
	GetHitKeyStateAll(tmpKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�
	for (int i = 0; i < 256; i++) {
		if (tmpKey[i] != 0) { // i�Ԃ̃L�[�R�[�h�ɑΉ�����L�[��������Ă�����
			mKey[i]++; // ���Z
		}
		else { // ������Ă��Ȃ����
			mKey[i] = 0; // 0�ɂ���
		}
	}
}

int InputManager::GetPushKeyFrame(int keyID) const
{
	return mKey[keyID];
}

bool InputManager::IsPushKeyNow(int keyID) const
{
	return mKey[keyID] == 1;
}

bool InputManager::IsPushingKey(int keyID) const
{
	return mKey[keyID] > 0;
}

