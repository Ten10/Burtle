// BurtleProvider.cpp : Implementation of CBurtleProvider

#include "stdafx.h"
#include "BurtleProvider.h"
#include "OptionsDialog.h"
#include "BugSelectionDialog.h"
#include "Options.h"
#include "FieldGetSet.h"
#include "Util.h"
#include "dllmain.h"

#include <Shlwapi.h>

static const CString CommonDelimiters = _T(" ,");

static vector<int> ParseIntList(const CString& Text, bool bAllowZero, bool bAllowNegativeNumber)
{
	vector<int> result;

	auto list = Split(Text, CommonDelimiters);
	for (auto& text : list)

	{
		int number = _wtoi(text.c_str());
		if ((number || bAllowZero) && (number >= 0 || bAllowNegativeNumber))
			result.push_back(number);
	}
	
	return result;
}

BugFunctionParams::CUpdateBugsParam CBugModificationInfo::ToUpdateBugParam(const CBugInfo& BugInfo) const
{
	int nBugID = BugInfo.GetBugID();
	BugFunctionParams::CUpdateBugsParam param;
	param.UpdatedFields["ids"][0] = nBugID;
	for (const pair<CStringA, CString>& p : FieldNameToNewFieldValue)
	{
		CStringA fieldName = p.first;
		XmlRpcValue::ValueStruct bugFields = BugInfo.BugInfo;
		auto it = bugFields.find(fieldName);
		if (it == bugFields.end())
		{
			CStringA redundantPrefixes[] = { "rep_", "bug_" };
			auto itPrefix = find_if(begin(redundantPrefixes), end(redundantPrefixes), [=](const CStringA& Prefix) { return fieldName.find(Prefix) != CStringA::npos; });
			if (itPrefix != end(redundantPrefixes))
			{
				fieldName.erase(0, itPrefix->length());
				it = bugFields.find(fieldName);
			}
		}
		if (it != bugFields.end())
			param.UpdatedFields[fieldName] = BugInfo.BugInfo[fieldName];
		else
		{
			string sError = "Could not locate field ";
			sError += p.first + " in bug #" + to_string(nBugID) + ".";
			throw CException(sError);
		}
	}

	FieldNameAndValueVector fieldNameAndValue;
	fieldNameAndValue.reserve(FieldNameToNewFieldValue.size());
	for (const pair<CStringA, CString>& p : FieldNameToNewFieldValue)
		fieldNameAndValue.emplace_back(p.first, CT2A(p.second));

	CGetSetFieldsVisitor visitor(fieldNameAndValue, false);
	param.Accept(visitor);
	return param;
}

static const GUID BugTraqProvider_Category = { 0x3494fa92, 0xb139, 0x4730, {0x95, 0x91, 0x1, 0x13, 0x5d, 0x5e, 0x78, 0x31 } };

BEGIN_REGISTRY_MAP(CBurtleProvider)
	REGISTRY_KEY_VALUE(ClassID, &CLSID_BurtleProvider)
	REGISTRY_KEY_VALUE(ComponentID, _T("BurtleProvider"))
	REGISTRY_KEY_VALUES(ImplementedCategories, BugTraqProvider_Category)
END_REGISTRY_MAP()

// CBurtleProvider

CBurtleProvider::CBurtleProvider()
{
}

/*HRESULT CBurtleProvider::FinalConstruct()
{
    return S_OK;
}

void CBurtleProvider::FinalRelease()
{
}*/

HRESULT STDMETHODCALLTYPE CBurtleProvider::ValidateParameters(
    /* [in] */ HWND hParentWnd,
    /* [in] */ BSTR parameters,
    /* [retval][out] */ VARIANT_BOOL *valid)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
    // Don't bother validating the parameters yet.
    *valid = VARIANT_TRUE;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CBurtleProvider::GetLinkText(
    /* [in] */ HWND hParentWnd,
    /* [in] */ BSTR parameters,
    /* [retval][out] */ BSTR *linkText)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*linkText = SysAllocString(L"Burtle...");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CBurtleProvider::GetCommitMessage(
    /* [in] */ HWND hParentWnd,
    /* [in] */ BSTR parameters,
    /* [in] */ BSTR commonRoot,
    /* [in] */ SAFEARRAY * pathList,
    /* [in] */ BSTR originalMessage,
    /* [retval][out] */ BSTR *newMessage)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

    BSTR bugID = SysAllocString(_T(""));
    BSTR* bugIDOut = nullptr;
    HRESULT hr = GetCommitMessage2(hParentWnd, parameters, NULL, commonRoot, pathList, originalMessage, bugID, bugIDOut, NULL, NULL, newMessage);
    SysFreeString(bugID);
    return hr;
}

