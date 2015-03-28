#pragma once
#include "Size.h"

struct CPoint : public POINT
{
	CPoint();
	CPoint(DWORD Value);
	CPoint(LONG X, LONG Y);

	CSize operator-(const CPoint& Other) const;
};