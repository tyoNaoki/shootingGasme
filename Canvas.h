#pragma once
#include <vector>
#include <unordered_map>
#include <memory>

class UIBase;

class Canvas
{
private:
	std::vector<std::shared_ptr<UIBase>>elements;

public:
	~Canvas();

	void AddElement(std::shared_ptr<UIBase>element);

	std::vector<std::shared_ptr<UIBase>> GetElements();

	void Draw(float deltaTime);

	void Update(float deltaTime);

	void SetVisibilty(bool isVisible);

private:
	bool mIsVisible = false;
};

