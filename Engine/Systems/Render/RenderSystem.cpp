#include "RenderSystem.h"
#include <iostream>


RenderSystem::RenderSystem() : ThreadedTask()
{

}

void RenderSystem::AsyncTick(float Delta)
{
	std::cout << "printing from Render System: " << Delta << std::endl;
}
