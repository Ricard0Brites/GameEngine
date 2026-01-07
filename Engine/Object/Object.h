#pragma once
#include "Core/Core.h" // DLL, iostream
#include <type_traits>

class ENGINE_API Object
{
	
public:
	Object(Object* InOwner);
	Object(Object* InOwner, const char* InDisplayName);
	virtual ~Object();
	virtual void BeginPlay() = 0;
	virtual void Tick(float DeltaSeconds) = 0;

protected:
	// Pimpl - Pointer to implementation (DLL linkage warning removal)
	struct FObjectData;
	FObjectData* ObjectData = nullptr;
};

template<typename T>
concept DerivedFromObject = std::is_base_of_v(Object, T);

struct Object::FObjectData
{
public:
#pragma region Getters

	const std::string& GetDisplayName();
	Object* GetOwner();
	const bool& GetIsPendingKill();

	// TODO - Add as needed

#pragma endregion

#pragma region Setters
	void SetDisplayName(const char* InName);
	void SetOwner(Object *InOwner);
	void SetPendingKill();

	template<DerivedFromObject T>
	T* CreateChildOfClass();
#pragma endregion

private:
	// Name of the object
	std::string DisplayName = "";

	// List of children (We own them via unique ptr)
	std::vector<std::unique_ptr<Object>> Children = {};

	//nullptr id means this object is independent
	Object* Owner = nullptr;

	// Defines if the object is going to be destroyed or not (TODO - WIP GC)
	bool IsPendingKill = false;
};