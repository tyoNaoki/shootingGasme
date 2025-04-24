#pragma once
#include <chrono>
#include "Singleton.h"

class GameSystemManager
{
friend class Singleton<GameSystemManager>;

public:
	void UpdateDeltaTime();//デルタタイム計算

	float GetDeltaTime() const;//取得

private:
	GameSystemManager() : previousTime(std::chrono::steady_clock::now()), deltaTime(0.0f){} 

	std::chrono::steady_clock::time_point previousTime;//前の時間
	float deltaTime;//デルタタイム

	GameSystemManager(const GameSystemManager&) = delete;
	GameSystemManager& operator=(const GameSystemManager&) = delete;
};

