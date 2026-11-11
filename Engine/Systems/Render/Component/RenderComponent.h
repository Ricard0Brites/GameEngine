#pragma once
#include "Core/Core.h"
#include "Core/ComponentBase.h"

class RenderComponent : public ComponentBase
{
	friend class RenderSystem; // Allows ID access
public:
	RenderComponent(ERenderMode RenderMode);


	void BeginPlay() override;


	void Tick(float DeltaSeconds) override;

private:
	ERenderMode RenderMode = ERenderMode::None;

	GUID_META
};