#pragma once
#include "Core/Core.h"

class ENGINE_API Engine
{
public:
    Engine(const WCHAR* InWindowTitle);
    void Launch();
    void Quit();

private:
    ~Engine();
    bool IsRunning = false;
    class WindowBase* Window = nullptr;
};
