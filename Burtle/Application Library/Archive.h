#pragma once
#include "String.h"
#include <iostream>
#include <vector>

class CMemFile
{
public:
	typedef unsigned char byte;
	typedef std::vector<byte> memorystream;

	typedef unsigned char byte;
	class CBuffer
	{
		friend class CMemFile;
		memorystream m_Buffer;
	public:
		CBuffer(UINT nSize);
		~CBuffer();

		operator byte*();
	};
private:
	size_t m_Position = 0;
	memorystream m_Stream;
public:
	CMemFile();

	class CInsufficentBufferException : public std::exception
	{

	};

	void Read(byte* pBuffer, size_t BufferSize);
	void Write(const byte* pBuffer, size_t BufferSize);

	CBuffer GetBuffer(UINT nLength);
	void SetBuffer(CBuffer&& Buffer);

	void SeekToBegin();
	size_t GetLength() const;
	void SetLength(size_t nLength);
};

class CArchive
{
	bool m_bStore;
	CMemFile& m_File;
public:
	CArchive(CMemFile& File, bool bStore);

	class CArchiveException : public std::exception
	{

	};

	bool IsStoring() const { return m_bStore; }
	bool IsLoading() const { return !m_bStore; }

	template <typename T>
	typename std::enable_if<std::is_pod<T>::value && !std::is_pointer<T>::value, CArchive&>::type operator<<(const T& Value)
	{
		if (!m_bStore)
			throw CArchiveException();
		m_File.Write((const BYTE*)&Value, sizeof(T));
		return *this;
	}

	template <typename T>
	typename std::enable_if<std::is_same<T, CString>::value || std::is_same<T, CStringA>::value, CArchive&>::type operator<<(const T& Value)
	{
		if (!m_bStore)
			throw CArchiveException();
		DWORD dwLength = (DWORD)Value.length();
		m_File.Write((const BYTE*)&dwLength, sizeof(dwLength));
		if (dwLength)
			m_File.Write((const BYTE*)Value.c_str(), sizeof(T::traits_type::char_type) * Value.length());
		return *this;
	}

	template <typename T>
	typename std::enable_if<std::is_pod<T>::value && !std::is_pointer<T>::value, CArchive&>::type operator>>(T& Value)
	{
		if (m_bStore)
			throw CArchiveException();
		m_File.Read((BYTE*)&Value, sizeof(T));
		return *this;
	}

	template <typename T>
	typename std::enable_if<std::is_same<T, CString>::value || std::is_same<T, CStringA>::value, CArchive&>::type operator>>(T& Value)
	{
		if (m_bStore)
			throw CArchiveException();
		DWORD dwLength = 0;
		m_File.Read((BYTE*)&dwLength, sizeof(dwLength));
		if (!dwLength)
			Value.clear();
		else
		{
			vector<T::traits_type::char_type> buffer;
			buffer.resize(dwLength + 1, 0);
			m_File.Read((BYTE*)&buffer[0], dwLength);
			Value = &buffer[0];
		}
		return *this;
	}
};

template <typename T>
static void Serialize(CArchive& Archive, T& Value)
{
	if (Archive.IsStoring())
		Archive << Value;
	else
		Archive >> Value;
}

template <typename T>
static void Serialize(CArchive& Archive, std::vector<T>& Value)
{
	size_t size = Value.size();
	::Serialize(Archive, size);
	if (Archive.IsLoading())
		Value.resize(size);
	for (auto it = Value.begin(); it != Value.end(); ++it)
		::Serialize(Archive, *it);
}