// Burtle.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "Burtle_i.h"
#include "dllmain.h"
#include "Burtle.h"
#include "BurtleProvider.h"
#include "COM.h"

ULONG CBurtleFactory::ServerLocks = 0;

HRESULT STDMETHODCALLTYPE CBurtleFactory::CreateInstance(
	/* [unique][in] */ IUnknown *pUnkOuter,
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void **ppvObject)
{
	HRESULT hr = E_UNEXPECTED;
	if (riid == IID_IBugTraqProvider || riid == IID_IBugTraqProvider2)
	{
		hr = E_OUTOFMEMORY;
		if (auto pObject = CComObject<CBurtleProvider>::CreateInstance())
		{
			hr = pObject->QueryInterface(riid, ppvObject);
			pObject->Release();
		}
	}
	else
		hr = E_NOTIMPL;
	return hr;
}

HRESULT STDMETHODCALLTYPE CBurtleFactory::LockServer(
	/* [in] */ BOOL fLock)
{
	if (fLock)
		InterlockedIncrement(&ServerLocks);
	else
		InterlockedDecrement(&ServerLocks);
	return S_OK;
}

// Used to determine whether the DLL can be unloaded by OLE.
STDAPI DllCanUnloadNow(void)
{
	HRESULT hr = E_UNEXPECTED;
	if (CBurtleFactory::CanUnload() && CComObjectImpl::CanUnload())
		hr = S_OK;
	else 
		hr = S_FALSE;
	return hr;
}

// Returns a class factory to create an object of the requested type.
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
{
	HRESULT hr = E_UNEXPECTED;
	if (rclsid == CLSID_BurtleProvider && riid == IID_IClassFactory)
	{
		CBurtleFactory* pFactory = CComObject<CBurtleFactory>::CreateInstance();
		if (!pFactory)
			hr = E_OUTOFMEMORY;
		else 
		{
			hr = pFactory->QueryInterface(riid, ppv);
			pFactory->Release();
		}
	}
	else 
		hr = CLASS_E_CLASSNOTAVAILABLE;
	return hr;
}

// DllRegisterServer - Adds entries to the system registry.
STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	return theModule.Register();
}

// DllUnregisterServer - Removes entries from the system registry.
STDAPI DllUnregisterServer(void)
{
	return theModule.Unregister();
}

// DllInstall - Adds/Removes entries to the system registry per user per machine.
STDAPI DllInstall(BOOL bInstall, _In_opt_  LPCWSTR pszCmdLine)
{
	HRESULT hr = E_FAIL;
	/*static const wchar_t szUserSwitch[] = L"user";

	if (pszCmdLine != NULL)
	{
		if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
		{
			//ATL::AtlSetPerUserRegistration(true);
		}
	}*/

	if (bInstall)
	{	
		hr = DllRegisterServer();
		if (FAILED(hr))
		{
			DllUnregisterServer();
		}
	}
	else
	{
		hr = DllUnregisterServer();
	}

	return hr;
}