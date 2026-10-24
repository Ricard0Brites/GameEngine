#pragma once
#include "Systems/Base/ComponentBase.h"


class RenderComponent : ComponentBase
{
	friend class RenderSystem; // Allows ID access
public:
	RenderComponent();
};