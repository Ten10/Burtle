#include "stdafx.h"
#include "Util.h"
#include <algorithm>

vector<CString> Split(const CString& Text, const CString& Delimiters)
{
	vector<CString> result;
	for (size_t pos = 0; pos != CString::npos;)
	{
		auto nextDelimiterPos = Text.find_first_of(Delimiters.c_str(), pos);
		CString sWord = Text.substr(pos, nextDelimiterPos - pos);
		if (!sWord.empty())
			result.emplace_back(move(sWord));
		pos = Text.find_first_not_of(Delimiters.c_str(), nextDelimiterPos);
	}
	return result;
}

UINT BurtleMessageBox(const CString& sText, UINT uType)
{
	return ::MessageBox(HWND_DESKTOP, sText.c_str(), _T("Burtle"), uType);
}