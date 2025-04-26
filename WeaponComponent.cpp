#include "WeaponComponent.h"
#include "CharacterBase.h"

std::shared_ptr<CharacterBase> WeaponComponent::GetWeaponOwner()
{
    return mWeaponOwner.lock();
}
