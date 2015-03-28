#pragma once
#include <string>

typedef ::std::wstring tstring;
typedef unsigned int UINT;

struct CMessageBoxProvider
{
	static const CMessageBoxProvider* pProvider;

	CMessageBoxProvider()
	{
		pProvider = this;
	}
	virtual ~CMessageBoxProvider()
	{
		pProvider = nullptr;
	}

	virtual UINT OpenMessageBox(const tstring& Text, UINT uType) const = 0;
};

struct CInstanceProvider
{
	static const CInstanceProvider* pProvider;
	CInstanceProvider()
	{
		pProvider = this;
	}
	virtual ~CInstanceProvider()
	{
		pProvider = nullptr;
	}

	virtual HINSTANCE GetInstance() const = 0;
};

HINSTANCE GetInstance();
void OpenMessageBox(const tstring& Text, UINT uType);