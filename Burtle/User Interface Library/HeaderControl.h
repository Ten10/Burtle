#pragma once
#include "Control.h"

class CHeaderControl : public CWindow
{
public:
	bool GetItemCount(UINT& ItemCount) const;
	bool GetItemRect(UINT nRow, CRect& Rect);
};

typedef CHeaderControl CHeaderCtrl;