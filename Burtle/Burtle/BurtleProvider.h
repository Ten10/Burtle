// BurtleProvider.h : Declaration of the CBurtleProvider

#pragma once
#include "resource.h"       // main symbols
#include "COM.h"

#include "Burtle_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

struct CBugzillaQuery
{
	BugFunctionParams::CBugSearchParam SearchParam;
	vector<CBugInfo> BugInfos;

	CBugzillaQuery& operator=(CBugzillaQuery&& Other)
	{
		SearchParam = move(Other.SearchParam);
		BugInfos = move(Other.BugInfos);
		return *this;
	}
};

struct CBugModificationInfo
{
	map<CStringA, CString> FieldNameToNewFieldValue;

	BugFunctionParams::CUpdateBugsParam ToUpdateBugParam(const CBugInfo& BugInfo) const;
};

// CBurtleProvider

class NO_VTABLE CBurtleProvider :
	/*public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBurtleProvider, &CLSID_BurtleProvider>,*/
	public IBugTraqProvider2
{
	unique_ptr<CServer> m_upServer;
	CBugzillaQuery m_RecentQuery;
	map<int, CBugModificationInfo> m_BugIDToModificationInfo;

public:
	CBurtleProvider();

/*#ifdef _WIN64
DECLARE_REGISTRY_RESOURCEID(IDR_BURTLEPROVIDER_X64)
#else
DECLARE_REGISTRY_RESOURCEID(IDR_BURTLEPROVIDER_X86)
#endif*/

	DECLARE_REGISTRY_MAP(CBurtleProvider)

	BEGIN_COM_MAP()
		COM_INTERFACE_ENTRY(IBugTraqProvider)
		COM_INTERFACE_ENTRY(IBugTraqProvider2)
	END_COM_MAP()

	//DECLARE_PROTECT_FINAL_CONSTRUCT()

	/*HRESULT FinalConstruct();
	void FinalRelease();*/

public:
	virtual HRESULT STDMETHODCALLTYPE ValidateParameters(
		/* [in] */ HWND hParentWnd,
		/* [in] */ BSTR parameters,
		/* [retval][out] */ VARIANT_BOOL *valid);

	virtual HRESULT STDMETHODCALLTYPE GetLinkText(
		/* [in] */ HWND hParentWnd,
		/* [in] */ BSTR parameters,
		/* [retval][out] */ BSTR *linkText);

	virtual HRESULT STDMETHODCALLTYPE GetCommitMessage(
		/* [in] */ HWND hParentWnd,
		/* [in] */ BSTR parameters,
		/* [in] */ BSTR commonRoot,
		/* [in] */ SAFEARRAY * pathList,
		/* [in] */ BSTR originalMessage,
		/* [retval][out] */ BSTR *newMessage);

	virtual HRESULT STDMETHODCALLTYPE GetCommitMessage2(
		/* [in] */ HWND hParentWnd,
		/* [in] */ BSTR parameters,
		/* [in] */ BSTR commonURL,
		/* [in] */ BSTR commonRoot,
		/* [in] */ SAFEARRAY * pathList,
		/* [in] */ BSTR originalMessage,
		/* [in] */ BSTR bugID,
		/* [out]*/ BSTR * bugIDOut,
		/* [out]*/ SAFEARRAY ** revPropNames,
		/* [out]*/ SAFEARRAY ** revPropValues,
		/* [retval][out] */ BSTR *newMessage);

	virtual HRESULT STDMETHODCALLTYPE CheckCommit (
		/* [in] */ HWND hParentWnd,
		/* [in] */ BSTR parameters,
		/* [in] */ BSTR commonURL,
		/* [in] */ BSTR commonRoot,
		/* [in] */ SAFEARRAY * pathList,
		/* [in] */ BSTR commitMessage,
		/* [out, retval] */ BSTR * errorMessage);

	virtual HRESULT STDMETHODCALLTYPE OnCommitFinished (
		/* [in] */ HWND hParentWnd,
		/* [in] */ BSTR commonRoot,
		/* [in] */ SAFEARRAY * pathList,
		/* [in] */ BSTR logMessage,
		/* [in] */ ULONG revision,
		/* [out, retval] */ BSTR * error);

	virtual HRESULT STDMETHODCALLTYPE HasOptions(
		/* [out, retval] */ VARIANT_BOOL *ret           // Whether the provider provides options
		);

	// this method is called if HasOptions() returned true before.
	// Use this to show a custom dialog so the user doesn't have to
	// create the parameters string manually
	virtual HRESULT STDMETHODCALLTYPE ShowOptionsDialog(
		/* [in] */ HWND hParentWnd,                 // Parent window for the options dialog
		/* [in] */ BSTR parameters,
		/* [out, retval] */ BSTR * newparameters    // the parameters string
		);

};

//OBJECT_ENTRY_AUTO(__uuidof(BurtleProvider), CBurtleProvider)
