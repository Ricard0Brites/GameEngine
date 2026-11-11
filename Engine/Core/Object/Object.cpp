#include "Object.h"
#include "Engine.h"

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

void Object::BeginPlay()
{
	for (const auto& [Guid, Components] : ObjectData.ComponentsCache)
		for (const auto& Comp : Components)
			if (Comp)
				Comp->BeginPlay();
	ObjectData.bHasBeginPlayAlreadyRun = true;
}

void Object::Tick(float DeltaSeconds)
{
	for (const auto& [Guid, Components] : ObjectData.ComponentsCache)
		for (const auto& Comp : Components)
			if (Comp)
				Comp->Tick(DeltaSeconds);
}

#pragma region FObjectData Structure

Object::FObjectData::FObjectData()
{
	// Assign a global id to the object
	
	// Using mutex lock to ensure async safety
	static std::mutex id_mutex;
	std::lock_guard<std::mutex> lock(id_mutex);
	
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

void Object::FObjectData::CacheComponent(const GUID& ClassGUID, std::unique_ptr<ComponentBase> Component)
{
	// Store a raw pointer to the component
	auto* CompRaw = Component.get();

	// Move the unique pointer
	ComponentsCache[ClassGUID].push_back(std::move(Component));
	
	// Execute BeginPlay on component addition if begin play has already been executed.
	if (bHasBeginPlayAlreadyRun && CompRaw)
		CompRaw->BeginPlay();
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
