// OptionsDialog.cpp : Implementation of COptionsDialog

#include "stdafx.h"
#include "OptionsDialog.h"

// COptionsDialog

void COptionsDialog::InitializeEventMap()
{
	__super::InitializeEventMap();
	CButtonPressEvent<IDC_AUTHENTICATION_CHECK>(m_EventMap) += CEventHandler<CNoEventArgs>(&COptionsDialog::OnBnClickedAuthenticationCheck, *this);
}

COptionsDialog::COptionsDialog(const CString& sOptions)
{
	if (!sOptions.empty())
		m_Options = COptions::FromTortoiseString(sOptions);
}

COptionsDialog::~COptionsDialog()
{
}

CDialog::eResult COptionsDialog::DoModal(HWND hWndParent)
{
	return __super::DoModal(IDD, hWndParent);
}

bool COptionsDialog::OnInitDialog()
{
	__super::OnInitDialog();

	SetDlgItemValue(IDC_URI_EDIT, m_Options.BugzillaURI.c_str());
	SetDlgItemValue(IDC_USERNAME_EDIT, m_Options.Username.c_str());
	SetDlgItemValue(IDC_PASSWORD_EDIT, m_Options.Password.GetString().c_str());
	CheckDlgButton(IDC_AUTHENTICATION_CHECK, m_Options.bAuthenticationRequired);
	OnBnClickedAuthenticationCheck(nullptr);

	return true; 
}

void COptionsDialog::DoDataExchange(CDataExchange& DataExchange)
{
	DDX(DataExchange, IDC_URI_EDIT, m_Options.BugzillaURI);
	DDX(DataExchange, IDC_USERNAME_EDIT, m_Options.Username);
	DDX(DataExchange, IDC_PASSWORD_EDIT, m_Options.Password);
	DDX(DataExchange, IDC_MAX_NUMBER_OF_BUGS_PER_SEARCH_EDIT, m_Options.MaxNumberOfBugsPerSearch);
	DDX_CheckBox(DataExchange, IDC_AUTHENTICATION_CHECK, m_Options.bAuthenticationRequired);

}

void COptionsDialog::OnBnClickedAuthenticationCheck(const CWindow* pSender)
{
	bool bAuthenticationRequired = IsDlgButtonChecked(IDC_AUTHENTICATION_CHECK) ? true : false;
	EnableDlgItem(IDC_USERNAME_EDIT, bAuthenticationRequired);
	EnableDlgItem(IDC_PASSWORD_EDIT, bAuthenticationRequired);
}

CString COptionsDialog::GetOptions() const
{
	return COptions::ToTortoiseString(m_Options);
}