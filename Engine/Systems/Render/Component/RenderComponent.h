#pragma once
#include <stdint.h>


class RenderComponent
{
	friend class RenderSystem; // To allow ID access
public:
	RenderComponent(uint64_t GID);

private:
	uint64_t GID = -1;
};