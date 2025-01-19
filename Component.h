#pragma once
#include <memory>
#include "CharacterBase.h"
#include "WeaponStatus.h"

class Component {
public:
	//
	Component(std::shared_ptr<CharacterBase> owner) : mOwner(owner) {}

	virtual void Init(){;}
	virtual void Update(const float deltaTime) = 0; 
	virtual ~Component() = default;

	virtual void SetStatus(std::shared_ptr<WeaponStatus>newStatus){};

	void SetEnable(bool isEnable);

	bool IsEnable()const;

	std::string GetName(){return mComponentName;};
protected: 
	std::shared_ptr<CharacterBase> GetOwner() const { return mOwner.lock(); } 
	std::string mComponentName;
	
private: 
	std::weak_ptr<CharacterBase> mOwner;

	bool mIsEnable = true;
};