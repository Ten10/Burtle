#include "stdafx.h"
#include "Rectangle.h"

CRectangle::CRectangle()
{
	left = right = top = bottom = 0;
}

CRectangle::CRectangle(LONG Left, LONG Top, LONG Right, LONG Bottom)
{
	left = Left;
	right = Right;
	top = Top;
	bottom = Bottom;
}

LONG CRectangle::Width() const { return right - left; }
LONG CRectangle::Height() const { return bottom - top; }

void CRectangle::OffsetRect(int X, int Y)
{
	left += X;
	right += X;
	bottom += Y;
	top += Y;
}

void CRectangle::OffsetRect(CSize Offset)
{
	OffsetRect(Offset.cx, Offset.cy);
}

CPoint CRectangle::Center() const
{
	return CPoint(static_cast<LONG>((right + left) / 2.0), static_cast<LONG>((bottom + top) / 2));
}

CPoint CRectangle::TopLeft() const
{
	return CPoint(left, top);
}

CPoint CRectangle::BottomRight() const
{
	return CPoint(right, bottom);
}

CScreenRect::CScreenRect(HWND hWnd)
{
	::GetWindowRect(hWnd, this);
}

CClientRect::CClientRect(HWND hWnd)
{
	::GetClientRect(hWnd, this);
}

CClientRect::CClientRect(const CScreenRect& Rect)
{
	static_cast<CRect&>(*this) = Rect;
	OffsetRect(-left, -top);
}

CScreenRect CClientRect::ToScreen(HWND hWindow) const
{
	CPoint topLeft = TopLeft();
	CPoint bottomRight = BottomRight();
	::ClientToScreen(hWindow, &topLeft);
	::ClientToScreen(hWindow, &bottomRight);
	CScreenRect rect;
	static_cast<CRect&>(rect) = CRectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
	return rect;
}

CWindowRelativeRect::CWindowRelativeRect(const CScreenRect& Rect, const CScreenRect& ParentRect)
{
	static_cast<CRect&>(*this) = CClientRect(Rect);
	OffsetRect(Rect.left - ParentRect.left, Rect.top - ParentRect.top);
}