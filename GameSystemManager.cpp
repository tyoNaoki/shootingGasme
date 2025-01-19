#include "GameSystemManager.h"

void GameSystemManager::UpdateDeltaTime()
{
	//�f���^�^�C���v�Z
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsed = currentTime - previousTime;
	deltaTime = elapsed.count();
	previousTime = currentTime;
}

float GameSystemManager::GetDeltaTime() const
{
	return deltaTime;
}
