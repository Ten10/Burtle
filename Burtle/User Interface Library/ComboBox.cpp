#include "stdafx.h"
#include "ComboBox.h"

bool CComboBox::Create(DWORD dwStyle, const CRect& Rect, const CWindow& Parent)
{
	HWND hWnd = CWindow::Create(WC_COMBOBOX, _T(""), dwStyle, Rect, Parent, nullptr, nullptr);
	bool bSuccess = hWnd && SubclassWindow(hWnd);
	if (!bSuccess && hWnd)
		::DestroyWindow(hWnd);
	return bSuccess;
}

bool CComboBox::AddString(const wstring& sItemText)
{
	return SendMessage(CB_ADDSTRING, 0, (LPARAM)sItemText.c_str()) >= 0;
}

bool CComboBox::InsertString(const ::std::wstring& sItemText, UINT nPosition)
{
	return SendMessage(CB_INSERTSTRING, (WPARAM)nPosition, (LPARAM)sItemText.c_str()) >= 0;
}

bool CComboBox::SelectString(UINT nStartIndex, const ::std::wstring& sItemText)
{
	return SendMessage(CB_SELECTSTRING, (WPARAM)nStartIndex, (LPARAM)sItemText.c_str()) ? true : false;
}

void CComboBox::SetCurSel(UINT nItemIndex)
{
	SendMessage(CB_SETCURSEL, (WPARAM)nItemIndex, 0);
}

UINT CComboBox::GetCurSel() const
{
	return (UINT)SendMessage(CB_GETCURSEL);
}

UINT CComboBox::GetCount() const
{
	return (UINT)SendMessage(CB_GETCOUNT);
}

bool CComboBox::SetItemData(UINT nItem, DWORD_PTR Data)
{
	return SendMessage(CB_SETITEMDATA, nItem, Data) != CB_ERR;
}

DWORD_PTR CComboBox::GetItemData(UINT nItem) const
{
	return SendMessage(CB_GETITEMDATA, nItem);
}