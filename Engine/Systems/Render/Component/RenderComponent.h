#pragma once
#include "Core/Core.h"


class RenderComponent
{
	friend class RenderSystem; // Allows ID access
public:
	RenderComponent();

private:
	GUID_META
};