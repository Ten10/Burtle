#pragma once

#include <Wincrypt.h>
#include "String.h"
#include "Archive.h"
#include <vector>

#pragma comment(lib, "Crypt32.lib")

class CCryptKey
{
	HCRYPTKEY m_hKey = NULL;
	bool SetKeyParam(DWORD dwParam, const BYTE* pData, DWORD dwFlags);
public:
	CCryptKey() = default;
	CCryptKey(HCRYPTKEY hKey);
	CCryptKey(const CCryptKey&) = delete;
	CCryptKey(CCryptKey&&);
	CCryptKey& operator=(const CCryptKey&) = delete;
	CCryptKey& operator=(CCryptKey&&);
	~CCryptKey();

	operator bool() const { return m_hKey ? true : false; };

	bool SetMode(DWORD Mode);
	bool SetInitializationVector(const ::std::vector<BYTE>& Data);
};

class CCryptProvider
{
public:
	struct CProviderInfo
	{
		CString Name;
		DWORD Type;
		DWORD Flags;
	};

protected:
	HCRYPTPROV m_hProvider;
	CProviderInfo m_Info;
	CString m_KeyContainerName;

	CCryptKey ImportKey(const BYTE* pKeyData, DWORD dwKeyDataLength, DWORD dwKeySize, DWORD dwFlags) const;
public:
	CCryptProvider(const CString& KeyContainerName, std::initializer_list<CProviderInfo> PossibleProviders);
	virtual ~CCryptProvider();

	operator HCRYPTPROV() const { return m_hProvider; }
};

struct AesKey
{
	enum eAesKeySize : UINT
	{
		AesKeySize128 = 16,
		AesKeySize192 = 24,
		AesKeySize256 = 32
	};

	static const BYTE AesBlockSize = 16;

	PUBLICKEYSTRUC PublicKeyStruct;
	DWORD dwKeyLength = 0;
	BYTE Key[AesKeySize256];

	AesKey(const ::std::vector<BYTE>& Key);
};

class CAesProvider : protected CCryptProvider
{
public:
	CAesProvider(const CString& KeyContainerName);

	CCryptKey ImportKey(const AesKey& Key, DWORD dwFlags = 0) const;
};

class CAes
{
	CCryptKey m_Key;
	CAesProvider m_Provider;
public:
	CAes(const CString& KeyContainerName);
	~CAes();

	bool SetKeyAndIV(const ::std::vector<BYTE>& Key, const ::std::vector<BYTE>& InitializationVector);

	bool MaxEncryptedDataSize(size_t DataSize, size_t& EncryptedDataSize);
	bool MaxDecryptedDataSize(size_t DataSize, size_t& DecyptedDataSize);

	::std::vector<BYTE> Encrypt(const ::std::vector<BYTE>& Data);
	::std::vector<BYTE> Decrypt(const ::std::vector<BYTE>& Data);
};

class CProtectedData
{
protected:
	size_t m_Length = 0;
	::std::vector<BYTE> m_Data;

	struct CDataScopeGuard
	{
		CProtectedData& Data;
		CDataScopeGuard(CProtectedData& ProtectedData)
			: Data(ProtectedData)
		{
			ProtectedData.Unprotect();
		}
		~CDataScopeGuard()
		{
			Data.Protect();
		}
	};

private:
	friend struct CDataScopeGuard;

	void Protect();
	void Unprotect();
public:
	CProtectedData() = default;
	CProtectedData(const ::std::vector<BYTE>& Data);
	
	void Serialize(CArchive& Archive);
};

class CProtectedStringA : public CProtectedData
{
public:
	CProtectedStringA() = default;
	CProtectedStringA(const CStringA& Text);

	CStringA GetString();
};

class CDataExchange;
void DDX(CDataExchange& DataExchange, UINT nIDC, CProtectedStringA& Value);