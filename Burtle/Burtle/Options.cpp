#include "stdafx.h"
#include "Options.h"

#include <WinCrypt.h>
#include <iostream>
#include <iomanip>
#include <numeric>

class CPasswordKey
{
	CPasswordKey()
	{

	}
};

static bool HexStringToByteArray(CString& sHexString, vector<BYTE>& Data)
{
	size_t length = sHexString.length();
	if (length <= 0 || length % 2)
		return false;

	wstringstream ss;
	unsigned int buffer;
	size_t offset = 0;
	while (offset < sHexString.length()) 
	{
		ss.clear();
		ss << ::std::hex << sHexString.substr(offset, 2);
		ss >> buffer;
		Data.push_back(static_cast<BYTE>(buffer));
		offset += 2;
	}
	return true;
}

static void ByteArrayToHexString(const vector<BYTE>& Data, CString& sHexString)
{
	sHexString = ::std::accumulate(Data.begin(), Data.end(), CString(), [=](const CString& Result, const TCHAR input)
	{
		wostringstream oss;
		oss << Result <<::std::hex << ::std::setw(2) << ::std::setfill(L'0') << static_cast<int>(input);
		return oss.str();
	});
}

static vector<BYTE> EncryptPassword()
{

}

void COptions::Serialize(CArchive& Archive)
{
	::Serialize(Archive, bAuthenticationRequired);
	::Serialize(Archive, BugzillaURI);
	::Serialize(Archive, Username);
	Password.Serialize(Archive);
	::Serialize(Archive, MaxNumberOfBugsPerSearch);
}

COptions COptions::FromTortoiseString(const CString& String)
{
	if (String.empty())
		return COptions();
	vector<BYTE> data;
	CString sOptions = String;
	HexStringToByteArray(sOptions, data);

	CMemFile file;
	file.Write(&data[0], static_cast<UINT>(data.size()));
	file.SeekToBegin();
	CArchive archive(file, false);

	COptions options;
	try
	{
		options.Serialize(archive);
	}
	catch (exception&)
	{
		ASSERT(false);
		options = COptions();
	}
	return options;
}

CString COptions::ToTortoiseString(const COptions& Options)
{
	auto options = Options;
	CMemFile file;
	{
		try
		{
			CArchive archive(file, true);
			options.Serialize(archive);
		}
		catch (exception&)
		{
			ASSERT(false);
			return _T("");
		}
	}
	file.SeekToBegin();

	vector<BYTE> data;
	data.resize(static_cast<size_t>(file.GetLength()));
	file.Read(&data[0], static_cast<UINT>(data.size()));

	CString sText;
	ByteArrayToHexString(data, sText);
	return sText;
}