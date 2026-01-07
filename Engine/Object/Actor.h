#pragma once
#include "object.h"
#include "Core\DataTypes.h"
#include "Core\Core.h"
#include <vector>

class ENGINE_API Actor : 
	public Object,
	public FTransform
{
public:
	Actor(Actor* Parent);
	Actor(Actor* Parent, const char* DisplayName);
	~Actor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	Actor* GetOwner() { return Owner; }

protected:
	//nullptr id means this object is independent
	Actor* Owner = nullptr;
	//std::vector<std::unique_ptr<Actor>> Children = {};
	bool IsPendingKill = false;

	// Pimpl - Pointer to implementation (DLL linkage warning removal)
	struct FActorData;
	FActorData* Data = nullptr;
};