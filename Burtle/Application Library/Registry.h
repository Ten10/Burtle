#pragma once
#include "String.h"

class CRegistryKeyImpl;
class CRegistryKey
{
	CRegistryKeyImpl* m_pImpl;
public:
	CRegistryKey(HKEY Key, const CString& Path, const CString& RootValue);
	~CRegistryKey();

	bool SetValue(const CString& ValueName, const CString& Value);
	CString GetValue(const CString& ValueName, const CString& DefaultValue);
	bool DeleteValue(const CString& ValueName);

	CRegistryKey AddKey(const CString& KeyName, const CString& RootValue);
	bool DeleteKey(const CString& KeyName);

	operator bool() const;
};


template <HKEY ConstKey>
class CRegistryKeyT : public CRegistryKey
{
public:
	CRegistryKeyT(const CString& Path)
		: CRegistryKey(ConstKey, Path, _T(""))
	{
	}
};

namespace Registry
{
	typedef CRegistryKeyT<HKEY_CURRENT_USER> CurrentUser;
	typedef CRegistryKeyT<HKEY_LOCAL_MACHINE> LocalMachine;
	typedef CRegistryKeyT<HKEY_CLASSES_ROOT> ClassesRoot;
};