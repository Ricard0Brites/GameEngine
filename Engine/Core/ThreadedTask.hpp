#pragma once
#include "Core.h"
#include <functional>
#include <thread>

class ENGINE_API ThreadedTask
{
    friend class Engine;
public:
    ThreadedTask() : Pimpl(new SData)
    {

    }
    ~ThreadedTask()
    {
        if (Pimpl)
        {
            if (Pimpl->Thread)
                delete(Pimpl->Thread);

            delete(Pimpl);
        }
    }

    // Starts the async execution
    virtual void Init(const std::function<void()>& Func)
    {
        Pimpl->StartFunction = Func;
        if (Pimpl->StartFunction)
        {
            Pimpl->Thread = new std::thread(Pimpl->StartFunction);
        }
    }
    virtual void End()
    {
        Join();
    }

    void Join()
    {
        if (Pimpl->Thread)
            Pimpl->Thread->join();
    }

private:
    struct SData
    {
        std::function<void()> StartFunction = nullptr;
        std::thread* Thread = nullptr; // Friend declaration allows the engine to access 
    };
    SData* Pimpl = nullptr;
};
