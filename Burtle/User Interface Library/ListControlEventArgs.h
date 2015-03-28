#pragma once
#include "ControlEventArgs.h"

struct CListEventArgs : public CEventArgs
{
	UINT Row = 0;
	UINT Column = 0;
};

struct CLabelBeginEditArgs : public CListEventArgs
{
	bool Cancel = false;
	CLabelBeginEditArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
	operator LRESULT() const;
};

struct CLabelEndEditArgs : public CListEventArgs
{
	::std::wstring Text;
	bool AcceptNewText = true;
	CLabelEndEditArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
	operator LRESULT() const;
};

struct CGetItemInfoArgs : public CListEventArgs
{
	DWORD Mask;

	CGetItemInfoArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
	operator LRESULT() const;

	void SetItemText(const ::std::wstring& Text);
	void SetItemState(DWORD State, DWORD StateMask);
private:
	NMLVDISPINFO* m_pDispInfo = nullptr;
};

struct CCacheHintArgs : public CListEventArgs
{
	int From = 0;
	int To = 0;

	CCacheHintArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
	operator LRESULT() const;
};

struct CListControlCustomDrawArgs : public CCustomDrawArgs
{
	UINT Row = 0;
	UINT Column = 0;

	CListControlCustomDrawArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam);
	operator LRESULT() const;

	void SetBackgroundColor(COLORREF Color);

private:
	NMLVCUSTOMDRAW* m_pListCustomDraw = nullptr;
};