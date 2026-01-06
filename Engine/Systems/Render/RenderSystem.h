#pragma once
#include "Core/ThreadedTask.hpp"

class ENGINE_API RenderSystem : public ThreadedTask
{
public:
    RenderSystem();

	void AsyncTick(float Delta) override;

};
