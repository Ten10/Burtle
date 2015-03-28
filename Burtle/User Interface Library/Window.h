#pragma once
#include "Size.h"
#include "Point.h"
#include "Rectangle.h"
#include "Font.h"
#include "EventMap.h"
#include "HandleMap.h"
#include "WindowEventArgs.h"

class CWindow;
class CWindowBase
{
protected:
	CEventMap m_EventMap;

	virtual void Default() = 0;
	virtual void InitializeEventMap() { };
public:
	CEventMap& GetEventMap() { return m_EventMap; }

	CEvent<WM_LBUTTONDOWN, CButtonPressEventArgs> OnLeftButtonDown { m_EventMap };
	CEvent<WM_HSCROLL, CScrollEventArgs> OnHorizontalScroll { m_EventMap };
	CEvent<WM_VSCROLL, CScrollEventArgs> OnVerticalScroll { m_EventMap };
	CEvent<WM_SETFOCUS, CFocusEventArgs> OnSetFocus { m_EventMap };
	CEvent<WM_KILLFOCUS, CFocusEventArgs> OnKillFocus { m_EventMap };
	CEvent<WM_SIZE, CSizeEventArgs> OnSize{ m_EventMap };
	CEvent<WM_CHAR, CCharEventArgs> OnChar { m_EventMap };
	CEvent<WM_NCDESTROY, CNoEventArgs> OnNonClientDestroy { m_EventMap };
	CEvent<WM_PAINT, CPaintArgs> OnPaint { m_EventMap };
	CEvent<WM_QUERYDRAGICON, CQueryDragIconArgs> OnQueryDragIcon { m_EventMap };
	CEvent<WM_GETMINMAXINFO, CGetMinMaxInfoArgs> OnGetMinMaxInfo { m_EventMap };
};

class CWindow : public CWindowBase
{
public:
	typedef CHandleMap<HWND, CWindow> CWindowMap;

	struct CWindowInfo
	{
		MSG LastMessage;
		LRESULT LastMessageResult = 0;
		CWindow* pWindow = nullptr;
	};

private:
	friend class CWindowMap;
	CWindow(CWindow&);

	static LRESULT CALLBACK WindowSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
protected:
	HWND m_hWnd = nullptr;
	bool m_bSubclassed = false;
	CWindowInfo m_WindowInfo;
	void SetHandle(HWND hWnd);

	void PumpMessages(const bool& bContinue);
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	void InitializeEventMap() override;

	static CWindow* FromPermenantHandle(HWND hWnd);
	static CWindowInfo* GetWindowInfo(HWND hWnd);
public:
	CWindow();

	bool Attach(HWND hWnd);
	bool Detach();

	HWND Create(const tstring& ClassName, const tstring& WindowName, DWORD dwStyle, const CRect& Rect, const CWindow& Parent, HMENU hMenu, HINSTANCE hInstance);
	void DestroyWindow();

	bool SubclassWindow(HWND hWnd);
	virtual void PreSubclassWindow();
	bool UnsubclassWindow();

	void Default() override;

	void SetIcon(HICON hIcon, bool bBigIcon);
	bool IsIconic() const;

	void SetFocus();

	void Invalidate(bool bErase = false);

	::std::wstring GetText() const;
	bool SetText(const ::std::wstring& Text) const;
	CWindow* GetParent() const;
	CRect GetClientRect() const;

	DWORD GetStyle() const;
	void SetStyle(DWORD dwStyle);

	DWORD GetStyleEx() const;
	void SetStyleEx(DWORD dwStyleEx);

	int GetDlgCtrlID() const;

	void SetFont(CFont Font, bool bRedraw = true);
	CFont GetFont() const;

	bool GetScrollInfo(bool bHorizontal, SCROLLINFO& Info) const;
	void Scroll(CSize Size);

	void ShowWindow(UINT nCmdShow);

	operator HWND() const { return m_hWnd; }

	LRESULT SendMessage(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
	bool PostMessage(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;

	void OnNcDestroy();
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void OnFinalMessage();

	static CWindow* FromHandle(HWND hWnd);
};

typedef CWindow CWnd;