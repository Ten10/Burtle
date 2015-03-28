#pragma once
#include "Control.h"

class CComboBox : public CControl
{
public:
	bool Create(DWORD dwStyle, const CRect& Rect, const CWindow& Parent);

	bool AddString(const ::std::wstring& sItemText);
	bool InsertString(const ::std::wstring& sItemText, UINT nPosition);
	bool SelectString(UINT nStartIndex, const ::std::wstring& sItemText);
	void SetCurSel(UINT nItemIndex);
	UINT GetCurSel() const;

	UINT GetCount() const;

	DWORD_PTR GetItemData(UINT nItem) const;
	bool SetItemData(UINT nItem, DWORD_PTR Data);

	CControlReflectEvent<CBN_SELCHANGE> OnSelectionChanged{ m_EventMap };
	CControlReflectEvent<CBN_CLOSEUP> OnCloseUp { m_EventMap };
};