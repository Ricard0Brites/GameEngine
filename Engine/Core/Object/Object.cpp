#include "Object.h"

Object::Object(Object* InOwner)
{
	ObjectData = std::make_unique<FObjectData>();
	ObjectData->SetOwner(InOwner);
}

Object::Object(Object* InOwner, const char* InDisplayName) : Object(InOwner)
{
	ObjectData->SetDisplayName(InDisplayName);
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

bool Object::FObjectData::GetIsPendingKill()
{
	return (LifecycleState & ELifeCycleState::PendingKill) != 0;
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
	LifecycleState = ELifeCycleState::PendingKill;
}

#pragma endregion

template<typename T>
requires std::is_base_of_v<Object, T>
std::shared_ptr<T> Object::FObjectData::CreateChildOfClass()
{
	std::shared_ptr<T> tmp = std::make_shared<T>();
	Engine::Objects.push_back(tmp);
	return tmp;
}

#pragma endregion