HRESULT STDMETHODCALLTYPE CBurtleProvider::GetCommitMessage2(
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
    /* [retval][out] */ BSTR *newMessage)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	auto options = COptions::FromTortoiseString(CString(parameters));
	if (options.BugzillaURI.empty() || (options.AuthenticationRequired && options.Username.empty()))
	{
		BurtleMessageBox(_T("Missing bugzilla connection info."), MB_OK);
		return S_FALSE;
	}

	m_upServer.reset(new CServer(options.BugzillaURI));
	if (!m_upServer->Initialize())
	{
		BurtleMessageBox(_T("Could not initialize bugzilla connection."), MB_OK);
		return S_FALSE;
	}

	if (options.AuthenticationRequired)
	{
		try
		{
			m_upServer->Login(options.Username, options.Password.GetString());
		}
		catch (CBugzillaException Exception)
		{
			BurtleMessageBox(CA2T(Exception.ErrorMessage), MB_OK);
			return S_FALSE;
		}
	}

	CBugSelectionDialog dlg(*m_upServer, m_RecentQuery, m_BugIDToModificationInfo, options.MaxNumberOfBugsPerSearch);
	CDialog::eResult result = dlg.DoModal(CBugSelectionDialog::IDD, hParentWnd);

	if (result != CDialog::eResult::OK)
		return S_FALSE;

    CString propName = _T("bugtraq:issueIDs");
    wstringstream stream;
	for (auto it = m_BugIDToModificationInfo.begin(); it != m_BugIDToModificationInfo.end(); ++it)
	{
		if (it != m_BugIDToModificationInfo.begin())
			stream << _T(", ");
		stream << it->first;
	}
	CString propValue = stream.str();

	if (revPropNames && revPropValues)
	{
		{
			SAFEARRAYBOUND bounds = {1, 0};
			SAFEARRAY* psa = SafeArrayCreate(VT_BSTR, 1, &bounds);
			BSTR* strArray;
			SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));
			strArray[0] = ::SysAllocString(propName.c_str());
			SafeArrayUnaccessData(psa);
			*revPropNames = psa;
		}
		{
			SAFEARRAYBOUND bounds = {1, 0};
			SAFEARRAY* psa = SafeArrayCreate(VT_BSTR, 1, &bounds);
			BSTR* strArray;
			SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));
			strArray[0] = ::SysAllocString(propValue.c_str());
			SafeArrayUnaccessData(psa);
			*revPropValues = psa;
		}
	}
	if (bugIDOut)
		*bugIDOut = ::SysAllocString(propValue.c_str());
	if (newMessage)
	{
		CString sMessage = originalMessage;
		sMessage += propValue;
		*newMessage = ::SysAllocString(sMessage.c_str());
	}
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CBurtleProvider::CheckCommit (
     /* [in] */ HWND hParentWnd,
     /* [in] */ BSTR parameters,
     /* [in] */ BSTR commonURL,
     /* [in] */ BSTR commonRoot,
     /* [in] */ SAFEARRAY * pathList,
     /* [in] */ BSTR commitMessage,
     /* [out, retval] */ BSTR * errorMessage)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CBurtleProvider::OnCommitFinished (
    /* [in] */ HWND hParentWnd,
    /* [in] */ BSTR commonRoot,
    /* [in] */ SAFEARRAY * pathList,
    /* [in] */ BSTR logMessage,
    /* [in] */ ULONG revision,
    /* [out, retval] */ BSTR * error)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!m_upServer)
		return S_FALSE;

	CStringA sCommitMessage(CT2A(logMessage));
	std::wstringstream sAllErrors;
	for (auto& bugInfo : m_RecentQuery.BugInfos)
	{
		auto it = m_BugIDToModificationInfo.find(bugInfo.GetBugID());
		if (it == m_BugIDToModificationInfo.end())
			continue;

		CBug bug(*m_upServer, bugInfo);
		CString sErrorType;
		CStringA sError;
		try
		{
			sErrorType = _T("Update");
			bug.Update(it->second.ToUpdateBugParam(bugInfo));
		}
		catch (exception& e)
		{
			sError = e.what();
		}
		// Currently chosen behavior is to only append the commit if we managed to update the bug.
		if (sError.empty())
		{
			try
			{
				// Not sure about behavior with unicode comments.
				sErrorType = _T("Append Comment");
				bug.AppendComment(sCommitMessage);
			}
			catch (exception& e)
			{
				sError = e.what();
			}
		}
		if (!sError.empty() && error)
		{
			sAllErrors << _T("Bug #") << to_wstring(it->first) << _T(" ") << sErrorType << _T(": ");
			sAllErrors << CA2T(sError.c_str()) << std::endl;
		}
	}

	if (error)
		*error = ::SysAllocString(sAllErrors.str().c_str());

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CBurtleProvider::HasOptions(
                   /* [out, retval] */ VARIANT_BOOL *ret)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
    *ret = VARIANT_TRUE;
    return S_OK;
}

// this method is called if HasOptions() returned true before.
// Use this to show a custom dialog so the user doesn't have to
// create the parameters string manually
HRESULT STDMETHODCALLTYPE CBurtleProvider::ShowOptionsDialog(
                          /* [in] */ HWND hParentWnd,
                          /* [in] */ BSTR parameters,
                          /* [out, retval] */ BSTR * newparameters
                          )
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString params = parameters;
	COptionsDialog dialog(params);
	if (dialog.DoModal(hParentWnd) == CDialog::eResult::OK)
	{
		CString sOptions = dialog.GetOptions();
		*newparameters = ::SysAllocString(sOptions.c_str());
		return S_OK;
	}
	else
	{
		*newparameters = ::SysAllocString(params.c_str());
		return S_FALSE;
	}
}