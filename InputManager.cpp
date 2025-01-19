#include "InputManager.h"
#include "DxLib.h"

void InputManager::Update()
{
	char tmpKey[256]; // 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
	for (int i = 0; i < 256; i++) {
		if (tmpKey[i] != 0) { // i番のキーコードに対応するキーが押されていたら
			mKey[i]++; // 加算
		}
		else { // 押されていなければ
			mKey[i] = 0; // 0にする
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

