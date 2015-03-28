#pragma once
#include "Window.h"

class CControl : public CWindow
{
public:
	virtual bool Create(DWORD dwStyle, const CRect& Rect, const CWindow& Parent) = 0;
	virtual tstring GetSelectedItem() const
	{
		return GetText();
	};
	virtual bool SetSelectedItem(const tstring& sItem)
	{
		return SetText(sItem);
	}
};