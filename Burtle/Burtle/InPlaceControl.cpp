#include "stdafx.h"
#include "InPlaceControl.h"

#include "ListCtrlEx.h"

CInPlaceControlImpl::CInPlaceControlImpl(CControl& Control, CListCtrlEx& ListCtrl, bool bAllowEmptyValues)
	: m_ListCtrl(ListCtrl),
	m_Control(Control)
{
	m_RowIndex = -1;
	m_ColumnIndex = -1;
	m_bCancel = false;
	m_bAllowEmptyValues = bAllowEmptyValues;
}

CInPlaceControlImpl::~CInPlaceControlImpl()
{
}

void CInPlaceControlImpl::OnClose(const CWindow* pSender)
{
	Close();
}

void CInPlaceControlImpl::OnKillFocus(const CWindow* pSender, CFocusEventArgs& Args)
{
	m_Control.Default();
	if (m_Control)
		m_Control.PostMessage(WM_CLOSE);
}

void CInPlaceControlImpl::OnChar(const CWindow*, CCharEventArgs& Args)
{
	if (Args.Char != VK_ESCAPE && Args.Char != VK_RETURN)
		m_Control.Default();
	else if (Args.Char == VK_ESCAPE)
		Cancel();
	else
		RequestClose();
}

void CInPlaceControlImpl::InitializeEventMap()
{
	CEvent<WM_CLOSE, CNoEventArgs>(m_Control.GetEventMap()) += CEventHandler<CNoEventArgs>(&CInPlaceControlImpl::OnClose, *this);
	m_Control.OnKillFocus += CEventHandler<CFocusEventArgs>(&CInPlaceControlImpl::OnKillFocus, *this);
	m_Control.OnChar += CEventHandler<CCharEventArgs>(&CInPlaceControlImpl::OnChar, *this);
}

void CInPlaceControlImpl::RequestClose()
{
	m_ListCtrl.SetFocus();
}

void CInPlaceControlImpl::Close()
{
	if (!m_Control)
		return;

	CString sValue = m_Control.GetSelectedItem();
	if (m_bCancel || (!m_bAllowEmptyValues && sValue.empty()))
		sValue = m_OriginalValue;

	// Send Notification to parent of edit ctrl
	if (CWindow* pParent = m_Control.GetParent())
	{
		LV_DISPINFO dispinfo;
		dispinfo.hdr.hwndFrom = m_ListCtrl;
		dispinfo.hdr.idFrom = m_ListCtrl.GetDlgCtrlID();
		dispinfo.hdr.code = LVN_ENDLABELEDIT;

		dispinfo.item.mask = LVIF_TEXT;
		dispinfo.item.iItem = m_RowIndex;
		dispinfo.item.iSubItem = m_ColumnIndex;
		dispinfo.item.pszText = LPTSTR(sValue.c_str());
		dispinfo.item.cchTextMax = (int)sValue.length();

		pParent->SendMessage(WM_NOTIFY, pParent->GetDlgCtrlID(), (LPARAM)&dispinfo);
	}
	m_Control.DestroyWindow();
}

void CInPlaceControlImpl::Cancel()
{
	m_bCancel = true;
	RequestClose();
}

bool CInPlaceControlImpl::Show(DWORD dwStyle, int nRowIndex, int nColumnIndex, const CString& sOriginalValue)
{
	if ((m_ListCtrl.GetStyle() & LVS_REPORT) != LVS_REPORT)
		return false;

	if (m_Control)
		Close();

	m_bCancel = false;
	m_RowIndex = nRowIndex;
	m_ColumnIndex = nColumnIndex;
	m_OriginalValue = sOriginalValue;

	CRect subItemRect;
	if (!m_ListCtrl.GetSubItemRect(nRowIndex, nColumnIndex, subItemRect, LVIR_LABEL))
		return false;

	bool bCreated = m_Control.Create(dwStyle | WS_VISIBLE, subItemRect, m_ListCtrl/*, 2*/) ? true : false;
	if (bCreated)
	{
		m_Control.SetSelectedItem(sOriginalValue);
		m_Control.SetFocus();
	}
	return bCreated;
}

CInPlaceComboBox::CInPlaceComboBox(CListCtrlEx& ListCtrl)
	: CControlWrapper<CComboBox>(ListCtrl)
{

}

void CInPlaceComboBox::InitializeEventMap()
{
	__super::InitializeEventMap();
	m_Impl.InitializeEventMap();
	CComboBox::OnCloseUp += CEventHandler<CNoEventArgs>(&CInPlaceComboBox::OnCloseUp, *this);
}

void CInPlaceComboBox::OnCloseUp(const CWindow*)
{
	m_Impl.RequestClose();
}

CInPlaceDateTimeControl::CInPlaceDateTimeControl(CListCtrlEx& ListCtrl)
	: CControlWrapper<CDateTimeControl>(ListCtrl, true)
{

}

CString CInPlaceDateTimeControl::GetSelectedItem() const
{
	CString sResult;
	SYSTEMTIME systemTime = { 0 };
	if (GetTime(systemTime))
		sResult = CTime::ToString(CTime::FromTime(systemTime));
	return sResult;
}

bool CInPlaceDateTimeControl::SetSelectedItem(const CString& sItem)
{
	SYSTEMTIME systemTime = CTime::FromString(sItem);
	return SetTime(systemTime, !sItem.empty());
}

bool CInPlaceDateTimeControl::Show(DWORD dwStyle, int nRowIndex, int nColumnIndex, const CString& sOriginalValue)
{
	if (!__super::Show(dwStyle | DTS_SHOWNONE, nRowIndex, nColumnIndex, sOriginalValue))
		return false;

	SetFormat(CTime::GetUserDateTimeFormat());
	return true;
}