#pragma once
#include "Core/Core.h"

class ComponentBase
{
public:
	ComponentBase() = default;
	
	virtual void BeginPlay() = 0;
	virtual void Tick(float DeltaSeconds) = 0;

private:
	GUID_META
};