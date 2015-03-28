#pragma once

#define NO_VTABLE __declspec(novtable)

class CComRegisterHelperBase;
class CComModule : public CInstanceProvider, public CMessageBoxProvider
{
	CLSID m_CLSID;
	CString m_Name;

public:
	HINSTANCE hInstance = nullptr;

	CComModule(const CString& Name, const CLSID& ModuleCLSID);

	HINSTANCE GetInstance() const;

	CString GetPath() const;
	CString GetProgramID(const CString& ComponentID) const;
	GUID GetGUID() const;

	UINT OpenMessageBox(const CString& Text, UINT uType) const override;

	HRESULT Register();
	HRESULT Unregister();
};

class CComObjectImpl
{
	ULONG m_RefCount;
	static ULONG ObjectCount;

public:
	CComObjectImpl();
	~CComObjectImpl();

	ULONG AddRef();
	ULONG Release();

	static bool CanUnload();
};

template <typename T>
class CComObject : public T
{
	CComObjectImpl m_Impl;
	CComObject()
	{

	}
	~CComObject()
	{

	}
public:
	ULONG STDMETHODCALLTYPE AddRef() override
	{
		return m_Impl.AddRef();
	}

	ULONG STDMETHODCALLTYPE Release() override
	{
		ULONG lResult = m_Impl.Release();
		if (lResult == 0)
			delete this;
		return lResult;
	}

	static T* CreateInstance()
	{
		T* pObject = new (nothrow) CComObject<T>();
		if (pObject)
			pObject->AddRef();
		return pObject;
	}
};

class CComRegisterHelperBase
{
	const GUID m_ClassID;
	vector<GUID> m_ImplementedCategories;
	const CString m_ComponentID;
	const CString m_ComponentVersion;

	CComRegisterHelperBase* m_pNext;
public:
	CComRegisterHelperBase(const GUID* pClassID, const TCHAR* pComponentID, const TCHAR* pComponentVersion, initializer_list<GUID>::iterator Begin, initializer_list<GUID>::iterator End);
	virtual ~CComRegisterHelperBase();

	HRESULT Register(const CComModule& Module);
	HRESULT Unregister(const CComModule& Module);

	static HRESULT RegisterAllClasses(const CComModule& Module);
	static HRESULT UnregisterAllClasses(const CComModule& Module);
};

template <typename T>
class CComRegisterHelper : protected CComRegisterHelperBase
{
public:
	static const GUID* ClassID;
	static const initializer_list<GUID> ImplementedCategories;
	static const TCHAR* ComponentID;
	static const TCHAR* Version;

	CComRegisterHelper()
		: CComRegisterHelperBase(ClassID, ComponentID, Version, ImplementedCategories.begin(), ImplementedCategories.end())
	{
	}
};

template <typename T> const GUID* CComRegisterHelper<T>::ClassID = nullptr;
template <typename T> const TCHAR* CComRegisterHelper<T>::ComponentID = nullptr;
template <typename T> const TCHAR* CComRegisterHelper<T>::Version = _T("1");

#define BEGIN_COM_MAP() HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) { if (!ppvObject) return E_INVALIDARG;
#define COM_INTERFACE_ENTRY(interfce) if (riid == __uuidof(interfce)) { AddRef(); *ppvObject = this; return S_OK; }
#define END_COM_MAP() return E_NOINTERFACE; }

#define DECLARE_REGISTRY_MAP(theclass) static CComRegisterHelper<theclass> ComRegisterHelper;

#define BEGIN_REGISTRY_MAP(theclass) typedef CComRegisterHelper<theclass> ClassRegisterHelperType;
#define REGISTRY_KEY_VALUE(key, value) decltype(ClassRegisterHelperType::key) ClassRegisterHelperType::key = value;
#define REGISTRY_KEY_VALUES(key, ...) decltype(ClassRegisterHelperType::key) ClassRegisterHelperType::key = { __VA_ARGS__ };
#define END_REGISTRY_MAP() static ClassRegisterHelperType ClassRegisteration;