#include "stdafx.h"
#include "Time.h"
#include <iomanip>
#include <sstream>

using namespace std;

static const TCHAR* pTimeFormat = _T("%H:%M:%S %d/%m/%Y");

CTime::CTime(const FILETIME& FileTime)
	: m_Time(FileTime)
{

}

CTime::operator SYSTEMTIME() const
{
	SYSTEMTIME systemTime = { 0 };
	if (!FileTimeToSystemTime(&m_Time, &systemTime))
		ZeroMemory(&systemTime, sizeof(systemTime));
	return systemTime;
}

CTime::operator tm() const
{
	SYSTEMTIME systemTime = *this;
	tm time = { 0 };
	time.tm_isdst = -1;
	time.tm_sec = (int)systemTime.wSecond;
	time.tm_min = (int)systemTime.wMinute;
	time.tm_hour = (int)systemTime.wHour;
	time.tm_mday = (int)systemTime.wDay;
	time.tm_mon = (int)systemTime.wMonth - 1;
	time.tm_year = (int)systemTime.wYear - 1900;
	time.tm_wday = (int)systemTime.wDayOfWeek;
	time_t t = mktime(&time);//normalize is_dst;
	localtime_s(&time, &t);
	return time;
}

CString CTime::ToString(const tm& Time)
{
	wostringstream stream;
	stream << ::std::put_time(&Time, pTimeFormat);
	return stream.str();
}

CTime CTime::FromString(const CString& sTime)
{
	tm time = { 0 };
	wstringstream stream(sTime);
	stream >> ::std::get_time(&time, pTimeFormat);

	return CTime::FromTime(time);
}

CTime CTime::FromTime(const SYSTEMTIME& Time)
{
	FILETIME fileTime = { 0 };
	if (!SystemTimeToFileTime(&Time, &fileTime))
		fileTime.dwLowDateTime = fileTime.dwLowDateTime = 0;
	return CTime(fileTime);
}

CTime CTime::FromTime(const tm& Time)
{
	SYSTEMTIME systemTime = { 0 };
	systemTime.wYear = (unsigned short)(Time.tm_year + 1900);
	systemTime.wMonth = (unsigned short)(Time.tm_mon + 1);
	systemTime.wDay = (unsigned short)Time.tm_mday;
	systemTime.wHour = (unsigned short)(Time.tm_hour);
	systemTime.wMinute = (unsigned short)Time.tm_min;
	systemTime.wSecond = (unsigned short)Time.tm_sec;
	return FromTime(systemTime);
}

CTime CTime::FromTime(const time_t& Time)
{
	tm time = { 0 };
	if (localtime_s(&time, &Time))
		ZeroMemory(&time, sizeof(time));
	return FromTime(time);
}

CString CTime::GetUserDateTimeFormat()
{
	TCHAR dateFormat[20] = { 0 };
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, dateFormat, _countof(dateFormat));
	TCHAR timeFormat[20] = { 0 };
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTTIME, timeFormat, _countof(timeFormat));

	CString sFormat = timeFormat;
	sFormat += _T(" ");
	sFormat += dateFormat;
	return sFormat;
}