#pragma once
#include "String.h"
#include "..\User Interface Library\ServiceProviders.h"

class CUIApplication : public CMessageBoxProvider, public CInstanceProvider
{
protected:
	CString m_Name;
	HINSTANCE m_hInstance = nullptr;
public:
	CUIApplication(const CString& Name);
	CUIApplication(const CUIApplication& Other) = delete;
	~CUIApplication();

	virtual bool InitInstance(HINSTANCE hInstance);
	virtual int ExitInstance();

	HINSTANCE GetInstance() const;
	UINT OpenMessageBox(const CString& sText, UINT uType) const override;

	CString GetString(const CString& sKey, const CString& Name);
	void WriteString(const CString& sKey, const CString& Name, const CString& Value);
};