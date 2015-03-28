#include "stdafx.h"
#include "DataExchange.h"

#include "Dialog.h"
#include "Exceptions.h"
#include "ServiceProviders.h"

CDataExchange::CDataExchange(CDialog& Dialog, bool bSave)
	: m_Dialog(Dialog),
	m_bSave(bSave),
	m_LastControlID(0)
{

}

bool CDataExchange::IsStoring() const 
{ 
	return m_bSave; 
}

void CDataExchange::Fail()
{
	if (m_LastControlID)
		if (HWND hDlgItem = GetDlgItem(m_Dialog, m_LastControlID))
			::SetFocus(hDlgItem);
	throw CDataExchangeException();
}

void CDataExchange::Fail(LPCTSTR Message, UINT nID)
{
	OpenMessageBox(Message, MB_OK);
	m_LastControlID = nID;
	Fail();
}

void CDataExchange::DoDataExchange(UINT nIDC, CWindow& Window)
{
	if (!Window)
	{
		if (HWND hDlgItem = ::GetDlgItem(m_Dialog, nIDC))
		{
			if (!Window.SubclassWindow(hDlgItem))
				throw CDataExchangeException("Could not subclass dialog item.");
		}
	}
}

void CDataExchange::DoCheckBoxExchange(UINT nIDC, bool& bChecked)
{
	if (IsStoring())
		bChecked = m_Dialog.IsDlgButtonChecked(nIDC) == BST_CHECKED;
	else
		m_Dialog.CheckDlgButton(nIDC, bChecked ? BST_CHECKED : BST_UNCHECKED);
}

void DDX_CheckBox(CDataExchange& DataExchange, UINT nIDC, bool& bChecked)
{
	DataExchange.DoCheckBoxExchange(nIDC, bChecked);
}