#include "stdafx.h"
#include "WindowEventArgs.h"

#include "Window.h"
#include "DeviceContext.h"

CButtonPressEventArgs::CButtonPressEventArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
	: Flags((UINT)wParam),
	Point((DWORD)lParam)
{
}

CScrollEventArgs::CScrollEventArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
	: ScrollBarCode(LOWORD(wParam)),
	Position(HIWORD(wParam)),
	ScrollBarWnd((HWND)lParam)
{
}

CFocusEventArgs::CFocusEventArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
	: pWindow(CWnd::FromHandle((HWND)wParam))
{
}

CSizeEventArgs::CSizeEventArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
	: Size(LOWORD(lParam), HIWORD(lParam))
{

}

CCharEventArgs::CCharEventArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
	: Char((UINT)wParam),
	RepeatCount((UINT)LOWORD(lParam)),
	Flags((UINT)HIWORD(lParam))
{
}

CPaintArgs::CPaintArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
{
	if (wParam)
		pPaintDC = CDC::FromHandle((HDC)wParam);
	else
	{
		pPaintDC = new CPaintDC(Window);
		m_bDelete = true;
	}
}

CPaintArgs::~CPaintArgs()
{
	if (m_bDelete)
		delete pPaintDC;
}

CQueryDragIconArgs::CQueryDragIconArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
{

}

CGetMinMaxInfoArgs::CGetMinMaxInfoArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
	: MinMaxInfo(*reinterpret_cast<MINMAXINFO*>(lParam))
{

}