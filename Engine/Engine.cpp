#include "Engine.h"
#include <iostream>

#pragma region Engine

Engine::Engine(const UVector2 &WindowResolution, const WCHAR &GameName)
{
    const WCHAR* ClassName = L"EngineWindowClass";
    EngineWindow = new WindowBase(UVector2(1280, 720), *ClassName , GameName);
    EngineWindow->Init();
}

Engine& Engine::Get(const UVector2 &WindowResolution, const WCHAR &GameName)
{
    static Engine instance(WindowResolution, GameName); // thread-safe since C++11
    return instance;
}

void Engine::Launch()
{
    //Start Game Loop
    IsRunning = true;
    while (IsRunning)
    {
        if (!EngineWindow)
        {
            IsRunning = false;
            break;
        }

        EngineWindow->UpdateMessages(); // Pump Win32 Messages


        /*********************************************************************
        *                                                                    *       
        *   ---------------------   Gameplay  ----------------------------   *
        *                                                                    *
        *********************************************************************/



        /*********************************************************************
        *                                                                    *
        *   ---------------------   Rendering  ---------------------------   *
        *                                                                    *
        *********************************************************************/


    }

}

void Engine::Quit()
{
    IsRunning = false;
}

#pragma endregion

#pragma region WindowBase
HINSTANCE WindowBase::ModuleInstance = nullptr; // Static Definition

void WindowBase::Init()
{
    ModuleInstance = GetModuleHandle(nullptr);
    

    if (!RegisterWindowClass() || !CreateWindowInstance())
        return;
}

// Window Setup
bool WindowBase::RegisterWindowClass()
{
    /*  Create Window Class  */
    WNDCLASSEX WindowClassEX;

    WindowClassEX.lpfnWndProc = &WindowBase::WindowProcedure;
    WindowClassEX.hInstance = ModuleInstance;

    WindowClassEX.cbSize = sizeof(WNDCLASSEX);
    WindowClassEX.style = CS_HREDRAW | CS_VREDRAW; // Horizontal and Vertical Redraw
    WindowClassEX.cbClsExtra = 0;
    WindowClassEX.cbWndExtra = 0;

    WindowClassEX.hCursor = LoadCursor(nullptr, IDC_ARROW); // Default Cursor
    WindowClassEX.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH); // Background Color

    WindowClassEX.hIcon = LoadIcon(0, IDI_APPLICATION); // Application Icon
    WindowClassEX.hIconSm = WindowClassEX.hIcon; // Application Icon (Tray/Taskbar)

    WindowClassEX.lpszClassName = ClassName;
    WindowClassEX.lpszMenuName = nullptr; // Remove Menus

    bool res = RegisterClassEx(&WindowClassEX);

    if (!res)
        std::cout << "Window Class Registration failed" << std::endl;

    return res;
}

bool WindowBase::CreateWindowInstance()
{
    WindowInstance = CreateWindowEx(
        0,
        ClassName,
        WindowTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WindowSize.x,
        WindowSize.y,
        nullptr,
        nullptr,
        ModuleInstance,
        this
    );

    if (!WindowInstance)
    {
        std::cout << "Window Instance is invalid. Aborting..." << std::endl;
        return false;
    }

    ShowWindow(WindowInstance, SW_SHOW);
    UpdateWindow(WindowInstance);

    return true;
}


// Message Handling
void WindowBase::UpdateMessages()
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK WindowBase::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    WindowBase* window = nullptr;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = static_cast<WindowBase*>(cs->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(window));

        window->WindowInstance = hWnd;
    }
    else
    {
        window = reinterpret_cast<WindowBase*>(
            GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (window)
        return window->HandleWindowProcedure(msg, wParam, lParam); // Bubble event to instance (Overrides in children)

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WindowBase::HandleWindowProcedure(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CLOSE:
        {
            DestroyWindow(WindowInstance);
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(WindowInstance, msg, wParam, lParam);
}

#pragma endregion