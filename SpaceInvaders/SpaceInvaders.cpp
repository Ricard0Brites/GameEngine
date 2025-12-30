//Modules
#include "Engine.h"

int main()
{
	Engine* const EngineInstance = new Engine(L"Space Invaders");
	if (!EngineInstance)
	{
		std::cout << "EngineInstance Is Not Valid. Aborting..." << std::endl;
		return 0;
	}
	
	EngineInstance->Launch();
}