#include "stdafx.h"
#include <CommCtrl.h>
#include "HeaderControl.h"

bool CHeaderControl::GetItemCount(UINT& ItemCount) const
{
	int nItemCount = (int)SendMessage(HDM_GETITEMCOUNT, 0, 0);
	if (nItemCount != -1)
		ItemCount = nItemCount;
	return nItemCount != -1;
}

bool CHeaderControl::GetItemRect(UINT nRow, CRect& Rect)
{
	return SendMessage(HDM_GETITEMRECT, nRow, (LPARAM)&Rect) ? true : false;
}