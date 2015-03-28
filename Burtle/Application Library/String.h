#pragma once
#include <string>
#include <WTypes.h>

typedef ::std::wstring CString;
typedef ::std::string CStringA;

class CComBSTR
{
	BSTR m_Value = nullptr;
	void Attach(LPCTSTR lpszText);
	void Detach();
public:
	CComBSTR() = default;
	CComBSTR(const CString& Text);
	~CComBSTR();

	operator BSTR() { return m_Value; }

	CComBSTR& operator=(const CString& Text);
};

CStringA CW2A(const CString& Text);
CString CA2W(const CStringA& Text);

#ifdef UNICODE
#define CT2A(x) CW2A(x)
#define CT2W(x) x
#define CA2T(x) CA2W(x)
#else
#define CT2A(x) x
#define CT2W(x) CA2W(x)
#define CA2T(x) x
#endif