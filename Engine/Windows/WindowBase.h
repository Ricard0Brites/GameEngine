#pragma once
#include "Core/Core.h"
#include <memory>
#include <unordered_map>

#pragma region Win32 API

static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static int WindowIDRunningCount = 0;

#pragma endregion

class WindowBase
{
public:
    WindowBase(const WCHAR* InWindowTitle);
    ~WindowBase();

    DECLARE_DELEGATE(WindowBaseDelegate);

    #pragma region Events

    virtual void OnMessageReceived(UINT msg) = 0;

    virtual void OnDestroy() = 0; // Bubble Destroy Event To gracefully shutdown any systems

    #pragma endregion

    HWND* GetWindow();
    FWindowBaseDelegate<FVector2> OnWindowResizeDelegate;

    const FVector2& GetResolution();
    
    void Internal_BroadcastWindowSize(const HWND& hWnd);
    bool Internal_WindowSizeDirty = false;

    void PumpMessages();

    #pragma region Pointer To Implementation
public:
    struct FWindowBaseData
    {
        FWindowBaseData() = default;
        ~FWindowBaseData() = default;

        const WCHAR* WindowTitle = nullptr;
        const WCHAR* ClassName = nullptr;
        std::wstring ClassNameCache; // To hold the generated class name string
        HWND WindowHandle = nullptr;
        FVector2 Resolution = 0;
    };
protected:
    inline void SetWindowName(std::wstring Title);
private:
    std::unique_ptr<FWindowBaseData> WindowData = nullptr;
    #pragma endregion
};

static std::unordered_map<HWND, WindowBase*> WindowsList = {};