#pragma once
#include "Core/Core.h"
#include "Windows/WindowBase.h"

class ENGINE_API Engine : private WindowBase
{
public:
    Engine(const WCHAR* InWindowTitle);
    void Launch();
    void Quit();

private:
    ~Engine();
    bool IsRunning = false;
};
