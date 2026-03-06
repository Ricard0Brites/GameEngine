#include "Engine.h"
#include "Systems/Collision/CollisionSystem.h"
#include "Systems/Physics/PhysicsSystem.h"
#include "Systems/Render/RenderSystem.h"
#include "Windows/WindowBase.h"

// Constructor
Engine::Engine(const WCHAR* InWindowTitle)
    : WindowBase(InWindowTitle), 
    EngineData(new FEngineData) // Deleted in Engine::~Engine()
{

    // Create Base Systems
    CreateThreadedTask<RenderSystem>();
    CreateThreadedTask<PhysicsSystem>();
    CreateThreadedTask<CollisionSystem>();
}

// Destructor
Engine::~Engine()
{
    if (EngineData)
    {
        delete(EngineData);
        EngineData = nullptr;
    }
}

void Engine::Launch()
{
    EngineData->IsRunning = true;
    while (EngineData->IsRunning)
    {
        PumpMessages();
    }
    
    JoinThreads();

    //The program ends here
}

void Engine::Quit()
{
    EngineData->IsRunning = false;
    StopThreads(); // Only stops execution (Destructor handles the rest)
}

void Engine::OnMessageReceived(UINT msg)
{
	// TODO - Input Event Handling
}

void Engine::OnDestroy()
{
    Quit();
}

#pragma region Threaded Tasks

template<DerivedFromThreadedTask T>
void Engine::CreateThreadedTask()
{
    // Emplace back directly constructs the unique_ptr in the vector
    EngineData->Tasks.emplace_back(std::make_unique<T>());
}

void Engine::JoinThreads()
{
    for (const auto& Task : EngineData->Tasks)
    {
        if (Task)
            Task->Join();
    }
}

void Engine::StopThreads()
{
    for (const std::unique_ptr<ThreadedTask> &Task : EngineData->Tasks)
    {
        if (Task)
            Task->StopThread();
    }
}

#pragma endregion