#pragma once
#include "Core/ThreadedTask.hpp"

class PhysicsSystem : public ThreadedTask
{
public:
    PhysicsSystem();

	void AsyncTick(float Delta) override;
	void AsyncInit() override;

};
