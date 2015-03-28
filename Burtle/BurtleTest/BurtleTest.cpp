
// BurtleTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BurtleTest.h"
#include "BurtleTestDlg.h"
#include "../Burtle/Burtle_i.c"
#include "..\Application Library\ExtendedHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	if (theApp.InitInstance(hInstance))
	{
		MSG msg = { 0 };
		while (GetMessage(&msg, nullptr, 0, 0) > 0)
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		
		theApp.ExitInstance();
	}
	return 0;
}

HRESULT __stdcall MyCoCreateInstance(LPCTSTR szDllName, IN REFCLSID rclsid, IUnknown* pUnkOuter, IN REFIID riid, OUT LPVOID FAR* ppv)
{
	HRESULT hr = REGDB_E_KEYMISSING;

	HMODULE hDll = ::LoadLibrary(szDllName);
	if (hDll == 0)
	return hr;

	typedef HRESULT (__stdcall *pDllGetClassObject)(IN REFCLSID rclsid, 
					IN REFIID riid, OUT LPVOID FAR* ppv);

	pDllGetClassObject GetClassObject =	(pDllGetClassObject)::GetProcAddress(hDll, "DllGetClassObject");
	if (GetClassObject == 0)
	{
		::FreeLibrary(hDll);
		return hr;
	}

	IClassFactory *pIFactory;

	hr = GetClassObject(rclsid, IID_IClassFactory, (LPVOID *)&pIFactory);

	if (!SUCCEEDED(hr))
	return hr;

	hr = pIFactory->CreateInstance(pUnkOuter, riid, ppv);
	pIFactory->Release();

	return hr;
}

// CBurtleTestApp

// CBurtleTestApp construction

CBurtleTestApp::CBurtleTestApp()
	: CUIApplication(_T("BurtleTest"))
{
}

CBurtleTestApp::~CBurtleTestApp()
{
}


// The one and only CBurtleTestApp object

EHOOKSTRUCT hookSetFocusMsg;
CBurtleTestApp theApp;


HWND WINAPI MySetFocus(HWND hWnd)
{
	typedef decltype(SetFocus) SetFocusType;
	return ((SetFocusType *)hookSetFocusMsg.adr_new_api)(hWnd);
}

// CBurtleTestApp initialization

bool CBurtleTestApp::InitInstance(HINSTANCE hInstance)
{
	if (!__super::InitInstance(hInstance))
		return false;
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	if (FAILED(CoInitialize(nullptr)))
		return false;

	//InstallEHookEx(&SetFocus, &hookSetFocusMsg, &MySetFocus);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Burtle-Test"));

	HRESULT hr = REGDB_E_CLASSNOTREG;// CoCreateInstance(CLSID_BurtleProvider, NULL, CLSCTX_ALL, IID_IBugTraqProvider2, (LPVOID *)&m_pBugTraqProvider);

	if (hr == REGDB_E_CLASSNOTREG)
	{
		hr = MyCoCreateInstance(_T("Burtle.dll"), CLSID_BurtleProvider, NULL, IID_IBugTraqProvider2, (LPVOID *)&m_pBugTraqProvider);
	}
	//ATLASSERT(SUCCEEDED(hr));


	CBurtleTestDlg dlg;
	CDialog::eResult response = dlg.DoModal(CBurtleTestDlg::IDD, HWND_DESKTOP);
	if (response == CDialog::eResult::OK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (response == CDialog::eResult::Cancel)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (response == CDialog::eResult::None)
	{
		//TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
	}


	CoUninitialize();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return false;
}

int CBurtleTestApp::ExitInstance()
{
	/*UninstallEHook(&hookPostMsg);
	UninstallEHook(&hookPostThreadMsg);*/
	//return __super::ExitInstance();
	return 0;
}

IBugTraqProvider2& CBurtleTestApp::GetBugTraqProvider()
{
	ENSURE(m_pBugTraqProvider);
	return *m_pBugTraqProvider;
}