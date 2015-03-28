#pragma once
#include <string>

class CException : public ::std::exception
{
	::std::string m_Message;
public:
	CException(const ::std::string& Message = "")
		: ::std::exception(Message.c_str())
	{

	}
};

class CDataExchangeException : public CException
{
public:
	CDataExchangeException(const ::std::string& Message = "")
		: CException(Message)
	{

	}
};

#define stringify(x) #x

#ifdef _DEBUG
#include <assert.h>
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif

#define ENSURE(exp) if (!(exp)) throw CException(stringify(exp))
