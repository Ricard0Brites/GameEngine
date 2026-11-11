#pragma once

#include "Core/Object/Actor.h"

class TestActor : public Actor
{

public:
	TestActor(Object* Owner, const char* DisplayName);
	
	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

};