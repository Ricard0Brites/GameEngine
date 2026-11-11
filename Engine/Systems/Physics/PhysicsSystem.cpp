#include "PhysicsSystem.h"
#include <iostream>

PhysicsSystem::PhysicsSystem() : ThreadedTask("Physics Thread")
{

}

void PhysicsSystem::AsyncTick(float Delta)
{
	//std::cout << Delta << std::endl;
}

void PhysicsSystem::AsyncInit()
{
}
