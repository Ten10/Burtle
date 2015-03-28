#include "stdafx.h"
#include "ControlEventArgs.h"

CCustomDrawArgs::CCustomDrawArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
{
	if (auto pCustomDraw = (NMCUSTOMDRAW*)lParam)
	{
		DrawStage = pCustomDraw->dwDrawStage;
		DeviceContext = pCustomDraw->hdc;
	}
}