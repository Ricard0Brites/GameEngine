#pragma once

#include <string>
#include "windows.h"

#pragma region Macros
#ifdef EXPORTS_ENGINE
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#pragma endregion

class ENGINE_API Engine
{
public:
    static Engine& Get();
    void Launch(std::wstring GameName);
    void Quit();

    static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
    Engine() = default;
    HWND WindowInstance = nullptr;
    bool IsRunning = false;

    bool RegisterWindowClass(std::wstring ClassName);
    bool CreateWindowInstance(const std::wstring& ClassName, const std::wstring& GameName);
};
