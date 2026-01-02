#pragma once
#include "Core/Core.h"

static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static int WindowIDRunningCount = 0; // No need to handle data races bc win32 by design is made to be run sync

class WindowBase
{
public:
	WindowBase(const WCHAR* InWindowTitle);
	~WindowBase();

	virtual void PumpMessages();

	/* Triggered Right Before Destruction */
	Delegate<> OnDestroy;

private:
	bool RegisterWindowClass();
	bool CreateWindowInstance();

	const WCHAR* WindowTitle, *ClassName;

	HWND WindowHandle = nullptr;
};