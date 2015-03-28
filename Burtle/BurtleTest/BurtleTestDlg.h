
// BurtleTestDlg.h : header file
//

#pragma once


// CBurtleTestDlg dialog
class CBurtleTestDlg : public CDialog
{
// Construction
public:
	CBurtleTestDlg();	// standard constructor

// Dialog Data
	enum { IDD = IDD_BURTLETEST_DIALOG };

protected:
	void DoDataExchange(CDataExchange& DX) override;	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CComBSTR m_Parameters;

	// Generated message map functions
	bool OnInitDialog() override;
	void OnPaint(const CWindow* pSender, CPaintArgs& Args);
	void OnQueryDragIcon(const CWindow* pSender, CQueryDragIconArgs& Args);

	void OnBnClickedOptionsButton(const CWindow* pSender);
	void OnBnClickedGetCommitMessageButton(const CWindow* pSender);
	void OnBnClickedOnCommitFinishedButton(const CWindow* pSender);

	void InitializeEventMap() override;
};
