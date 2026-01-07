#pragma once
#include "Core/Core.h"

class ENGINE_API Object
{
	
public:
	virtual ~Object() = default;
	virtual void BeginPlay() = 0;
	virtual void Tick(float DeltaSeconds) = 0;
};
