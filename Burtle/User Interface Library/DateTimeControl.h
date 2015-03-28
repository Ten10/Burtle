#pragma once
#include "Control.h"

class CDateTimeControl : public CControl
{
public:
	bool Create(DWORD dwStyle, const CRect& Rect, const CWindow& Parent);

	bool SetTime(SYSTEMTIME& SystemTime, bool bValidSystemTime = true);
	bool GetTime(SYSTEMTIME& SystemTime) const;

	bool SetFormat(const tstring& sFormat);
};