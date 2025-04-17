#include "LaserObjectPool.h"
#include "LaserFactory.h"

void LaserObjectPool::Init()
{
    OnCreate("Laser", laserPoolSize);
}

void LaserObjectPool::OnCreate(const char* name, int count)
{
	for (int i = 0; i < count; i++)
	{
		auto laser = Singleton<LaserFactory>::get_instance().CreateObject(name);
		laser->SetActive(false);
		pool[name].push(laser);
	}
}

std::shared_ptr<Laser> LaserObjectPool::OnGet(const char* name)
{
	auto laser = OnGet(name);

	if (!laser) return nullptr;

	return std::static_pointer_cast<Laser>(laser);
}
