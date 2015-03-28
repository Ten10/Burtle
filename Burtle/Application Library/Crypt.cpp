#include "stdafx.h"
#include "Crypt.h"
#include "..\User Interface Library\DataExchange.h"

using namespace std;

bool CCryptKey::SetKeyParam(DWORD dwParam, const BYTE* pData, DWORD dwFlags)
{
	return CryptSetKeyParam(m_hKey, dwParam, pData, dwFlags) ? true : false;
}

CCryptKey::CCryptKey(HCRYPTKEY hKey)
	: m_hKey(hKey)
{

}

CCryptKey::CCryptKey(CCryptKey&& Other)
{
	std::swap(m_hKey, Other.m_hKey);
}

CCryptKey& CCryptKey::operator = (CCryptKey&& Other)
{
	std::swap(m_hKey, Other.m_hKey);
	return *this;
}

CCryptKey::~CCryptKey()
{
	if (m_hKey)
		CryptDestroyKey(m_hKey);
}

bool CCryptKey::SetMode(DWORD Mode)
{
	return SetKeyParam(KP_MODE, (BYTE*)&Mode, 0);
}

bool CCryptKey::SetInitializationVector(const vector<BYTE>& Data)
{
	return SetKeyParam(KP_IV, Data.empty() ? nullptr : &Data[0], 0);
}

CCryptKey CCryptProvider::ImportKey(const BYTE* pKeyData, DWORD dwKeyDataLength, DWORD dwKeySize, DWORD dwFlags) const
{
	HCRYPTKEY key = 0;
	DWORD dwImportFlags = (dwKeySize << 16) | dwFlags;
	CryptImportKey(m_hProvider, pKeyData, dwKeyDataLength, 0, dwImportFlags, &key);
	return key;
}

CCryptProvider::CCryptProvider(const CString& KeyContainerName, std::initializer_list<CProviderInfo> PossibleProviders)
	: m_hProvider(0),
	m_KeyContainerName(KeyContainerName)
{
	for (auto& provider : PossibleProviders)
	{
		if (CryptAcquireContext(&m_hProvider, m_KeyContainerName.c_str(), provider.Name.c_str(), provider.Type, provider.Flags))
		{
			m_Info = provider;
			break;
		}
	}
}

CCryptProvider::~CCryptProvider()
{
	if (m_hProvider)
	{
		ASSERT(!m_Info.Name.empty());
		CryptAcquireContext(&m_hProvider, m_KeyContainerName.c_str(), m_Info.Name.c_str(), m_Info.Type, CRYPT_DELETEKEYSET);
	}
}

AesKey::AesKey(const vector<BYTE>& RawKey)
{
	ZeroMemory(&PublicKeyStruct, sizeof(PublicKeyStruct));
	ZeroMemory(Key, _countof(Key));
	PublicKeyStruct.bType = PLAINTEXTKEYBLOB;
	PublicKeyStruct.bVersion = CUR_BLOB_VERSION;
	dwKeyLength = (DWORD)RawKey.size();
	if (dwKeyLength == AesKeySize128)
		PublicKeyStruct.aiKeyAlg = CALG_AES_128;
	else if (dwKeyLength == AesKeySize192)
		PublicKeyStruct.aiKeyAlg = CALG_AES_192;
	else if (dwKeyLength == AesKeySize256)
		PublicKeyStruct.aiKeyAlg = CALG_AES_256;

	if (PublicKeyStruct.aiKeyAlg)
		memcpy_s(Key, _countof(Key), &RawKey[0], RawKey.size());
}

CAesProvider::CAesProvider(const CString& KeyContainerName)
	: CCryptProvider(KeyContainerName, 
	{
		{ MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0 },
		{ MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_NEWKEYSET },
		{ MS_ENH_RSA_AES_PROV_XP, PROV_RSA_AES, 0 },
		{ MS_ENH_RSA_AES_PROV_XP, PROV_RSA_AES, CRYPT_NEWKEYSET },
	})
{
}

CCryptKey CAesProvider::ImportKey(const AesKey& Key, DWORD dwFlags) const
{
	return __super::ImportKey((const BYTE*)&Key, sizeof(Key) - AesKey::AesKeySize256 + Key.dwKeyLength, Key.dwKeyLength, dwFlags);
}

CAes::CAes(const CString& KeyContainerName)
	: m_Provider(KeyContainerName)
{

}

CAes::~CAes()
{
}

bool CAes::SetKeyAndIV(const vector<BYTE>& Key, const vector<BYTE>& InitializationVector)
{
	m_Key = move(m_Provider.ImportKey(Key));
	if (!m_Key)
		return false;

	if (!m_Key.SetInitializationVector(InitializationVector))
		return false;

	return m_Key.SetMode(CRYPT_MODE_CBC);
}

