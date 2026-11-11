#pragma once
#include "object.h"
#include "Core\DataTypes.h"

class Actor : public Object, public FTransform
{
public:
	Actor(Object* Owner, const char* DisplayName);
	~Actor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	Object* GetOwner() { return ObjectData.GetOwner(); }
};