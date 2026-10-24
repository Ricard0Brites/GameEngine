#pragma once
#include "Systems/Base/ComponentBase.h"

class RenderComponent : public ComponentBase<EEngineSystem::Render>
{
	friend class RenderSystem; // Allows ID access
public:
	RenderComponent(ERenderMode RenderMode);

private:
	ERenderMode RenderMode = ERenderMode::None;

};
