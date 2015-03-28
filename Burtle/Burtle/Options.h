#pragma once

class CArchive;
struct COptions
{
	CStringA BugzillaURI;

	bool bAuthenticationRequired;
	CStringA Username;
	CProtectedStringA Password;
	UINT MaxNumberOfBugsPerSearch = 100;

	void Serialize(CArchive& Archive);
	
	static COptions FromTortoiseString(const CString& String);
	static CString ToTortoiseString(const COptions& Options);
};