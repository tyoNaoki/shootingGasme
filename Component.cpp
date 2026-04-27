#include "Component.h"

void Component::SetEnable(bool isEnable)
{
	mIsEnable = isEnable;
}

bool Component::IsEnable() const
{
	return mIsEnable;
}
