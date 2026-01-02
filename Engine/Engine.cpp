#include "Engine.h"
#include "Windows/WindowBase.h"

Engine::Engine(const WCHAR* InWindowTitle)
    : WindowBase(InWindowTitle)
{
    OnDestroy.Bind(this, &Engine::Quit);

}

Engine::~Engine()
{
}

void Engine::Launch()
{    
    IsRunning = true;
    while (IsRunning)
    {
        PumpMessages();
    }

}

void Engine::Quit()
{
    IsRunning = false;
}
