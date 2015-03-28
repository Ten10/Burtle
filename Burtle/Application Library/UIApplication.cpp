#include "stdafx.h"
#include "UIApplication.h"
#include "Registry.h"

CUIApplication::CUIApplication(const CString& Name)
	: m_Name(Name)
{
}

CUIApplication::~CUIApplication()
{
}

bool CUIApplication::InitInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	return true;
}

int CUIApplication::ExitInstance()
{
	return 0;
}

HINSTANCE CUIApplication::GetInstance() const
{
	return m_hInstance;
}

UINT CUIApplication::OpenMessageBox(const CString& sText, UINT uType) const
{
	return ::MessageBox(HWND_DESKTOP, sText.c_str(), m_Name.c_str(), uType);
}

CString CUIApplication::GetString(const CString& sKey, const CString& Name)
{
	CString sPath = _T("Software\\") + m_Name + _T("\\") + sKey + _T("\\");
	Registry::CurrentUser user(sPath);
	return user.GetValue(Name, _T(""));
}

void CUIApplication::WriteString(const CString& sKey, const CString& Name, const CString& Value)
{
	CString sPath = _T("Software\\") + m_Name + _T("\\") + sKey + _T("\\");
	Registry::CurrentUser user(sPath);
	bool bSuccess = user.SetValue(Name, Value);
	ASSERT(bSuccess);
}