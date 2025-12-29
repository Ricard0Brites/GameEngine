//Modules
#include "Engine.h"


//STD
#include <memory>
#include <iostream>

int main()
{
	Engine* const EngineInstance = &(Engine::Get());
	if (!EngineInstance)
	{
		std::cout << "EngineInstance Is Not Valid. Aborting..." << std::endl;
		return 0;
	}
	
	EngineInstance->Launch(L"SpaceInvaders");
}