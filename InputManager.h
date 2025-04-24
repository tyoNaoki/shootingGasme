#pragma once
#include "Singleton.h"

class InputManager
{
friend class Singleton<InputManager>;

public:
	// キー入力状態を更新する
	void Update();

	// 指定したキーが押されているフレーム数を取得する
	int GetPushKeyFrame(int keyID) const;

	// 指定したキーが直近に押されたかを取得する
	bool IsPushKeyNow(int keyID) const;

	// 指定したキーが現在押されているかを取得する
	bool IsPushingKey(int keyID) const;

private:
	InputManager() = default;
	~InputManager() = default;

	int mKey[256] = {}; // 全てのキーが押されているフレーム数
};

