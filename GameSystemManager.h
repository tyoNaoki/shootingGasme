#pragma once
#include <chrono>
#include "Singleton.h"

class GameSystemManager
{
friend class Singleton<GameSystemManager>;

public:
	void UpdateDeltaTime();//�f���^�^�C���v�Z

	float GetDeltaTime() const;//�擾

private:
	GameSystemManager() : previousTime(std::chrono::steady_clock::now()), deltaTime(0.0f){} 

	std::chrono::steady_clock::time_point previousTime;//�O�̎���
	float deltaTime;//�f���^�^�C��

	GameSystemManager(const GameSystemManager&) = delete;
	GameSystemManager& operator=(const GameSystemManager&) = delete;
};

