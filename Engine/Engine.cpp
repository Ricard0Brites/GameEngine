#include "Engine.h"
#include <memory>
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
    auto RenderSystemSharedPtr = CreateThreadedTask<RenderSystem>(this);
    OnWindowResizeDelegate.Bind(RenderSystemSharedPtr, &RenderSystem::OnWindowResizedEvent);

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

template<DerivedFromThreadedTask T, typename ...Args>
std::shared_ptr<T> Engine::CreateThreadedTask(Args ...args)
{
    // Emplace back directly constructs the unique_ptr in the vector
    auto NewTask = std::make_shared<T>(args...);
    EngineData->Tasks.emplace_back(NewTask);

    return NewTask;
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
    for (const std::shared_ptr<ThreadedTask> &Task : EngineData->Tasks)
    {
        if (Task)
            Task->StopThread();
    }
}

#pragma endregion