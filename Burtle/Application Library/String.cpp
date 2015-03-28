#include "stdafx.h"
#include "String.h"

#include <codecvt>
#include <OleAuto.h>

#pragma comment(lib, "OleAut32.lib")

using namespace std;

void CComBSTR::Attach(LPCTSTR lpszText)
{
	Detach();
	m_Value = ::SysAllocString(lpszText);
}

void CComBSTR::Detach()
{
	if (m_Value)
	{
		::SysFreeString(m_Value);
		m_Value = nullptr;
	}
}

CComBSTR::CComBSTR(const CString& Text)
{
	Attach(Text.c_str());
}

CComBSTR::~CComBSTR()
{
	Detach();
}

CComBSTR& CComBSTR::operator=(const CString& Text)
{
	Attach(Text.c_str());
	return *this;
}

CStringA CW2A(const CString& Text)
{
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes(Text);
}

CString CA2W(const CStringA& Text)
{
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.from_bytes(Text);
}