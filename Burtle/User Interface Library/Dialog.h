#pragma once

#include "Window.h"

class CDataExchange;
class CDialog : public CWnd
{
	static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
public:
	typedef unsigned int ResourceIDType;
	enum class eResult
	{
		None = 0,
		OK = IDOK,
		Cancel = IDCANCEL,
	};
	eResult m_Result;
	bool m_bContinue;

	void InitializeEventMap() override;

	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange& DataExchange);
	void OnFinalMessage() override;
public:
	CDialog();
	CDialog(const CDialog&) = delete;
	eResult DoModal(ResourceIDType ResourceID, HWND hParentWnd);
	void EndDialog(eResult Result);

	virtual bool OnInitDialog();
	bool UpdateData(bool bSave);

	bool SetDlgItemValue(UINT nIDC, LPCWSTR lpszText);
	bool SetDlgItemValue(UINT nIDC, LPCSTR lpszText);
	bool SetDlgItemValue(UINT nIDC, UINT& Value);
	bool SetDlgItemValue(UINT nIDC, INT& Value);
	template <typename T>
	typename ::std::enable_if<::std::is_same<::std::wstring, T>::value || ::std::is_same<::std::string, T>::value, bool>::type SetDlgItemValue(UINT nIDC, const T& Text)
	{
		return SetDlgItemValue(nIDC, Text.c_str());
	}

	UINT GetDlgItemValue(UINT nIDC, WCHAR* pBuffer, UINT BufferSize);
	UINT GetDlgItemValue(UINT nIDC, CHAR* pBuffer, UINT BufferSize);
	void GetDlgItemValue(UINT nIDC, ::std::wstring& Value);
	void GetDlgItemValue(UINT nIDC, ::std::string& Value);
	bool GetDlgItemValue(UINT nIDC, UINT& Value);
	bool GetDlgItemValue(UINT nIDC, INT& Value);

	void CheckDlgButton(UINT nIDC, UINT nCheck);
	UINT IsDlgButtonChecked(UINT nIDC);

	void EnableDlgItem(UINT nIDC, bool bEnable);

private:
	void OnOKClicked(const CWindow* pSender);
	void OnCancelClicked(const CWindow* pSender);
};