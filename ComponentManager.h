#pragma once
#include <vector>
#include <memory>
#include <string>

class Component;

class ComponentManager
{
public:
    ~ComponentManager();

    template <typename CompType>
    std::weak_ptr<CompType> AddComponent(std::shared_ptr<CompType>component, int idx)
    {
        if (idx < 0 || idx > static_cast<int>(mComponents.size())) {
            return std::weak_ptr<CompType>();
        }

        mComponents.insert(mComponents.begin() + idx,component);
        return std::weak_ptr<CompType>(component);
    }

    template<typename compType, typename... Args>
    std::weak_ptr<compType> AddComponent(int idx, Args&&... args)
    {
        return AddComponent(std::make_shared<compType>(std::forward<Args>(args)...), idx);
    };

    void InitComponents();

    void UpdateComponents(float deltaTime);

    std::shared_ptr<Component>GetComponent(int idx);

    std::shared_ptr<Component>GetComponent(std::string name);

    template <typename T>
    std::shared_ptr<T> GetComponent()
    {
        for (auto& component : mComponents)
        {
            if (auto result = std::dynamic_pointer_cast<T>(component))
            {
                return result;
            }
        }

        return nullptr;
    }

    std::vector<std::shared_ptr<Component>>GetComponents();

    void RemoveComponent(const int idx);

    void ClearComponents();

private:
    std::vector<std::shared_ptr<Component>> mComponents;
};



