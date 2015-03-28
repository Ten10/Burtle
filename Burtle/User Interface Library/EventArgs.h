#pragma once

class CWindow;
struct CNoEventArgs;
struct CEventArgs
{
	CEventArgs() = default;

	CEventArgs(const CWindow&, WPARAM wParam, LPARAM lParam)
	{
	}

	CEventArgs(const CEventArgs& Args) = delete;

	virtual ~CEventArgs()
	{
	}

	operator LRESULT() const
	{
		return true;
	}
};