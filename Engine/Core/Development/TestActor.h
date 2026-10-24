#pragma once

#include "Core/Object/Actor.h"
#include "Systems/Render/Component/RenderComponent.h"

class TestActor : public Actor
{

public:
	TestActor(Object* Owner, const char* DisplayName);
	void BeginPlay() override;

};