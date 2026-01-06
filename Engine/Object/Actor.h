#pragma once
#include "object.h"
#include "Core\DataTypes.h"

class FTransform;
class Vector;

class Actor : 
	public Object,
	public FTransform
{
public:
	Actor(Actor* Parent);
	Actor(Actor* Parent, const char* DisplayName);
	~Actor();

#ifdef _DEBUG
	std::string ActorDisplayName = "";
#endif // _DEBUG

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	Object* GetOwner() { return Owner; }

protected:

	//nullptr id means this object is independent
	Object* Owner = nullptr;
	bool IsPendingKill = false;
};