#include "Object.h"

Object::Object(Object* InOwner)
{
	ObjectData = new FObjectData;
	ObjectData->SetOwner(InOwner);
}

Object::Object(Object* InOwner, const char* InDisplayName) : Object(InOwner)
{
	ObjectData->SetDisplayName(InDisplayName);
}

Object::~Object()
{
	if (ObjectData)
	{
		delete ObjectData;
		ObjectData = nullptr;
	}
}

#pragma region FObjectData Structure

#pragma region Getters

const std::string& Object::FObjectData::GetDisplayName()
{
	return DisplayName;
}

Object* Object::FObjectData::GetOwner()
{
	return Owner;
}

const bool& Object::FObjectData::GetIsPendingKill()
{
	return IsPendingKill;
}

#pragma endregion

#pragma region Setters

void Object::FObjectData::SetDisplayName(const char* InName)
{
	DisplayName = InName;
}

void Object::FObjectData::SetOwner(Object* InOwner)
{
	Owner = InOwner;
}

void Object::FObjectData::SetPendingKill()
{
	IsPendingKill = true;
}

#pragma endregion

template<DerivedFromObject T>
T* Object::FObjectData::CreateChildOfClass()
{
	return nullptr;
}

#pragma endregion
