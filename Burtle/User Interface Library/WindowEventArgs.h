#pragma once
#include "EventArgs.h"
#include "Point.h"

class CDeviceContext;
struct CButtonPressEventArgs : public CEventArgs
{
	UINT Flags;
	CPoint Point;

	CButtonPressEventArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
};

struct CScrollEventArgs : public CEventArgs
{
	HWND ScrollBarWnd;
	UINT ScrollBarCode;
	UINT Position;

	CScrollEventArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
};

struct CFocusEventArgs : public CEventArgs
{
	CWindow* pWindow;

	CFocusEventArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
};

struct CSizeEventArgs : public CEventArgs
{
	CSize Size;

	CSizeEventArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
};

struct CCharEventArgs : public CEventArgs
{
	UINT Char;
	UINT RepeatCount;
	UINT Flags;

	CCharEventArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
};

struct CPaintArgs : public CEventArgs
{
	CDeviceContext* pPaintDC = nullptr;

	CPaintArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
	~CPaintArgs();
private:
	bool m_bDelete = false;
};

struct CQueryDragIconArgs : public CEventArgs
{
	HCURSOR DragIcon = NULL;
	CQueryDragIconArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
};

struct CGetMinMaxInfoArgs : public CEventArgs
{
	MINMAXINFO& MinMaxInfo;
	CGetMinMaxInfoArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
	operator LRESULT() { return 0; }
};