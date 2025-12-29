#include "Engine.h"
#include <iostream>


Engine& Engine::Get()
{
    static Engine instance; // thread-safe since C++11
    return instance;
}

void Engine::Launch(std::wstring GameName)
{
    std::wstring ClassName = L"DX12EngineClass";

    if (!RegisterWindowClass(ClassName.c_str()))
        return;

    if (!CreateWindowInstance(ClassName.c_str(), L"DirectX 12 Engine - " + GameName))
        return;
    

    IsRunning = true;
    MSG msg{};
    while (IsRunning)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                IsRunning = false;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Gameplay / rendering here
    }

}

void Engine::Quit()
{
    IsRunning = false;
}

LRESULT CALLBACK Engine::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            Engine::Get().Quit();
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool Engine::RegisterWindowClass(std::wstring ClassName)
{
    /*  Create Window Class  */
    WNDCLASSEX WindowClassEX;

    WindowClassEX.lpfnWndProc = &Engine::WindowProcedure;
    WindowClassEX.hInstance = GetModuleHandle(nullptr);   

    WindowClassEX.cbSize = sizeof(WNDCLASSEX);
    WindowClassEX.style = CS_HREDRAW | CS_VREDRAW; // Horizontal and Vertical Redraw
    WindowClassEX.cbClsExtra = 0;
    WindowClassEX.cbWndExtra = 0;

    WindowClassEX.hCursor = LoadCursor(nullptr, IDC_ARROW); // Default Cursor
    WindowClassEX.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH); // Background Color

    WindowClassEX.hIcon = LoadIcon(0, IDI_APPLICATION); // Application Icon
    WindowClassEX.hIconSm = WindowClassEX.hIcon; // Application Icon (Tray/Taskbar)

    WindowClassEX.lpszClassName = ClassName.c_str();
    WindowClassEX.lpszMenuName = nullptr; // Remove Menus

    bool res = RegisterClassEx(&WindowClassEX);
    
    if(!res ) 
        std::cout << "Window Class Registration failed" << std::endl;

    return res;
}

bool Engine::CreateWindowInstance(const std::wstring& ClassName, const std::wstring& GameName)
{
    WindowInstance = CreateWindowExW(
        0,
        ClassName.c_str(),
        GameName.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1280,
        720,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
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