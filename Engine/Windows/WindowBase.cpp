#include "WindowBase.h"
#include <string>

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

WindowBase::WindowBase(const WCHAR *InWindowTitle):
    WindowTitle(InWindowTitle)
{
    std::wstring cache = L"WindowClass_" + std::to_wstring(WindowIDRunningCount++);
    ClassName = cache.c_str();

    RegisterWindowClass();
    CreateWindowInstance();
}

WindowBase::~WindowBase()
{
    if (WindowHandle)
        DestroyWindow(WindowHandle);
}

void WindowBase::PumpMessages()
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        OnMessageReceived.Execute(msg.message); // Bubble Event
        
        // Standard Behavior
        if (msg.message == WM_QUIT)
        {
            OnDestroy.Execute();
            DestroyWindow(WindowHandle);
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool WindowBase::RegisterWindowClass()
{
    /*  Create Window Class  */
    WNDCLASSEX WindowClassEX;

    WindowClassEX.lpfnWndProc = &WinProc;
    WindowClassEX.hInstance = GetModuleHandle(nullptr);

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
    WindowHandle = CreateWindowExW(
        0,
        ClassName,
        WindowTitle,
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

    if (!WindowHandle)
    {
        std::cout << "Window Instance is invalid. Aborting..." << std::endl;
        return false;
    }

    ShowWindow(WindowHandle, SW_SHOW);
    UpdateWindow(WindowHandle);

    return true;
}