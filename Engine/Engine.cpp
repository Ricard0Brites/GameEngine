#include "Engine.h"
#include "Windows/WindowBase.h"

#include "Systems/Render/RenderSystem.h"
#include "Systems/Physics/PhysicsSystem.h"

#include <vector>

// Define the implementation struct (PImpl - pointer to implementation)
struct Engine::EngineImpl
{
    bool IsRunning = false;
    std::vector<ThreadedTask*> Tasks; // Keeps a ref to all the async tasks
};

// Constructor
Engine::Engine(const WCHAR* InWindowTitle)
    : WindowBase(InWindowTitle), 
    Pimpl(new EngineImpl) // Deleted in Engine::~Engine()
{
    Pimpl->Tasks.push_back(new RenderSystem);   // Deleted in Engine::~Engine()
    Pimpl->Tasks.push_back(new PhysicsSystem);  // Deleted in Engine::~Engine()
}

// Destructor
Engine::~Engine()
{
    DestroyThreadedTasks();

    if (Pimpl)
    {
        delete(Pimpl);
        Pimpl = nullptr;
    }
}

void Engine::Launch()
{
    Pimpl->IsRunning = true;
    while (Pimpl->IsRunning)
    {
        PumpMessages();
    }
    
    JoinThreads();

    //The program ends here
}

void Engine::Quit()
{
    Pimpl->IsRunning = false;
    // The destructor will handle calling End() on tasks
}

void Engine::OnMessageReceived(UINT msg)
{
	// TODO
}

void Engine::OnDestroy()
{
    Quit();
}

void Engine::DestroyThreadedTasks()
{
    // Gracefully end all tasks before destruction
    for (ThreadedTask* Task : Pimpl->Tasks)
    {
        if (!Task)
            continue;

        Task->End();
        delete(Task);
    }
}

void Engine::JoinThreads()
{
    for (ThreadedTask* tt : Pimpl->Tasks)
    {
        if (tt)
            tt->Join();
    }
}