bool CAes::MaxEncryptedDataSize(size_t DataSize, size_t& EncryptedTextSize)
{
	size_t blockCount = (DataSize / AesKey::AesBlockSize) + 1;
	EncryptedTextSize = blockCount * AesKey::AesBlockSize;
	return true;
}

bool CAes::MaxDecryptedDataSize(size_t Size, size_t& DecyptedDataSize)
{
	if (Size % AesKey::AesBlockSize) 
		return false;

	DecyptedDataSize = Size;
	return true;
}

vector<BYTE> CAes::Encrypt(const vector<BYTE>& Data)
{
	vector<BYTE> output;
	size_t encryptedDataSize;
	if (!MaxEncryptedDataSize(Data.size(), encryptedDataSize))
		return output;

	output.resize(encryptedDataSize);
	output.assign(Data.begin(), Data.end());

	DWORD dwDataLength = (DWORD)Data.size();
	DWORD dwBufferLength = (DWORD)output.size();
	if (!CryptEncrypt(m_Key, 0, true, 0, &output[0], &dwDataLength, dwBufferLength))
		output.clear();
	return output;
}

vector<BYTE> CAes::Decrypt(const vector<BYTE>& Data)
{
	vector<BYTE> output;
	size_t decryptedDataSize;
	if (!MaxDecryptedDataSize(Data.size(), decryptedDataSize))
		return output;

	output.resize(decryptedDataSize);
	output.assign(Data.begin(), Data.end());

	DWORD dwDataLength = (DWORD)Data.size();
	DWORD dwBufferLength = (DWORD)output.size();
	if (!CryptDecrypt(m_Key, 0, true, 0, &output[0], &dwDataLength))
		output.clear();
	return output;
}

void CProtectedData::Protect()
{
	if (m_Length)
		CryptProtectMemory(&m_Data[0], (DWORD)m_Length, CRYPTPROTECTMEMORY_SAME_PROCESS);
}

void CProtectedData::Unprotect()
{
	if (m_Length)
		CryptUnprotectMemory(&m_Data[0], (DWORD)m_Length, CRYPTPROTECTMEMORY_SAME_PROCESS);
}

CProtectedData::CProtectedData(const ::std::vector<BYTE>& Data)
	: m_Data(Data)
{
	DWORD remainder = Data.size() % CRYPTPROTECTMEMORY_BLOCK_SIZE;
	m_Data.resize(Data.size() + CRYPTPROTECTMEMORY_BLOCK_SIZE - remainder);
	m_Length = m_Data.size();
	Protect();
}

void CProtectedData::Serialize(CArchive& Archive)
{
	CDataScopeGuard guard(*this);
	vector<BYTE> data;
	if (Archive.IsStoring() && !m_Data.empty())
	{
		DATA_BLOB dataBlob = { 0 };
		dataBlob.cbData = (DWORD)m_Data.size();
		dataBlob.pbData = &m_Data[0];

		DATA_BLOB protectedDataBlob = { 0 };
		if (CryptProtectData(&dataBlob, nullptr, nullptr, nullptr, nullptr, 0, &protectedDataBlob))
		{
			data.assign(protectedDataBlob.pbData, protectedDataBlob.pbData + protectedDataBlob.cbData);
			LocalFree(protectedDataBlob.pbData);
		}
	}
	::Serialize(Archive, data);
	if (Archive.IsLoading() && !data.empty())
	{
		DATA_BLOB protectedDataBlob = { 0 };
		protectedDataBlob.cbData = (DWORD)data.size();
		protectedDataBlob.pbData = &data[0];
		DATA_BLOB dataBlob = { 0 };
		if (CryptUnprotectData(&protectedDataBlob, nullptr, nullptr, nullptr, nullptr, 0, &dataBlob))
		{
			data.assign(dataBlob.pbData, dataBlob.pbData + dataBlob.cbData);
			m_Data = data;
			m_Length = data.size();
		}
	}
}

CProtectedStringA::CProtectedStringA(const CStringA& Text)
	: CProtectedData(vector<BYTE>(Text.begin(), Text.end()))
{

}

CStringA CProtectedStringA::GetString()
{
	CDataScopeGuard guard(*this);
	return CStringA(m_Data.begin(), m_Data.end());;
}

void DDX(CDataExchange& DataExchange, UINT nIDC, CProtectedStringA& Value)
{
	CStringA sValue = Value.GetString();
	DDX(DataExchange, nIDC, sValue);
	if (DataExchange.IsStoring())
		Value = sValue;
}