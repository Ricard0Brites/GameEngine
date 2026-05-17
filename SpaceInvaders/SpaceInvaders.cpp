//Modules
#include <memory> // shared_ptr
#include "Engine.h"
#include "Core/Object/Actor.h"


int main()
{	
	std::shared_ptr<Engine> EngineInstance = Engine::GetEngineInstance();
	if (!EngineInstance.get())
		__debugbreak();



	EngineInstance->Launch(); // Game Loop

}