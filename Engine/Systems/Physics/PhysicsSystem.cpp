#include "PhysicsSystem.h"
#include <iostream>

PhysicsSystem::PhysicsSystem() : ThreadedTask()
{

}

void PhysicsSystem::AsyncTick(float Delta)
{
	std::cout << Delta << std::endl;
}
