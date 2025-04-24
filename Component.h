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

<<<<<<< HEAD
=======
	virtual void SetStatus(std::shared_ptr<WeaponStatus>newStatus){};

>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
	void SetEnable(bool isEnable);

	bool IsEnable()const;

	std::string GetName(){return mComponentName;};
<<<<<<< HEAD

protected:
	std::shared_ptr<CharacterBase> GetOwner() const { return mOwner.lock(); } 
	std::string mComponentName;
	
private:
=======
protected: 
	std::shared_ptr<CharacterBase> GetOwner() const { return mOwner.lock(); } 
	std::string mComponentName;
	
private: 
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
	std::weak_ptr<CharacterBase> mOwner;

	bool mIsEnable = true;
};