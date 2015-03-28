#include "stdafx.h"
#include "Edit.h"

bool CEdit::Create(DWORD dwStyle, const CRect& Rect, const CWindow& Parent)
{
	HWND hWnd = CWindow::Create(WC_EDIT, _T(""), dwStyle, Rect, Parent, NULL, NULL);
	bool bSuccess = hWnd && SubclassWindow(hWnd);
	if (!bSuccess && hWnd)
		::DestroyWindow(hWnd);
	return bSuccess;
}

bool CEdit::SetSelectedItem(const tstring& sItem)
{
	if (!__super::SetSelectedItem(sItem))
		return false;
	
	SetSelection(0, -1);
	return true;
}

void CEdit::SetSelection(int Start, int End)
{
	SendMessage(EM_SETSEL, Start, End);
}