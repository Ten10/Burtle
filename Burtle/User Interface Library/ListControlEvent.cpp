#include "stdafx.h"
#include "ListControlEventArgs.h"

#include <strsafe.h>

CLabelBeginEditArgs::CLabelBeginEditArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
{

}

CLabelBeginEditArgs::operator LRESULT() const
{
	return Cancel;
}

CLabelEndEditArgs::CLabelEndEditArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
{
	if (auto pInfo = (NMLVDISPINFO*)lParam)
	{
		if (pInfo->item.pszText)
			Text = pInfo->item.pszText;

		Row = pInfo->item.iItem;
		Column = pInfo->item.iSubItem;
	}
}

CLabelEndEditArgs::operator LRESULT() const
{
	return AcceptNewText;
}

CGetItemInfoArgs::CGetItemInfoArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
	: m_pDispInfo((NMLVDISPINFO*)lParam)
{
	if (m_pDispInfo)
	{
		Mask = m_pDispInfo->item.mask;
		Row = m_pDispInfo->item.iItem;
		Column = m_pDispInfo->item.iSubItem;
	}
}

CGetItemInfoArgs::operator LRESULT() const
{
	return true;
}

void CGetItemInfoArgs::SetItemText(const ::std::wstring& Text)
{
	if (m_pDispInfo)
		StringCchCopy(m_pDispInfo->item.pszText, m_pDispInfo->item.cchTextMax, Text.c_str());
}

void CGetItemInfoArgs::SetItemState(DWORD State, DWORD StateMask)
{
	m_pDispInfo->item.state = State;
	m_pDispInfo->item.stateMask = StateMask;
}

CCacheHintArgs::CCacheHintArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
{
	if (NMLVCACHEHINT* pHintInfo = (NMLVCACHEHINT*)lParam)
	{
		From = pHintInfo->iFrom;
		To = pHintInfo->iTo;
	}
}

CCacheHintArgs::operator LRESULT() const
{
	return 0;
}

CListControlCustomDrawArgs::CListControlCustomDrawArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
	: CCustomDrawArgs(Window, wParam, lParam),
	m_pListCustomDraw((NMLVCUSTOMDRAW*)lParam)
{
	if (m_pListCustomDraw)
	{
		Row = static_cast<UINT>(m_pListCustomDraw->nmcd.dwItemSpec);
		Column = m_pListCustomDraw->iSubItem;
	}
}

CListControlCustomDrawArgs::operator LRESULT() const
{
	return Result;
}

void CListControlCustomDrawArgs::SetBackgroundColor(COLORREF Color)
{
	if (m_pListCustomDraw)
		m_pListCustomDraw->clrTextBk = Color;
}