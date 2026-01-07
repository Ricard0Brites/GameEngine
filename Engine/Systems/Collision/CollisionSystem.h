#pragma once
#include "Core/ThreadedTask.hpp"

class ENGINE_API CollisionSystem : public ThreadedTask
{
public:
	CollisionSystem();

	void AsyncTick(float Delta) override;

};
