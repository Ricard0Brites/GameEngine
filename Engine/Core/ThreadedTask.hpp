#pragma once
#include "Core.h"
#include <chrono>
#include <functional>
#include <thread>
#include <type_traits>



class ENGINE_API ThreadedTask
{
    friend class Engine; // The only one to call ThreadedTask::Join()
public:
    virtual void AsyncTick(float Delta) = 0;
    ThreadedTask() : TaskData(new FData)
    {
        Init();
    }

    ~ThreadedTask()
    {
        if (TaskData)
        {
            if (TaskData->Thread)
            {
                delete(TaskData->Thread);
                TaskData->Thread = nullptr;
            }

            delete(TaskData);
            TaskData = nullptr;
        }
    }
    
private:

    // Starts the async execution
    void Init();
    //Runs the async loop
    void Async_Init();
    
    bool IsRunning() { return TaskData->IsRunning; };
    
    void StopThread() { TaskData->IsRunning = false;  }
    
    void Join();


    struct FData
    {
        FData() = default;
        std::thread* Thread = nullptr; // Friend declaration allows the engine to access 
        bool IsRunning = false;
    };
    FData* TaskData = nullptr;
};

inline void ThreadedTask::Join()
{
    if (TaskData->Thread)
        TaskData->Thread->join();
}

inline void ThreadedTask::Init()
{
    TaskData->IsRunning = true;
    TaskData->Thread = new std::thread(&ThreadedTask::Async_Init, this);
}

inline void ThreadedTask::Async_Init()
{
    std::chrono::steady_clock::time_point PreviousFrame = std::chrono::high_resolution_clock::now();

    while (IsRunning())
    {
        std::chrono::steady_clock::time_point CurrentFrame = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> DeltaTime = CurrentFrame - PreviousFrame;
        PreviousFrame = CurrentFrame;

        AsyncTick(DeltaTime.count());
    }
}