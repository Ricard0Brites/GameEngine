#pragma once

#include "windows.h"
#include "math.h"

#include <string>

#pragma region Macros
#ifdef EXPORTS_ENGINE
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#pragma endregion

struct ENGINE_API UVector2
{
    unsigned int x = 0;
    unsigned int y = 0;

    UVector2() = default;
    UVector2(unsigned int v) : x(v), y(v) {}
    UVector2(unsigned int InX, unsigned int InY) : x(InX), y(InY) {}

    double Length() const
    {
        double dx = static_cast<double>(x);
        double dy = static_cast<double>(y);
        return sqrt((dx * dx) + (dy * dy));
    }

    #pragma region Operators

    UVector2& operator=(const UVector2& In)
    {
        x = In.x;
        y = In.y;
        return *this;
    }

    UVector2 operator-(const UVector2& In) const
    {
        return UVector2(x - In.x, y - In.y);
    }

    bool operator>(const UVector2& In) const
    {
        return Length() > In.Length();
    }

    bool operator<(const UVector2& In) const
    {
        return Length() < In.Length();
    }

    bool operator>=(const UVector2& In) const
    {
        return Length() >= In.Length();
    }

    bool operator<=(const UVector2& In) const
    {
        return Length() <= In.Length();
    }

    #pragma endregion
};

class ENGINE_API WindowBase
{
public:
    WindowBase(const UVector2 &InWindowSize, const WCHAR &InClassName, const WCHAR &InWindowTitle) : 
        WindowSize(InWindowSize),
        ClassName(&InClassName),
        WindowTitle(&InWindowTitle)
    {};
    ~WindowBase() 
    {
        if (WindowInstance)
        {
            DestroyWindow(WindowInstance);
            WindowInstance = nullptr;
        }
    };

    virtual void Init();
    void UpdateMessages(); // Message Handling
protected:
    virtual bool RegisterWindowClass();
    virtual bool CreateWindowInstance();
    virtual LRESULT HandleWindowProcedure(UINT msg, WPARAM wParam, LPARAM lParam);

private:
    void UpdateResolution()
    {

    }

    // Message handling
    static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    
    // References
    HWND WindowInstance = nullptr;
    static HINSTANCE ModuleInstance; 
    
    // Parameters
    UVector2 WindowSize;
    const WCHAR* ClassName = nullptr, *WindowTitle = nullptr;

};

class ENGINE_API Engine
{
public:
    Engine(const UVector2 &WindowResolution, const WCHAR &IGameName);
    static Engine& Get(const UVector2 &WindowResolution, const WCHAR &GameName);
    void Launch();
    void Quit();

private:
    bool IsRunning = false;
    WindowBase* EngineWindow = nullptr; // The engine doesn't have UI so this will be the game.

    
};
