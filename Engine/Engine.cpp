#include "Engine.h"
#include "Windows/WindowBase.h"

#include "Systems/Render/RenderSystem.h"
#include "Systems/Physics/PhysicsSystem.h"

#include <vector>

// Define the implementation struct (PImpl - pointer to implementation)
struct Engine::EngineImpl
{
    bool IsRunning = false;
    std::vector<std::unique_ptr<ThreadedTask>> Tasks; // Keeps a ref to all the async tasks
};

// Constructor
Engine::Engine(const WCHAR* InWindowTitle)
    : WindowBase(InWindowTitle), 
    EngineData(new EngineImpl) // Deleted in Engine::~Engine()
{
    CreateThreadedTask<RenderSystem>();
    CreateThreadedTask<PhysicsSystem>();
}

template<DerivedFromThreadedTask T>
void Engine::CreateThreadedTask()
{
    // Emplace back directly constructs the unique_ptr in the vector
    EngineData->Tasks.emplace_back(std::make_unique<T>());
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

void Engine::JoinThreads()
{
    for (const auto& tt : EngineData->Tasks)
    {
        if (tt)
            tt->Join();
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