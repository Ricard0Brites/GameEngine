//Modules
#include "Engine.h"


//STD
#include <memory>
#include <iostream>

const WCHAR* GameName = L"Space Invaders";
Engine* const EngineInstance = &(Engine::Get(UVector2(1280, 720), *GameName));

int main()
{
	if (!EngineInstance)
	{
		std::cout << "EngineInstance Is Not Valid. Aborting..." << std::endl;
		return 0;
	}
	
	EngineInstance->Launch();
}