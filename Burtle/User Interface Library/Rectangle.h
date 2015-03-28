#pragma once
#include "Point.h"
#include "Size.h"

struct CRectangle : public RECT
{
	CRectangle();

	CRectangle(LONG Left, LONG Top, LONG Right, LONG Bottom);

	LONG Width() const;
	LONG Height() const;

	void OffsetRect(int X, int Y);
	void OffsetRect(CSize Offset);
	CPoint Center() const;
	CPoint TopLeft() const;
	CPoint BottomRight() const;
};

struct CScreenRect : public CRectangle
{
	CScreenRect() = default;
	CScreenRect(HWND hWnd);
};

struct CClientRect : public CRectangle
{
	CClientRect() = default;
	CClientRect(HWND hWnd);
	CClientRect(const CScreenRect& Rect);

	CScreenRect ToScreen(HWND hWindow) const;
};

struct CWindowRelativeRect : public CRectangle
{
	CWindowRelativeRect() = default;
	CWindowRelativeRect(const CScreenRect& Rect, const CScreenRect& ParentRect);
};

typedef CRectangle CRect;