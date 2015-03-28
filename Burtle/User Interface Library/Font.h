#pragma once
#include "DrawingObject.h"

class CFontBase
{
	HFONT& m_hFont;
	virtual bool Attach(HFONT hFont) = 0;
	virtual void Detach() = 0;
public:
	CFontBase(HFONT& hFont)
		: m_hFont(hFont)
	{
	}

	bool GetLogFont(LOGFONT& LogFont)
	{
		return ::GetObject(m_hFont, sizeof(LOGFONT), &LogFont) == sizeof(LOGFONT);
	}

	bool CreateFontIndirect(LOGFONT& LogFont)
	{
		return Attach(::CreateFontIndirect(&LogFont));
	}
};

using CFont = CDrawingObjectHelper<CFontBase, HFONT>;