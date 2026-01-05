#pragma once
#include "Core/Core.h"
#include "Windows/WindowBase.h"

class ENGINE_API Engine : private WindowBase
{
public:
    Engine(const WCHAR* InWindowTitle);
    ~Engine(); // Destructor must be defined in the .cpp file with PImpl

    void Launch();
    void Quit();

    // Window Base Interface
    void OnMessageReceived(UINT msg) override;
    void OnDestroy() override;

private:
    // Forward-declare the implementation struct
    struct EngineImpl; 
    
    // The pointer to the implementation (PImpl)
    EngineImpl *Pimpl;

#pragma region Threaded Tasks

    /* ----------------- Memory Management ------------------------- */

    //Call only on object destruction
    void DestroyThreadedTasks(); 

    /* ----------------- Helpers ------------------------- */

    void JoinThreads();

#pragma endregion
    
};
