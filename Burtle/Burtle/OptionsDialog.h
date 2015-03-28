// OptionsDialog.h : Declaration of the COptionsDialog

#pragma once

#include "resource.h"       // main symbols
#include "Options.h"

//#include <atlhost.h>

//using namespace ATL;

// COptionsDialog

class COptionsDialog : public CDialog
{
	COptions m_Options;

	void InitializeEventMap() override;
public:

	COptionsDialog(const CString& sOptions);
	~COptionsDialog();

	enum { IDD = IDD_OPTIONSDIALOG };

	CDialog::eResult DoModal(HWND hWndParent);

	bool OnInitDialog() override;
	void DoDataExchange(CDataExchange& DataExchange) override;

	void OnBnClickedAuthenticationCheck(const CWindow* pSender);

public:
	CString GetOptions() const;
};


