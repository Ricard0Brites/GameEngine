#pragma once
#include "Core/Core.h"
#include "Windows/Spawnable/SpawnableWindow.h"
#include <vector>
#include <type_traits>

class ThreadedTask;
class Actor;

template <typename T>
concept DerivedFromThreadedTask = std::is_base_of_v<ThreadedTask, T>;

template <typename T>
concept DerivedFromActor = std::is_base_of_v<Actor, T>;

class Engine : private std::enable_shared_from_this<Engine>
{
public:
    Engine() = default;
    
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
    FEngineData EngineData;

    #pragma endregion

private:
    void Init();
public:
    void Launch();
    void Quit();

    #pragma region Object
private:
    static std::vector<std::shared_ptr<Actor>> ObjectCache;

public:

    template<DerivedFromActor T, typename ...Args>
    static std::shared_ptr<T> CreateActor(Args ...args);

    #pragma endregion

    //static std::unique_ptr<class World> WorldInstance;
};

template<DerivedFromActor T, typename ...Args>
std::shared_ptr<T> Engine::CreateActor(Args ...args)
{
    auto sptr = std::make_shared<T>(args...);
    ObjectCache.push_back(sptr);
    return sptr;
}