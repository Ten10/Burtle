#pragma once
#include "Control.h"

class CEdit : public CControl
{
public:
	bool Create(DWORD dwStyle, const CRect& Rect, const CWindow& Parent);

	bool SetSelectedItem(const tstring& sItem) override;
	void SetSelection(int Start, int End);
};