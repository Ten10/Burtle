#include "stdafx.h"
#include "ListCtrlEx.h"

#include "Util.h"

CListCtrlEx::CListCtrlEx()
	: m_InPlaceComboBox(*this),
	m_InPlaceEditBox(*this),
	m_InPlaceDateTimeControl(*this)
{
	m_EditBoxStyle = ES_WANTRETURN | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT | ES_NOHIDESEL | WS_CHILD | WS_VISIBLE;
	m_ComboBoxStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | 
							CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;
	m_DateTimeControlStyle = WS_BORDER | WS_CHILD | WS_VISIBLE;
}

CListCtrlEx::~CListCtrlEx()
{
}

void CListCtrlEx::InitializeEventMap()
{
	__super::InitializeEventMap();
	OnHorizontalScroll += CEventHandler<CScrollEventArgs>(&CListCtrlEx::OnHScroll, *this);
	OnVerticalScroll += CEventHandler<CScrollEventArgs>(&CListCtrlEx::OnVScroll, *this);
	OnLeftButtonDown += CEventHandler<CButtonPressEventArgs>(&CListCtrlEx::OnLButtonDown, *this);
	CListCtrl::OnBeginLabelEdit += CEventHandler<CLabelBeginEditArgs>(&CListCtrlEx::OnBeginLabelEdit, *this);
}

/////////////////////////////////////////////////////////////////////////////
// CComboListCtrl message handlers

bool CListCtrlEx::ShowInPlaceEdit(UINT RowIndex, UINT ColumnIndex, bool bShow)
{
	if (!EnsureVisible(RowIndex, true))
		return false;

	UINT nColumnCount = 0;
	CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
	if (!pHeaderCtrl || !pHeaderCtrl->GetItemCount(nColumnCount) || ColumnIndex >= static_cast<size_t>(nColumnCount) || GetColumnWidth(ColumnIndex) == 0)
		return false;

	auto columnType = fnGetColumnEditType(ColumnIndex);
	if (columnType == typeComboCtrl)
	{
		CString sOriginalValue = GetItemText(RowIndex, ColumnIndex);
		m_InPlaceComboBox.Show(m_ComboBoxStyle, RowIndex, ColumnIndex, sOriginalValue);
		m_InPlaceComboBox.SetFont(GetFont());
		if (fnInitializeComboBox)
			fnInitializeComboBox(m_InPlaceComboBox, RowIndex, ColumnIndex);
		m_InPlaceComboBox.SelectString(0, sOriginalValue);
	}
	else if (columnType == typeEditCtrl)
	{
		CString sOriginalValue = GetItemText(RowIndex, ColumnIndex);
		m_InPlaceEditBox.Show(m_EditBoxStyle, RowIndex, ColumnIndex, sOriginalValue);
		m_InPlaceEditBox.SetFont(GetFont());
	}
	else if (columnType == typeDateCtrl)
	{
		CString sOriginalValue = GetItemText(RowIndex, ColumnIndex);
		m_InPlaceDateTimeControl.Show(m_DateTimeControlStyle, RowIndex, ColumnIndex, sOriginalValue);
	}
	return true;
}

void CListCtrlEx::OnHScroll(const CWindow* pSender, CScrollEventArgs& Args)
{
	if (GetFocus() != m_hWnd)
		SetFocus();

	Default();
}

void CListCtrlEx::OnVScroll(const CWindow* pSender, CScrollEventArgs& Args)
{
	if (GetFocus() != m_hWnd)
		SetFocus();

	Default();
}

void CListCtrlEx::OnLButtonDown(const CWindow* pSender, CButtonPressEventArgs& Args)
{
	int columnIndex = -1;
	int rowIndex = -1;
	LVHITTESTINFO info = {0};
	info.pt = Args.Point;
	int nItem = SubItemHitTest(info);
	if (nItem == -1 || (info.flags & LVHT_ONITEM) == 0)
	{
		SetFocus();
		return;
	}

	rowIndex = info.iItem;
	columnIndex = info.iSubItem;
	if ((GetKeyState(VK_SHIFT) & 0x80) || (GetKeyState(VK_CONTROL) & 0x80))
	{
		Default();
		return;
	}

	if (info.flags & LVHT_ONITEMSTATEICON)
	{
		SetFocus();
		if (fnOnCheckBoxClicked)
			fnOnCheckBoxClicked(rowIndex);
		return;
	}

	if (rowIndex != -1)
	{
		SetItemState(rowIndex, LVGS_SELECTED, LVIS_SELECTED);
		CString strCurSelection = GetItemText(rowIndex, columnIndex);
		if (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS)			
			ShowInPlaceEdit(rowIndex, columnIndex, true);
	}
}

bool CListCtrlEx::IsReadOnly(int ColumnIndex)
{
	return fnGetColumnEditType
		&& fnGetColumnEditType(ColumnIndex) == typeReadOnly;
}

bool CListCtrlEx::IsCombo(int ColumnIndex)
{
	return fnGetColumnEditType
		&& fnGetColumnEditType(ColumnIndex) == typeComboCtrl;
}

bool CListCtrlEx::OnOK()
{
	bool bHandled = true;
	if (m_InPlaceComboBox)
		m_InPlaceComboBox.Close();
	else if (m_InPlaceEditBox)
		m_InPlaceEditBox.Close();
	else if (m_InPlaceDateTimeControl)
		m_InPlaceDateTimeControl.Close();
	else
		bHandled = false;
	return bHandled;
}

void CListCtrlEx::EnableHScroll(bool bEnable /*= true*/)
{
	if (bEnable)
	{
		m_ComboBoxStyle |= WS_HSCROLL;
	}
	else
	{
		m_ComboBoxStyle &= ~WS_HSCROLL;
	}	
}

void CListCtrlEx::EnableVScroll(bool bEnable /*= true*/)
{
	if (bEnable)
	{
		m_ComboBoxStyle |= WS_VSCROLL;
	}
	else
	{
		m_ComboBoxStyle &= ~WS_VSCROLL;
	}
}

void CListCtrlEx::ScrollToView(int iColumnIndex, /*int iOffSet, */CRect& robCellRect)
{
	CRect rcClient = GetClientRect();

	int iColumnWidth = GetColumnWidth(iColumnIndex);

	int iOffSet = 0;
	for (int iIndex_ = 0; iIndex_ < iColumnIndex; iIndex_++)
	{
		iOffSet += GetColumnWidth(iIndex_);
	}

	CSize obScrollSize;

	if (((iOffSet + robCellRect.left) < rcClient.left) || 
		((iOffSet + robCellRect.left) > rcClient.right))
	{
		obScrollSize.cx = iOffSet + robCellRect.left;
	}
	else if ((iOffSet + robCellRect.left + iColumnWidth) > rcClient.right)
	{
		obScrollSize.cx = iOffSet + robCellRect.left + iColumnWidth - rcClient.right;
	}

	Scroll(obScrollSize);
	robCellRect.left -= obScrollSize.cx;
	
	// Set the width to the column width
	robCellRect.left += iOffSet;
	robCellRect.right = robCellRect.left + iColumnWidth;
}

void CListCtrlEx::OnBeginLabelEdit(const CWindow* pSender, CLabelBeginEditArgs& Args) 
{
	Args.Cancel = IsReadOnly(Args.Column);
}
