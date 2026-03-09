#include "WindowBase.h"
#include <string>

// --- PImpl Definition ---
struct WindowBase::WindowBaseImpl
{
    // Window properties
    const WCHAR* WindowTitle = nullptr;
    const WCHAR* ClassName = nullptr;
    std::wstring ClassNameCache; // To hold the generated class name string
    HWND WindowHandle = nullptr;
    FVector2 Resolution = 0;
};

// --- Free Functions for Window Logic ---
bool RegisterWindowClass(WindowBase::WindowBaseImpl* Pimpl)
{
    WNDCLASSEX WindowClassEX;
    WindowClassEX.lpfnWndProc = &WinProc;
    WindowClassEX.hInstance = GetModuleHandle(nullptr);
    WindowClassEX.cbSize = sizeof(WNDCLASSEX);
    WindowClassEX.style = CS_HREDRAW | CS_VREDRAW;
    WindowClassEX.cbClsExtra = 0;
    WindowClassEX.cbWndExtra = 0;
    WindowClassEX.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WindowClassEX.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    WindowClassEX.hIcon = LoadIcon(0, IDI_APPLICATION);
    WindowClassEX.hIconSm = WindowClassEX.hIcon;
    WindowClassEX.lpszClassName = Pimpl->ClassName;
    WindowClassEX.lpszMenuName = nullptr;

    if (!RegisterClassEx(&WindowClassEX))
    {
        std::cout << "Window Class Registration failed" << std::endl;
        return false;
    }
   
    return true;
}

bool CreateWindowInstance(WindowBase::WindowBaseImpl* Pimpl)
{
    if (!Pimpl)
        return false;

    Pimpl->Resolution.Set(1280, 720); // TODO - Replace this to be dynamic (INI file maybe)

    Pimpl->WindowHandle = CreateWindowExW(
        0,
        Pimpl->ClassName,
        Pimpl->WindowTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (int)Pimpl->Resolution.GetX(),
        (int)Pimpl->Resolution.GetY(),
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        Pimpl
    );

    if (!Pimpl->WindowHandle)
    {
        std::cout << "Window Instance is invalid. Aborting..." << std::endl;
        return false;
    }

    return true;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // This is a simplified WinProc. A real implementation would store the Pimpl pointer
    // (e.g., using SetWindowLongPtr) and forward messages to a member function.
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
    {
        // Mark dirty for any size-affecting change
        if (wParam != SIZE_MINIMIZED)
        {
            WindowBase* WindowRef = WindowsList[hWnd];
            if (!WindowRef)
                break;
            WindowRef->Internal_WindowSizeDirty = true;
        }
        break;
    }
    case WM_EXITSIZEMOVE:
    {
        WindowBase* WindowRef = WindowsList[hWnd];

        if (!WindowRef)
            break;

        // Fired once when user finishes free resize or move
        if (WindowRef->Internal_WindowSizeDirty)
        {
            WindowRef->Internal_BroadcastWindowSize(hWnd);
            WindowRef->Internal_WindowSizeDirty = false;
        }
        break;
    }

    case WM_SYSCOMMAND:
    {
        switch (wParam & 0xFFF0)
        {
        case SC_MAXIMIZE:
        case SC_RESTORE:
        {
            WindowBase* WindowRef = WindowsList[hWnd];

            if (!WindowRef)
                break;

            LRESULT Res = DefWindowProcW(hWnd, msg, wParam, lParam);
            WindowRef->Internal_BroadcastWindowSize(hWnd);
            WindowRef->Internal_WindowSizeDirty = false;
            return Res;
        }
        }
        break;
    }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// --- WindowBase Method Implementations ---

WindowBase::WindowBase(const WCHAR* InWindowTitle)
    : Pimpl(new WindowBaseImpl)
{
    Pimpl->WindowTitle = InWindowTitle;
    Pimpl->ClassNameCache = L"WindowClass_" + std::to_wstring(WindowIDRunningCount++);
    Pimpl->ClassName = Pimpl->ClassNameCache.c_str();

    RegisterWindowClass(Pimpl);
    CreateWindowInstance(Pimpl);
    WindowsList.insert({ Pimpl->WindowHandle, this });
    UpdateWindow(Pimpl->WindowHandle);
    ShowWindow(Pimpl->WindowHandle, SW_SHOW);
}

WindowBase::~WindowBase()
{
    if (Pimpl)
    {
        if(Pimpl->WindowHandle)
            DestroyWindow(Pimpl->WindowHandle);
     
        delete(Pimpl);
        Pimpl = nullptr;
    }
}

HWND* WindowBase::GetWindow()
{
    return &Pimpl->WindowHandle;
}

const FVector2& WindowBase::GetResolution()
{
    return Pimpl->Resolution;
}

void WindowBase::Internal_BroadcastWindowSize(const HWND& hWnd)
{
    // Send Window Resize Event
    RECT r;
    if (GetWindowRect(hWnd, &r))
    {
        FVector2 NewResolution = FVector2((float)(r.right - r.left), (float)(r.bottom - r.top));
        OnWindowResizeDelegate.Execute(NewResolution);
        Pimpl->Resolution = NewResolution;
    }

    // Reset Dirtiness
    Internal_WindowSizeDirty = false;
}

void WindowBase::PumpMessages()
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        OnMessageReceived(msg.message);

        if (msg.message == WM_QUIT)
        {
            OnDestroy();
            if (Pimpl->WindowHandle)
            {
                DestroyWindow(Pimpl->WindowHandle);
                Pimpl->WindowHandle = nullptr;
            }
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}