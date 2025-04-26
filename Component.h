#pragma once
#include <memory>
#include <string>

class Actor;

class Component {
public:
	Component(std::shared_ptr<Actor> owner) : mOwner(owner) {}

	virtual void Init(){;}
	virtual void Update(const float deltaTime) = 0; 
	virtual ~Component() = default;

	void SetEnable(bool isEnable);

	bool IsEnable()const;

	std::string GetName(){return mComponentName;};

protected:
	std::shared_ptr<Actor> GetOwner() const { return mOwner.lock(); } 
	std::string mComponentName;
	
private:
	std::weak_ptr<Actor> mOwner;

	bool mIsEnable = true;
};