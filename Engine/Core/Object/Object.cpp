#include "Object.h"
#include "Engine.h"

#pragma region Static Implementations

uint64_t Object::FObjectData::GlobalIDCounter = 0;

#pragma endregion

Object::Object(Object* InOwner)
{
	ObjectData.OnInitializedStarted();
	ObjectData.SetOwner(InOwner);
}

Object::Object(Object* InOwner, const char* InDisplayName) : Object(InOwner)
{
	ObjectData.SetDisplayName(InDisplayName);
	ObjectData.OnInitializedFinished();
}

#pragma region FObjectData Structure

Object::FObjectData::FObjectData()
{
	// Assign a global id to the object
	
	// Using mutex lock to ensure async safety
	static std::mutex id_mutex;
	std::lock_guard<std::mutex> lock(id_mutex);
	GlobalID = GlobalIDCounter++;
	
}

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

uint64_t Object::FObjectData::GetGID()
{
	return GlobalID;
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

void Object::FObjectData::OnInitializedStarted()
{
	LifecycleState = ELifeCycleState::Initializing;
}

void Object::FObjectData::OnInitializedFinished()
{
	LifecycleState = ELifeCycleState::Initialized;
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
