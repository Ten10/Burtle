#pragma once
#include <time.h>
#include <WinBase.h>

#include "String.h"

class CTime
{
	FILETIME m_Time;
public:
	CTime(const FILETIME& FileTime);

	operator SYSTEMTIME() const;
	operator tm() const;

	static CString ToString(const tm& Time);
	static CTime FromString(const CString& sTime);

	static CTime FromTime(const SYSTEMTIME& Time);
	static CTime FromTime(const tm& Time);
	static CTime FromTime(const time_t& Time);
	static CString GetUserDateTimeFormat();
};