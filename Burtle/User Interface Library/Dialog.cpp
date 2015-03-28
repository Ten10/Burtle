#include "stdafx.h"
#include "Dialog.h"

#include "DataExchange.h"
#include "DeviceContext.h"
#include "Exceptions.h"
#include "ServiceProviders.h"

INT_PTR CALLBACK CDialog::DialogProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	bool bHandled = false;
	LRESULT result = 0;
	if (nMsg == WM_INITDIALOG)
	{
		if (CDialog* pDialog = (CDialog*)lParam)
		{
			pDialog->Attach(hWnd);
			result = pDialog->OnInitDialog();
			pDialog->ShowWindow(SW_SHOW);
			bHandled = true;
		}
	}
	else if (CWindowInfo* pInfo = (CWindowInfo*)GetWindowLongPtr(hWnd, GWLP_USERDATA))
	{
		if (auto pDialog = dynamic_cast<CDialog*>(pInfo->pWindow))
			result = pDialog->HandleMessage(hWnd, nMsg, wParam, lParam, bHandled);
	}

	if (bHandled)
	{
		if (nMsg == WM_CHARTOITEM
			|| nMsg == WM_COMPAREITEM
			|| nMsg == WM_CTLCOLORBTN
			|| nMsg == WM_CTLCOLORDLG
			|| nMsg == WM_CTLCOLOREDIT
			|| nMsg == WM_CTLCOLORLISTBOX
			|| nMsg == WM_CTLCOLORSCROLLBAR
			|| nMsg == WM_CTLCOLORSTATIC
			|| nMsg == WM_INITDIALOG
			|| nMsg == WM_QUERYDRAGICON
			|| nMsg == WM_VKEYTOITEM)
			return result;
		else
		{
			SetWindowLongPtr(hWnd, DWLP_MSGRESULT, result);
			return true;
		}
	}
	return false;
}

CDialog::CDialog()
{

}

