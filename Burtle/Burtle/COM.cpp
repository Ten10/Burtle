#include "stdafx.h"
#include "COM.h"

const BYTE CLSID_STRING_SIZE = 39;

static CComRegisterHelperBase* ClassListStart = nullptr;

static CString CLSIDToString(const CLSID& Clsid)
{
	CString sResult;
	LPOLESTR wszCLSID = nullptr;
	HRESULT hr = StringFromCLSID(Clsid, &wszCLSID);
	if (SUCCEEDED(hr))
	{
		sResult = wszCLSID;
		CoTaskMemFree(wszCLSID);
	}
	return sResult;
}

CComModule::CComModule(const wstring& Name, const CLSID& ModuleCLSID)
	: m_Name(Name),
	m_CLSID(ModuleCLSID)
{
}

HINSTANCE CComModule::GetInstance() const
{
	return hInstance;
}

CString CComModule::GetPath() const
{
	CString sResult;
	TCHAR szModule[MAX_PATH] = { 0 };
	DWORD dwResult = GetModuleFileName((HMODULE)hInstance, szModule, sizeof(szModule) / sizeof(WCHAR));
	if (dwResult)
		sResult = szModule;
	return sResult;
}

CString CComModule::GetProgramID(const CString& ComponentID) const
{
	return m_Name + _T(".") + ComponentID;
}

GUID CComModule::GetGUID() const
{
	return m_CLSID;
}

UINT CComModule::OpenMessageBox(const CString& Text, UINT uType) const
{
	return ::MessageBox(HWND_DESKTOP, Text.c_str(), m_Name.c_str(), uType);
}

HRESULT CComModule::Register()
{
	HRESULT hr = CComRegisterHelperBase::RegisterAllClasses(*this);
	if (FAILED(hr))
		Unregister();
	return hr;
}

HRESULT CComModule::Unregister()
{
	return CComRegisterHelperBase::UnregisterAllClasses(*this);
}

ULONG CComObjectImpl::ObjectCount = 0;

CComObjectImpl::CComObjectImpl()
	: m_RefCount(0)
{
	InterlockedIncrement(&ObjectCount);
}

CComObjectImpl::~CComObjectImpl()
{
	InterlockedDecrement(&ObjectCount);
}

ULONG CComObjectImpl::AddRef()
{
	return InterlockedIncrement(&m_RefCount);
}

ULONG CComObjectImpl::Release()
{
	return InterlockedDecrement(&m_RefCount);
}

bool CComObjectImpl::CanUnload()
{
	return ObjectCount == 0;
}

CComRegisterHelperBase::CComRegisterHelperBase(const GUID* pClassID, const TCHAR* pComponentID, const TCHAR* pComponentVersion, initializer_list<GUID>::iterator Begin, initializer_list<GUID>::iterator End)
	: m_ClassID(pClassID ? *pClassID : GUID_NULL),
	m_ImplementedCategories(Begin, End),
	m_ComponentID(pComponentID ? pComponentID : _T("")),
	m_ComponentVersion(pComponentVersion ? pComponentVersion : _T("")),
	m_pNext(ClassListStart)
{
	ASSERT(pClassID && pComponentID && pComponentVersion && m_ImplementedCategories.size());
	ClassListStart = this;
}

CComRegisterHelperBase::~CComRegisterHelperBase()
{
}

HRESULT CComRegisterHelperBase::Register(const CComModule& Module)
{
	Registry::ClassesRoot registry(_T("CLSID"));
	if (!registry)
		return E_ACCESSDENIED;

	CRegistryKey classRegistry = registry.AddKey(CLSIDToString(m_ClassID), m_ComponentID);
	if (!classRegistry)
		return E_ACCESSDENIED;

	CString sVersionIndependentProgID = Module.GetProgramID(m_ComponentID);
	if (!classRegistry.AddKey(L"VersionIndependentProgID", sVersionIndependentProgID))
		return E_ACCESSDENIED;

	CString sProgID = sVersionIndependentProgID + _T(".") + m_ComponentVersion;
	if (!classRegistry.AddKey(L"ProgID", sProgID))
		return E_ACCESSDENIED;

	CRegistryKey inprocServer32 = classRegistry.AddKey(L"InprocServer32", Module.GetPath());
	if (!inprocServer32 || !inprocServer32.SetValue(L"ThreadingModel", _T("Apartment")))
		return E_ACCESSDENIED;

	CRegistryKey implementedCategories = classRegistry.AddKey(L"Implemented Categories", L"");
	if (!implementedCategories)
		return E_ACCESSDENIED;

	for (const CLSID& implementedCategoryCLSID : m_ImplementedCategories)

	{
		if (!implementedCategories.AddKey(CLSIDToString(implementedCategoryCLSID), _T("")))
			return E_ACCESSDENIED;
	}

	if (!classRegistry.AddKey(_T("TypeLib"), CLSIDToString(Module.GetGUID())))
		return E_ACCESSDENIED;

	return S_OK;
}

HRESULT CComRegisterHelperBase::Unregister(const CComModule& Module)
{
	CString sFullPath = _T("CLSID");
	Registry::ClassesRoot root(sFullPath);
	return root.DeleteKey(CLSIDToString(m_ClassID)) ? S_OK : E_ACCESSDENIED;
}

HRESULT CComRegisterHelperBase::RegisterAllClasses(const CComModule& Module)
{
	for (auto pRegisterClassObject = ClassListStart; pRegisterClassObject; pRegisterClassObject = pRegisterClassObject->m_pNext)
	{
		HRESULT hr = pRegisterClassObject->Register(Module);
		if (FAILED(hr))
			return hr;
	}
	return S_OK;
}

HRESULT CComRegisterHelperBase::UnregisterAllClasses(const CComModule& Module)
{
	for (auto pRegisterClassObject = ClassListStart; pRegisterClassObject; pRegisterClassObject = pRegisterClassObject->m_pNext)
	{
		if (!pRegisterClassObject)
			break;
		HRESULT hr = pRegisterClassObject->Unregister(Module);
		if (FAILED(hr))
			return hr;
	}
	return S_OK;
}