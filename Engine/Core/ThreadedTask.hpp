#pragma once
#include "Core.h"
#include <chrono>
#include <thread>

class ThreadedTask
{
    friend class Engine; // The only one allowed to call ThreadedTask::StopThread()
public:
    #pragma region Exposed Task Events

    virtual void AsyncTick(float Delta) = 0;
    virtual void AsyncInit() = 0;

    #pragma endregion
    
    ThreadedTask() : TaskData(std::make_unique<FData>())
    {
        Init();
    }

    ~ThreadedTask()
    {
        Join(); // Join the thread before self is destroyed
    }

private:

    // Internal synchronous execution -> Parent thread
    void Init()
    {
        TaskData->IsRunning = true;
        TaskData->Thread = std::make_unique<std::thread>(&ThreadedTask::Async_Init, this);
    }

    // Internal asynchronous loop -> New Thread
    void Async_Init()
    {
        AsyncInit();

        // Tick
        std::chrono::steady_clock::time_point PreviousFrame = std::chrono::high_resolution_clock::now();
        while (IsRunning())
        {
            std::chrono::steady_clock::time_point CurrentFrame = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> DeltaTime = CurrentFrame - PreviousFrame;
            PreviousFrame = CurrentFrame;

            AsyncTick(DeltaTime.count());
        }
    }
    
    inline bool IsRunning() 
    {
        return TaskData->IsRunning; 
    };
    
    inline void StopThread() 
    { 
        TaskData->IsRunning = false;  
    }
    
    inline void Join()
    {
        if(TaskData.get() && TaskData->Thread.get())
            TaskData->Thread->join();
    }

    #pragma region Pointer To Implementation
    
    struct FData
    {
        std::unique_ptr<std::thread> Thread = nullptr; // Friend declaration allows the engine to access 
        bool IsRunning = false;
    };

    std::unique_ptr<FData> TaskData = nullptr;

    #pragma endregion
};