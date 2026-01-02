#pragma once

class Object
{
	
public:
	virtual void BeginPlay() = 0;
	virtual void Tick(float DeltaSeconds) = 0;
};
