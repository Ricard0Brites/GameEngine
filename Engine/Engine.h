#pragma once
#include "Core/Core.h"
#include "Windows/WindowBase.h"

class ThreadedTask;

template <typename T>
concept DerivedFromThreadedTask = std::is_base_of_v<ThreadedTask, T>;

class ENGINE_API Engine : private WindowBase
{
public:
    Engine(const WCHAR* InWindowTitle);
    ~Engine(); // Destructor must be defined in the .cpp file with PImpl
    
    // Forward-declare the implementation struct
    struct EngineImpl;

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

    // The pointer to the implementation (Pimpl is used to resolve dependency linkage issues)
    EngineImpl* EngineData;

    // Threads
    void JoinThreads();
    void StopThreads();
};



