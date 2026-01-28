#pragma once
#include "Core/Core.h"

static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static int WindowIDRunningCount = 0; // No need to handle data races bc win32 by design is made to be run sync

class WindowBase
{
public:
	WindowBase(const std::wstring &InWindowTitle);
	~WindowBase();

	/* Triggered Right Before Destruction */
	Delegate<> OnDestroy;

protected:
	void PumpMessages();
	virtual void OnMessageReceived(MSG InMessage) {};
	virtual void OnWindowResize(int X, int Y) {};
	HWND* GetWindowHandle() { return &WindowHandle;  }

private:
	bool RegisterWindowClass();
	bool CreateWindowInstance();

	std::wstring ClassName, WindowTitle;

	HWND WindowHandle = nullptr;
};