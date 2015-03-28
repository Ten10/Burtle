#pragma once
#include "Color.h"
#include "DrawingObject.h"

class CBrushBase
{
	HBRUSH& m_hBrush;
	virtual bool Attach(HBRUSH hBrush) = 0;
	virtual void Detach() = 0;
public:
	CBrushBase(HBRUSH& hBrush)
		: m_hBrush(hBrush)
	{
	}

	bool CreateSolidBrush(COLORREF Color)
	{
		return Attach(::CreateSolidBrush(Color));
	}

	bool CreateSolidBrush(eColor Color)
	{
		return CreateSolidBrush(static_cast<COLORREF>(Color));
	}
};

using CBrush = CDrawingObjectHelper<CBrushBase, HBRUSH>;