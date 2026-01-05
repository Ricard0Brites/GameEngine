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
    Pimpl->WindowHandle = CreateWindowExW(
        0,
        Pimpl->ClassName,
        Pimpl->WindowTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1280,
        720,
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

    ShowWindow(Pimpl->WindowHandle, SW_SHOW);
    UpdateWindow(Pimpl->WindowHandle);

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
}

WindowBase::~WindowBase()
{
    if (Pimpl && Pimpl->WindowHandle)
    {
        DestroyWindow(Pimpl->WindowHandle);
    }

    if (Pimpl)
    {
        delete(Pimpl);
        Pimpl = nullptr;
    }
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