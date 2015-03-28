#include "stdafx.h"
#include "DateTimeControl.h"

bool CDateTimeControl::Create(DWORD dwStyle, const CRect& Rect, const CWindow& Parent)
{
	HWND hWnd = CWindow::Create(DATETIMEPICK_CLASS, _T(""), dwStyle, Rect, Parent, nullptr, nullptr);
	bool bSuccess = hWnd && SubclassWindow(hWnd);
	if (!bSuccess && hWnd)
		::DestroyWindow(hWnd);
	return bSuccess;
}

bool CDateTimeControl::SetTime(SYSTEMTIME& SystemTime, bool bValidSystemTime)
{
	return SendMessage(DTM_SETSYSTEMTIME, bValidSystemTime ? GDT_VALID : GDT_NONE, (LPARAM)&SystemTime) ? true : false;
}

bool CDateTimeControl::GetTime(SYSTEMTIME& SystemTime) const
{
	return SendMessage(DTM_GETSYSTEMTIME, 0, (LPARAM)&SystemTime) == GDT_VALID;
}

bool CDateTimeControl::SetFormat(const tstring& sFormat)
{
	return SendMessage(DTM_SETFORMAT, 0, (LPARAM)sFormat.c_str()) ? true : false;
}