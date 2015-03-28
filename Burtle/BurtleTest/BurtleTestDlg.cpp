
// BurtleTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BurtleTest.h"
#include "BurtleTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBurtleTestDlg dialog

CBurtleTestDlg::CBurtleTestDlg()
{
	//m_hIcon = LoadIcon(IDR_MAINFRAME);
}

void CBurtleTestDlg::DoDataExchange(CDataExchange& DataExchange)
{
	__super::DoDataExchange(DataExchange);
}

// CBurtleTestDlg message handlers

bool CBurtleTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString sOptions = theApp.GetString(_T("Workspace"), _T("Options"));
	m_Parameters = ::SysAllocString(sOptions.c_str());

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, true);			// Set big icon
	SetIcon(m_hIcon, false);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBurtleTestDlg::OnPaint(const CWindow* pSender, CPaintArgs& Args)
{
	if (IsIconic())
	{
		if (!Args.pPaintDC)
			return;

		CDC& dc = *Args.pPaintDC;

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(static_cast<HDC>(dc)), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect = GetClientRect();
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.Draw(x, y, m_hIcon);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
void CBurtleTestDlg::OnQueryDragIcon(const CWindow* pSender, CQueryDragIconArgs& Args)
{
	Args.DragIcon = m_hIcon;
}

void CBurtleTestDlg::OnBnClickedOptionsButton(const CWindow* pSender)
{
	auto& provider = theApp.GetBugTraqProvider();

	BSTR newParametes;
	VARIANT_BOOL bHasOptions;
	if (SUCCEEDED(provider.HasOptions(&bHasOptions)) && bHasOptions == VARIANT_TRUE)
	{
		HRESULT hr = provider.ShowOptionsDialog(m_hWnd, m_Parameters, &newParametes);
		if (hr == S_OK)
		{
			m_Parameters = newParametes;
			CString params = m_Parameters;
			theApp.WriteString(_T("Workspace"), _T("Options"), params);
		}
		if (newParametes)
			::SysFreeString(newParametes);
	}
}

void CBurtleTestDlg::OnBnClickedGetCommitMessageButton(const CWindow* pSender)
{
	auto& provider = theApp.GetBugTraqProvider();
	provider.GetCommitMessage2(m_hWnd, m_Parameters, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void CBurtleTestDlg::OnBnClickedOnCommitFinishedButton(const CWindow* pSender)
{
	BSTR error = nullptr;
	auto& provider = theApp.GetBugTraqProvider();
	provider.OnCommitFinished(m_hWnd, nullptr, nullptr, nullptr, 100, &error);
	if (error)
	{
		::MessageBox(m_hWnd, error, _T("Commit Finished"), MB_OK);
		::SysFreeString(error);
	}
}

void CBurtleTestDlg::InitializeEventMap()
{
	__super::InitializeEventMap();
	CWindowBase::OnPaint += CEventHandler<CPaintArgs>(&CBurtleTestDlg::OnPaint, *this);
	CWindowBase::OnQueryDragIcon += CEventHandler<CQueryDragIconArgs>(&CBurtleTestDlg::OnQueryDragIcon, *this);
	CButtonPressEvent<IDC_OPTIONS_BUTTON>(m_EventMap) += CEventHandler<CNoEventArgs>(&CBurtleTestDlg::OnBnClickedOptionsButton, *this);
	CButtonPressEvent<IDC_GET_COMMIT_MESSAGE_BUTTON>(m_EventMap) += CEventHandler<CNoEventArgs>(&CBurtleTestDlg::OnBnClickedGetCommitMessageButton, *this);
	CButtonPressEvent<IDC_ON_COMMIT_FINISHED_BUTTON>(m_EventMap) += CEventHandler<CNoEventArgs>(&CBurtleTestDlg::OnBnClickedOnCommitFinishedButton, *this);
}