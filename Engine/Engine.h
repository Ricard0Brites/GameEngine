#pragma once
#include "Core/Core.h"
#include "Windows/WindowBase.h"
#include <vector>

class ThreadedTask;

template <typename T>
concept DerivedFromThreadedTask = std::is_base_of_v<ThreadedTask, T>;

class ENGINE_API Engine : private WindowBase
{
public:
    Engine(const WCHAR* InWindowTitle);
    ~Engine(); // Destructor must be defined in the .cpp file with PImpl
    
    // Forward-declare the implementation struct
    struct FEngineData;

    void Launch();
    void Quit();

    // Window Base Interface
    void OnMessageReceived(UINT msg) override;
    void OnDestroy() override;
    
    //Threaded Tasks
    template<DerivedFromThreadedTask T>
    void CreateThreadedTask();
private:
    // Remove Heap operations
    void* operator new(size_t) = delete;
    void* operator new[](size_t) = delete;
    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

    // Pointer to the implementation
    FEngineData* EngineData;

    // Threads
    void JoinThreads();
    void StopThreads();
};

// Define the implementation struct (PImpl - pointer to implementation)
struct Engine::FEngineData
{
    bool IsRunning = false;
    
    // Keeps a reference & ownership of all threaded tasks
    std::vector<std::unique_ptr<ThreadedTask>> Tasks;
};
