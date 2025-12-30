#include "Engine.h"
#include "Windows/WindowBase.h"

Engine::Engine(const WCHAR* InWindowTitle) :
    Window(new WindowBase(InWindowTitle))
{
    if (Window)
    {
        Window->OnDestroy.Bind(this, &Engine::Quit);
    }
}

Engine::~Engine()
{
    if (Window)
        delete(Window);
}

void Engine::Launch()
{    
    IsRunning = true;
    while (IsRunning)
    {
        if (Window)
            Window->PumpMessages();
    }

}

void Engine::Quit()
{
    IsRunning = false;

}
