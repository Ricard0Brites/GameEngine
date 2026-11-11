//Modules
#include <memory> // shared_ptr
#include "Engine.h"

#include "Core/Development/TestActor.h" // Development Only... DELETE LATER
#include "Systems/Render/Component/RenderComponent.h" 

int main()
{	
	std::shared_ptr<Engine> EngineInstance = Engine::GetEngineInstance();
	if (!EngineInstance.get())
		__debugbreak();

	std::shared_ptr<TestActor> Tester = EngineInstance->CreateActor<TestActor>(nullptr, "Test Actor 1");
	Tester->AddComponent<RenderComponent>(ERenderMode::World);

	EngineInstance->Launch(); // Game Loop

}