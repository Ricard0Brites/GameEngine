#include "WindowBase.h"
#include <string>
#include <list>


Delegate<int,int> OnWindowResizeDelegate;
bool WindowSizeDirty = false;

void BroadcastWindowSize(const HWND &hWnd)
{
    // Send Window Resize Event
    RECT r;
    if (GetWindowRect(hWnd, &r))
        OnWindowResizeDelegate.Execute(r.right - r.left, r.bottom - r.top);

    WindowSizeDirty = false;
}

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
    case WM_SIZE:
        WindowSizeDirty = true;
        
        // Handles Maximize and restore
        if (wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED)
            BroadcastWindowSize(hWnd);

        // Handles Fullscreen 
        QUERY_USER_NOTIFICATION_STATE qry;
        SHQueryUserNotificationState(&qry);
        if (qry == (QUNS_RUNNING_D3D_FULL_SCREEN | QUNS_BUSY | QUNS_PRESENTATION_MODE))
            BroadcastWindowSize(hWnd);

        break;
    case WM_EXITSIZEMOVE:
        if (WindowSizeDirty)
        {
            BroadcastWindowSize(hWnd);
        }
        break;
    }

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

WindowBase::WindowBase(const std::wstring& InWindowTitle) :
    WindowTitle(InWindowTitle)
{
    ClassName = L"WindowClass_";

    OnWindowResizeDelegate.Bind(this, &WindowBase::OnWindowResize);

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
        OnMessageReceived(msg);

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

    WindowClassEX.lpszClassName = ClassName.c_str();
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
        ClassName.c_str(),
        WindowTitle.c_str(),
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