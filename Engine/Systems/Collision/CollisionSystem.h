#pragma once
#include "Core/ThreadedTask.hpp"

class CollisionSystem : public ThreadedTask
{
public:
	CollisionSystem();

	void AsyncTick(float Delta) override;


	void AsyncInit() override;

};
