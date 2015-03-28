#include "stdafx.h"
#include "ServiceProviders.h"

const CMessageBoxProvider* CMessageBoxProvider::pProvider = nullptr;
const CInstanceProvider* CInstanceProvider::pProvider = nullptr;

HINSTANCE GetInstance()
{
	HINSTANCE hInstance = nullptr;
	if (CInstanceProvider::pProvider)
		hInstance = CInstanceProvider::pProvider->GetInstance();
	return hInstance;
}

void OpenMessageBox(const wstring& sText, UINT uType)
{
	if (CMessageBoxProvider::pProvider)
		CMessageBoxProvider::pProvider->OpenMessageBox(sText, uType);
}