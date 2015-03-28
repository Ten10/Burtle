#include "stdafx.h"
#include "Point.h"

CPoint::CPoint()
{
	x = y = 0;
}

CPoint::CPoint(DWORD Value)
{
	x = LOWORD(Value);
	y = HIWORD(Value);
}

CPoint::CPoint(LONG X, LONG Y)
{
	x = X;
	y = Y;
}

CSize CPoint::operator-(const CPoint& Other) const
{
	return CSize(Other.x - x, Other.y - y);
}