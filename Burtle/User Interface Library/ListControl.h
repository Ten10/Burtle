#pragma once
#include "Control.h"
#include "HeaderControl.h"
#include "ListControlEventArgs.h"

class CListControl : public CWindow
{
	CHeaderCtrl m_HeaderCtrl;

	void PreSubclassWindow() override;
public:
	CNotifyReflectEvent<LVN_BEGINLABELEDIT, CLabelBeginEditArgs> OnBeginLabelEdit { m_EventMap };
	CNotifyReflectEvent<LVN_ENDLABELEDIT, CLabelEndEditArgs> OnEndLabelEdit { m_EventMap };
	CNotifyReflectEvent<LVN_GETDISPINFO, CGetItemInfoArgs> OnGetItemInfo { m_EventMap };
	CNotifyReflectEvent<LVN_ODCACHEHINT, CCacheHintArgs> OnCacheHint { m_EventMap };
	CNotifyReflectEvent<NM_CUSTOMDRAW, CListControlCustomDrawArgs> OnCustomDraw { m_EventMap };

	bool Create(DWORD dwStyle, const CRect& Rect, const CWindow& Parent);

	UINT GetColumnWidth(UINT nColumn) const;
	bool SetColumnWidth(UINT nColumn, UINT nWidth);
	::std::wstring GetItemText(UINT nColumn, UINT nRow) const;
	bool GetItemRect(UINT nRow, CRect& Rect, DWORD ItemPortion) const;
	bool GetSubItemRect(UINT nRow, UINT nColumn, CRect& Rect, DWORD ItemPortion) const;
	DWORD GetItemData(UINT nItem) const;
	bool GetCheck(UINT nItem) const;
	UINT GetItemCount() const;
	void SetItemCount(UINT nCount, DWORD Flags = LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);

	UINT InsertColumn(UINT nColumn, LPCTSTR Name);

	bool SubItemHitTest(LVHITTESTINFO& Info) const;
	bool EnsureVisible(UINT nItem, bool bPartial);
	bool SetItemState(UINT nRow, DWORD dwStateMask, DWORD dwState);
	bool RedrawItems(UINT nFirstItemIndex, UINT nLastItemIndex);

	DWORD SetExtendedStyle(DWORD dwStyleEx);
	DWORD GetExtendedStyle() const;

	CHeaderCtrl* GetHeaderCtrl() const;
};

typedef CListControl CListCtrl;