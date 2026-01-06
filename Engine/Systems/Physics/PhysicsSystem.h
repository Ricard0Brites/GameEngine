#pragma once
#include "Core/ThreadedTask.hpp"

class ENGINE_API PhysicsSystem : public ThreadedTask
{
public:
    PhysicsSystem();

	void AsyncTick(float Delta) override;

};
