#pragma once
#include "Core/Core.h"
#include <memory>
#include <unordered_map>

static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static int WindowIDRunningCount = 0;

class ENGINE_API WindowBase
{
public:
    struct WindowBaseImpl;

    WindowBase(const WCHAR* InWindowTitle);
    ~WindowBase();

    virtual void OnMessageReceived(UINT msg) = 0;
    virtual void OnDestroy() = 0; // Bubble Destroy Event To gracefully shutdown any systems (rendering mb
    
    HWND* GetWindow();
    Delegate<FVector2> OnWindowResizeDelegate;

    const FVector2& GetResolution();
    
    void Internal_BroadcastWindowSize(const HWND& hWnd);
    bool Internal_WindowSizeDirty = false;

protected:
    void PumpMessages();

private:
    WindowBaseImpl* Pimpl = nullptr;
};

static std::unordered_map<HWND, WindowBase*> WindowsList = {};