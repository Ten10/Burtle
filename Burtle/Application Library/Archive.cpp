#include "stdafx.h"
#include "Archive.h"

using namespace std;

static byte* get_vector_ptr(vector<byte>& Data, size_t Offset, size_t nLength)
{
	if (Data.size() < Offset + nLength)
		throw CMemFile::CInsufficentBufferException();

	return &Data[Offset];
}

CMemFile::CBuffer::CBuffer(UINT nSize)
{
	m_Buffer.resize(nSize);
}

CMemFile::CBuffer::~CBuffer()
{
}

CMemFile::CBuffer::operator byte*()
{
	return get_vector_ptr(m_Buffer, 0, m_Buffer.size());
}

CMemFile::CMemFile()
{

}

void CMemFile::Read(byte* pBuffer, size_t BufferSize)
{
	if (BufferSize > (GetLength() - m_Position))
		throw CInsufficentBufferException();
	memcpy_s(pBuffer, BufferSize, get_vector_ptr(m_Stream, m_Position, BufferSize), BufferSize);
	m_Position += BufferSize;
}

void CMemFile::Write(const byte* pBuffer, size_t BufferSize)
{
	m_Stream.insert(m_Stream.begin() + m_Position, pBuffer, pBuffer + BufferSize);
	m_Position += BufferSize;
}

CMemFile::CBuffer CMemFile::GetBuffer(UINT nLength)
{
	CBuffer buffer(nLength);
	size_t pos = m_Position;
	m_Position = 0;
	Read(buffer, GetLength());
	m_Position = pos;
	return buffer;
}

void CMemFile::SetBuffer(CBuffer&& Buffer)
{
	m_Position = 0;
	m_Stream = move(Buffer.m_Buffer);
}

void CMemFile::SeekToBegin()
{
	m_Position = 0;
}

size_t CMemFile::GetLength() const
{
	return m_Stream.size();
}

void CMemFile::SetLength(size_t Length)
{
	m_Stream.resize(Length);
}

CArchive::CArchive(CMemFile& File, bool bStore)
	: m_File(File),
	m_bStore(bStore)
{

}