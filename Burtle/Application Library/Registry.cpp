#include "stdafx.h"
#include "Registry.h"

#pragma comment(lib, "Advapi32.lib")

using namespace std;

TCHAR* Convert(BYTE* pBuffer)
{
	return (TCHAR*)pBuffer;
}

template <typename T>
static typename enable_if<is_pod<T>::value, void>::type AssignBuffer(T& Value, BYTE* Buffer, DWORD Size)
{
	memcpy_s(Value, sizeof(T), Buffer, Size)
}

template <typename T>
static typename enable_if<!is_pod<T>::value, void>::type AssignBuffer(T& Value, BYTE* Buffer, DWORD Size)
{
	Value = Convert(Buffer);
}

class CRegistryKeyImpl
{
	HKEY m_hKey;

	template <typename T>
	T GetValueT(const CString& ValueName, const T& Default) const
	{
		if (!m_hKey)
			return Default;

		T result = Default;
		DWORD dwType = 0;
		DWORD dwLength = 0;
		LSTATUS status = RegQueryValueEx(m_hKey, ValueName.c_str(), nullptr, &dwType, nullptr, &dwLength);
		if (status == ERROR_SUCCESS && dwLength)
		{
			unique_ptr<BYTE[]> spData(new BYTE[dwLength]);
			status = RegQueryValueEx(m_hKey, ValueName.c_str(), nullptr, &dwType, spData.get(), &dwLength);
			if (status == ERROR_SUCCESS)
				AssignBuffer(result, spData.get(), dwLength);
		}
		return result;
	}

public:
	CRegistryKeyImpl(HKEY BaseKey, const CString& FullPath, REGSAM DesiredAccess)
		: m_hKey(nullptr)
	{
		LSTATUS status = RegCreateKeyEx(BaseKey, FullPath.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, DesiredAccess, nullptr, &m_hKey, nullptr);
		if (status != ERROR_SUCCESS)
			m_hKey = NULL;
	}

	~CRegistryKeyImpl()
	{
		if (m_hKey)
			RegCloseKey(m_hKey);
	}

	operator bool() const
	{
		return m_hKey ? true : false;
	}

	HKEY GetKey() const
	{
		return m_hKey;
	}

	template <typename T>
	bool SetValue(const TCHAR* pValueName, const T& Value)
	{
		if (!m_hKey)
			return false;

		static_assert(sizeof(T) <= sizeof(DWORD), "unsupported pod type");
		DWORD value = Value;
		LSTATUS status = RegSetValueEx(m_hKey, pValueName, 0, , (const BYTE*)&value, sizeof(DWORD));
		return status == ERROR_SUCCESS;
	}

	template <>
	bool SetValue(const TCHAR* pValueName, const CString& Value)
	{
		if (!m_hKey)
			return false;

		size_t length = Value.length() * sizeof(CString::traits_type::char_type);
		ENSURE(length <= MAXDWORD);
		DWORD dwLength = static_cast<DWORD>(length);
		LSTATUS status = RegSetValueEx(m_hKey, pValueName, 0, REG_SZ, (const BYTE*)Value.c_str(), dwLength);
		return status == ERROR_SUCCESS;
	}

	template <typename T>
	T GetValue(const TCHAR* pValueName, const T& DefaultValue) const
	{
		return GetValueT(pValueName, DefaultValue);
	}

	bool DeleteValue(const CString& Name)
	{
		return RegDeleteValue(m_hKey, Name.c_str()) == ERROR_SUCCESS;
	}

	bool DeleteKey(const CString& SubKey)
	{
		return RegDeleteTree(m_hKey, SubKey.c_str()) == ERROR_SUCCESS;
	}
};

CRegistryKey::CRegistryKey(HKEY Key, const CString& Path, const CString& RootValue)
{
	m_pImpl = new CRegistryKeyImpl(Key, Path, KEY_ALL_ACCESS);
	if (m_pImpl && !RootValue.empty())
		m_pImpl->SetValue(nullptr, RootValue);
}

CRegistryKey::~CRegistryKey()
{
	if (m_pImpl)
		delete m_pImpl;
}

bool CRegistryKey::SetValue(const CString& ValueName, const CString& Value)
{
	ENSURE(m_pImpl);
	bool bResult = *m_pImpl;
	if (bResult)
		bResult = m_pImpl->SetValue(ValueName.c_str(), Value);
	return bResult;
}

CString CRegistryKey::GetValue(const CString& ValueName, const CString& DefaultValue)
{
	ENSURE(m_pImpl);
	return m_pImpl->GetValue(ValueName.c_str(), DefaultValue);
}

bool CRegistryKey::DeleteValue(const CString& ValueName)
{
	ENSURE(m_pImpl);
	return m_pImpl->DeleteValue(ValueName);
}

CRegistryKey CRegistryKey::AddKey(const CString& KeyName, const CString& RootValue)
{
	ENSURE(m_pImpl);
	return CRegistryKey(m_pImpl->GetKey(), KeyName, RootValue);
}

bool CRegistryKey::DeleteKey(const CString& KeyName)
{
	ENSURE(m_pImpl);
	return m_pImpl->DeleteKey(KeyName);
}

CRegistryKey::operator bool() const
{
	ENSURE(m_pImpl);
	return *m_pImpl ? true : false;
}