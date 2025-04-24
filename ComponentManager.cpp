#include "ComponentManager.h"
#include "Component.h"

ComponentManager::~ComponentManager()
{
    ClearComponents();
}

void ComponentManager::InitComponents()
{
    for (auto& component : mComponents) {
        component->Init();
    }
}

void ComponentManager::UpdateComponents(float deltaTime)
{
    for (auto& component : mComponents) {
        component->Update(deltaTime);
    }
}

std::shared_ptr<Component> ComponentManager::GetComponent(int idx)
{
    if (idx >= 0 && idx < mComponents.size())
    {
        return mComponents[idx];
    }

    return nullptr;
}

std::shared_ptr<Component> ComponentManager::GetComponent(std::string name)
{
    for (auto& component : mComponents)
    {
        if (component->GetName() == name)
        {
            return component;
        }
    }

    return nullptr;
}

std::vector<std::shared_ptr<Component>> ComponentManager::GetComponents()
{
    std::vector<std::shared_ptr<Component>> components;

    for (const auto& comp : mComponents) {
        components.push_back(comp);
    }

    return components;
}

void ComponentManager::RemoveComponent(const int idx)
{
    if (idx < 0 || idx >= static_cast<int>(mComponents.size())) {
        return;
    }

    mComponents.erase(mComponents.begin() + idx);
}

void ComponentManager::ClearComponents()
{
    mComponents.clear();
}
