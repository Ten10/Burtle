#pragma once
#include "EventArgs.h"

struct CCustomDrawArgs : public CEventArgs
{
	DWORD DrawStage = 0;
	LRESULT Result = 0;
	HDC DeviceContext = nullptr;

	CCustomDrawArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
};