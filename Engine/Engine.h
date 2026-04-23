#pragma once
#include "Core/Core.h"
#include "Windows/Spawnable/SpawnableWindow.h"
#include <vector>

class ThreadedTask;
class Object;

template <typename T>
concept DerivedFromThreadedTask = std::is_base_of_v<ThreadedTask, T>;

class ENGINE_API Engine : private std::enable_shared_from_this<Engine>
{
public:
    Engine();
    ~Engine();
    
    #pragma region Singleton

private:
    // Singleton pattern
    static std::shared_ptr<Engine> EngineInstance; 
public:
    // Returns a shared pointer to the engine instance or creates one if missing
    static std::shared_ptr<Engine> GetEngineInstance();
#pragma endregion

    #pragma region Threaded Tasks
public:
    //Threaded Tasks
    template<DerivedFromThreadedTask T, typename ...Args>
    std::shared_ptr<T> CreateThreadedTask(Args ...args);
private:
    void StopThreads();
    
    #pragma endregion

    #pragma region Pointer To Implementation

public:
    struct FEngineData
    {
        bool IsRunning = false;

        // Keeps a reference & ownership of all threaded tasks
        std::vector<std::shared_ptr<ThreadedTask>> Tasks;

        // Main Window
        std::shared_ptr<SpawnableWindow> Window = nullptr;
    };
private:
    // Pointer to the implementation
    std::unique_ptr<FEngineData> EngineData;

    #pragma endregion

private:
    void Init();
public:
    void Launch();
    void Quit();

private:
    //Objects
    static std::vector<std::shared_ptr<Object>> Objects;
    //static std::unique_ptr<class World> WorldInstance;
};