#include "Commdlg.h"
#pragma comment(lib, "Comdlg32.lib")
CDialog::eResult CDialog::DoModal(ResourceIDType ResourceID, HWND hParentWnd)
{
	m_bContinue = true;
	m_Result = eResult::None;

	HINSTANCE hInstance = GetInstance();
	HWND hWnd = CreateDialogParam(hInstance, MAKEINTRESOURCE(ResourceID), hParentWnd, &DialogProc, (LPARAM)this);
	if (!hWnd)
		return m_Result;

	Attach(hWnd);

	bool bParentDisabled = false;
	if (hParentWnd == HWND_DESKTOP || hParentWnd == HWND_MESSAGE)
		hParentWnd = GetDesktopWindow();

	if (hParentWnd != GetDesktopWindow())
	{
		bParentDisabled = true;
		::EnableWindow(hParentWnd, false);
		::SetActiveWindow(m_hWnd);
	}

	CScreenRect dialogRect(m_hWnd);
	CScreenRect parentRect(hParentWnd);
	CPoint ptDialogCenter = dialogRect.Center();
	CPoint ptParentCenter = parentRect.Center();

	dialogRect.OffsetRect(ptDialogCenter - ptParentCenter);
	SetWindowPos(m_hWnd, nullptr, dialogRect.left, dialogRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	PumpMessages(m_bContinue);

	if (bParentDisabled)
	{
		::EnableWindow(hParentWnd, true);
		::SetActiveWindow(hParentWnd);
	}

	Detach();
	return m_Result;
}

void CDialog::EndDialog(eResult Result)
{
	m_Result = Result;
	DestroyWindow();
}

bool CDialog::OnInitDialog()
{
	UpdateData(false);
	return true;
}

void CDialog::InitializeEventMap()
{
	__super::InitializeEventMap();
	CButtonPressEvent<IDOK>(m_EventMap) += CEventHandler<CNoEventArgs>(&CDialog::OnOKClicked, *this);
	CButtonPressEvent<IDCANCEL>(m_EventMap) += CEventHandler<CNoEventArgs>(&CDialog::OnCancelClicked, *this);
}

void CDialog::OnOK()
{
	if (UpdateData(true))
		EndDialog(eResult::OK);
}

void CDialog::OnCancel()
{
	EndDialog(eResult::Cancel);
}

void CDialog::DoDataExchange(CDataExchange& DataExchange)
{

}

void CDialog::OnFinalMessage()
{
	__super::OnFinalMessage();
	m_bContinue = false;
}

bool CDialog::UpdateData(bool bSave)
{
	bool bUpdated = true;
	CDataExchange dataExchange(*this, bSave);
	try
	{
		DoDataExchange(dataExchange);
	}
	catch (CDataExchangeException& exception)
	{
		bUpdated = false;
		exception = exception;
	}

	return bUpdated;
}

bool CDialog::SetDlgItemValue(UINT nIDC, LPCWSTR lpszText)
{
	return ::SetDlgItemTextW(m_hWnd, nIDC, lpszText) ? true : false;
}

bool CDialog::SetDlgItemValue(UINT nIDC, LPCSTR lpszText)
{
	return ::SetDlgItemTextA(m_hWnd, nIDC, lpszText) ? true : false;
}

bool CDialog::SetDlgItemValue(UINT nIDC, UINT& Value)
{
	return ::SetDlgItemInt(m_hWnd, nIDC, Value, false) ? true : false;
}

bool CDialog::SetDlgItemValue(UINT nIDC, INT& Value)
{
	return ::SetDlgItemInt(m_hWnd, nIDC, Value, true) ? true : false;
}

UINT CDialog::GetDlgItemValue(UINT nIDC, WCHAR* pBuffer, UINT BufferSize)
{
	return ::GetDlgItemTextW(m_hWnd, nIDC, pBuffer, BufferSize);
}

UINT CDialog::GetDlgItemValue(UINT nIDC, CHAR* pBuffer, UINT BufferSize)
{
	return ::GetDlgItemTextA(m_hWnd, nIDC, pBuffer, BufferSize);
}

void CDialog::GetDlgItemValue(UINT nIDC, ::std::wstring& Value)
{
	HWND hDlgItem = GetDlgItem(m_hWnd, nIDC);
	int length = GetWindowTextLength(hDlgItem);
	ASSERT(length >= 0);
	if (length < 0)
		return;
	if (!length)
	{
		Value.clear();
		return;
	}

	vector<TCHAR> buffer(length + 1, 0);
	GetDlgItemValue(nIDC, &buffer[0], (UINT)buffer.size());
	Value = &buffer[0];
};

void CDialog::GetDlgItemValue(UINT nIDC, ::std::string& Value)
{
	HWND hDlgItem = GetDlgItem(m_hWnd, nIDC);
	int length = GetWindowTextLength(hDlgItem);
	ASSERT(length >= 0);
	if (length < 0)
		return;
	if (!length)
	{
		Value.clear();
		return;
	}

	vector<CHAR> buffer(length + 1, 0);
	GetDlgItemValue(nIDC, &buffer[0], (UINT)buffer.size());
	Value.assign(buffer.begin(), buffer.end());
}

bool CDialog::GetDlgItemValue(UINT nIDC, UINT& Value)
{
	BOOL bSuccess = false;
	Value = ::GetDlgItemInt(m_hWnd, nIDC, &bSuccess, false);
	return bSuccess ? true : false;
}

bool CDialog::GetDlgItemValue(UINT nIDC, INT& Value)
{
	BOOL bSuccess = false;
	Value = ::GetDlgItemInt(m_hWnd, nIDC, &bSuccess, true);
	return bSuccess ? true : false;
}

void CDialog::CheckDlgButton(UINT nIDC, UINT nCheck)
{
	::CheckDlgButton(m_hWnd, nIDC, nCheck);
}

UINT CDialog::IsDlgButtonChecked(UINT nIDC)
{
	return ::IsDlgButtonChecked(m_hWnd, nIDC);
}

void CDialog::EnableDlgItem(UINT nIDC, bool bEnable)
{
	if (HWND hWnd = GetDlgItem(m_hWnd, nIDC))
		::EnableWindow(hWnd, bEnable);
}

void CDialog::OnOKClicked(const CWindow*)
{
	OnOK();
}

void CDialog::OnCancelClicked(const CWindow*)
{
	OnCancel();
}