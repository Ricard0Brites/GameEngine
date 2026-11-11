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
    
    ThreadedTask(std::string ThreadName) : TaskData(std::make_unique<FData>())
    {
        TaskData->ThreadName = ThreadName;

        Init();
    }

    ~ThreadedTask()
    {
        Join(); // Join the thread before self is destroyed
    }

private:

    std::string LogCategory = "Engine_ThreadedTask";

    // Internal synchronous execution -> Parent thread
    void Init()
    {
        Logger::Log(Logger::ELogCategories::Verbose, "Attempting to create Threaded Task '" + TaskData->ThreadName + "';", LogCategory);

        TaskData->Thread = std::make_unique<std::thread>(&ThreadedTask::Async_Init, this);

        if (!TaskData->Thread || !TaskData->Thread->joinable())
        {
            Logger::Log(Logger::ELogCategories::Error, "Threaded Task '" + TaskData->ThreadName + "' was not created. Please Refer to ThreadedTask::Init;", LogCategory);
            return;
        }
		
        // Set Thread state (In Main Thread)
        TaskData->IsRunning = true;

        // Set Thread Name
        {
            int size = MultiByteToWideChar(CP_UTF8, 0, TaskData->ThreadName.c_str(), -1, nullptr, 0);

            if (size > 0)
            {
			    std::wstring wideName(size, L'\0');

			    MultiByteToWideChar(
				    CP_UTF8, 0, TaskData->ThreadName.c_str(), -1,
				    wideName.data(), size
			    );

			    SetThreadDescription(TaskData->Thread->native_handle(), wideName.c_str());
            }
        }
    }

    // Internal asynchronous loop -> New Thread
    void Async_Init()
    {
		Logger::Log(Logger::ELogCategories::Verbose, "Created Threaded Task '" + TaskData->ThreadName + "';", LogCategory);


        AsyncInit(); // Virtual Event

        // Tick
        std::chrono::steady_clock::time_point PreviousFrame = std::chrono::high_resolution_clock::now();
        while (IsRunning())
        {
            std::chrono::steady_clock::time_point CurrentFrame = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> DeltaTime = CurrentFrame - PreviousFrame;
            PreviousFrame = CurrentFrame;

            AsyncTick(DeltaTime.count()); // Virtual Event
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
        std::string ThreadName = "Unnamed Threaded Task";
    };

    std::unique_ptr<FData> TaskData = nullptr;

    #pragma endregion
};