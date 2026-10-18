//Modules
#include <memory> // shared_ptr
#include "Engine.h"
#include "Core/Object/Actor.h"
#include "Core/Development/TestActor.h"


int main()
{	
	std::shared_ptr<Engine> EngineInstance = Engine::GetEngineInstance();
	if (!EngineInstance.get())
		__debugbreak();

	std::shared_ptr<TestActor> Tester = EngineInstance->CreateActor<TestActor>(nullptr, "Test Actor 1");


	EngineInstance->Launch(); // Game Loop

}