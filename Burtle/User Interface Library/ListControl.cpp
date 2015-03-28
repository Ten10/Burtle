#include "stdafx.h"
#include "ListControl.h"

void CListControl::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	m_HeaderCtrl.SubclassWindow(ListView_GetHeader(m_hWnd));
}

bool CListControl::Create(DWORD dwStyle, const CRect& Rect, const CWindow& Parent)
{
	HWND hWnd = CWindow::Create(WC_LISTVIEW, _T(""), dwStyle, Rect, Parent, nullptr, nullptr);
	bool bSuccess = hWnd && SubclassWindow(hWnd);
	if (!bSuccess && hWnd)
		::DestroyWindow(hWnd);
	return bSuccess;
}

UINT CListControl::GetColumnWidth(UINT nColumn) const
{
	return (UINT)SendMessage(LVM_GETCOLUMNWIDTH, nColumn, 0);
}

bool CListControl::SetColumnWidth(UINT nColumn, UINT nWidth)
{
	return SendMessage(LVM_SETCOLUMNWIDTH, nColumn, nWidth) ? true : false;
}

wstring CListControl::GetItemText(UINT nRow, UINT nColumn) const
{
	TCHAR buffer[512] = { 0 };
	LVITEM item = { 0 };
	item.iSubItem = nColumn;
	item.pszText = buffer;
	item.cchTextMax = _countof(buffer);
	SendMessage(LVM_GETITEMTEXT, (WPARAM)nRow, (LPARAM)&item);
	return buffer;
}

bool CListControl::GetItemRect(UINT nRow, CRect& Rect, DWORD ItemPortion) const
{
	Rect.left = ItemPortion;
	return SendMessage(LVM_GETITEMRECT, nRow, (LPARAM)&Rect) ? true : false;
}

bool CListControl::GetSubItemRect(UINT nRow, UINT nColumn, CRect& Rect, DWORD ItemPortion) const
{
	CRect r;
	r.top = nColumn;
	r.left = ItemPortion;
	if (!SendMessage(LVM_GETSUBITEMRECT, (WPARAM)nRow, (LPARAM)&r))
		return false;

	Rect = r;
	return true;
}

DWORD CListControl::GetItemData(UINT nItem) const
{
	LVITEM item = { 0 };
	item.mask = LVIF_PARAM;
	bool bGotItem = SendMessage(LVM_GETITEM, 0, (LPARAM)&item) ? true : false;
	return bGotItem ? (DWORD)item.lParam : 0;
}

bool CListControl::GetCheck(UINT nItem) const
{
	UINT nItemState = (UINT)SendMessage(LVM_GETITEMSTATE, nItem, (LPARAM)LVIS_STATEIMAGEMASK);
	return nItemState == INDEXTOSTATEIMAGEMASK(2);
}

UINT CListControl::GetItemCount() const
{
	return (UINT)SendMessage(LVM_GETITEMCOUNT);
}

void CListControl::SetItemCount(UINT nCount, DWORD Flags)
{
	SendMessage(LVM_SETITEMCOUNT, nCount, Flags);
}

UINT CListControl::InsertColumn(UINT nColumn, LPCTSTR Name)
{
	TCHAR buffer[512] = { 0 };
	_tcscpy_s(buffer, Name);
	LVCOLUMN columnInfo = { 0 };
	columnInfo.mask = LVCF_TEXT;
	columnInfo.pszText = buffer;
	return (UINT)SendMessage(LVM_INSERTCOLUMN, nColumn, (LPARAM)&columnInfo);
}

bool CListControl::SubItemHitTest(LVHITTESTINFO& Info) const
{
	return SendMessage(LVM_SUBITEMHITTEST, 0, (WPARAM)&Info) != -1;
}

bool CListControl::EnsureVisible(UINT nItem, bool bPartial)
{
	return SendMessage(LVM_ENSUREVISIBLE, nItem, bPartial) ? true : false;
}

bool CListControl::SetItemState(UINT nRow, DWORD dwStateMask, DWORD dwState)
{
	LVITEM item = { 0 };
	item.stateMask = dwStateMask;
	item.state = dwState;
	return SendMessage(LVM_SETITEMSTATE, nRow, (LPARAM)&item) ? true : false;
}

bool CListControl::RedrawItems(UINT nFirstItemIndex, UINT nLastItemIndex)
{
	return SendMessage(LVM_REDRAWITEMS, nFirstItemIndex, nLastItemIndex) ? true : false;
}

DWORD CListControl::SetExtendedStyle(DWORD dwStyleEx)
{
	return (DWORD)SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, dwStyleEx, dwStyleEx);
}

DWORD CListControl::GetExtendedStyle() const
{
	return (DWORD)SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
}

CHeaderCtrl* CListControl::GetHeaderCtrl() const
{
	CHeaderCtrl* pHeaderCtrl = nullptr;
	if (HWND hHeaderCtrl = (HWND)SendMessage(LVM_GETHEADER, 0, 0))
		pHeaderCtrl = dynamic_cast<CHeaderControl*>(CWnd::FromHandle(hHeaderCtrl));
	return pHeaderCtrl;
}