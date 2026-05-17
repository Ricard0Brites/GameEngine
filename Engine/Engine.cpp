#include "Engine.h"
#include <memory>
#include "Windows/Spawnable/SpawnableWindow.h"
#include "Systems/Collision/CollisionSystem.h"
#include "Systems/Physics/PhysicsSystem.h"
#include "Systems/Render/RenderSystem.h"

void Engine::Init()
{
    if (!EngineInstance.get())
        __debugbreak();

#pragma region Spawn Window

    EngineData.Window = std::make_shared<SpawnableWindow>();

    if (SpawnableWindow* w = EngineData.Window.get())
        w->OnWindowDestroyedDelegate.Bind(EngineInstance, &Engine::Quit);

#pragma endregion

#pragma region Spawn Systems

    // Create Base Systems
    CreateThreadedTask<RenderSystem>(EngineData.Window);
    CreateThreadedTask<PhysicsSystem>();
    CreateThreadedTask<CollisionSystem>();

#pragma endregion
}

void Engine::Launch()
{
    Init();

    EngineData.IsRunning = true;
    while (EngineData.IsRunning)
    {
        if (EngineData.Window.get())
            EngineData.Window->PumpMessages();
    }

    Quit();
}

void Engine::Quit()
{
    EngineData.IsRunning = false;
    StopThreads();
}

#pragma region Singleton

std::shared_ptr<Engine> Engine::GetEngineInstance()
{
    if (EngineInstance.get())
        return EngineInstance;
    EngineInstance = std::make_shared < Engine >();
    return EngineInstance;
}

std::shared_ptr<Engine> Engine::EngineInstance = nullptr;

#pragma endregion

#pragma region Threaded Tasks

template<DerivedFromThreadedTask T, typename ...Args>
std::shared_ptr<T> Engine::CreateThreadedTask(Args ...args)
{
    // Emplace back directly constructs the unique_ptr in the vector
    std::shared_ptr<T> NewTask = std::make_shared<T>(args...);
    EngineData.Tasks.push_back(NewTask);

    return NewTask;
}

void Engine::StopThreads()
{
    for (const std::shared_ptr<ThreadedTask>& Task : EngineData.Tasks)
    {
        if (Task)
            Task->StopThread();
    }
    EngineData.Tasks.clear();
}

#pragma endregion

#pragma region Objects

std::vector<std::shared_ptr<Actor>> Engine::ObjectCache = {}; // Initialize empty

#pragma endregion