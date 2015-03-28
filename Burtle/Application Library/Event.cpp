#include "stdafx.h"
#include "Event.h"

namespace Synchronization
{
	CEvent::CEvent(bool bManualReset, bool bInitialState, LPSECURITY_ATTRIBUTES pAttributes)
	{
		m_hEvent = CreateEvent(pAttributes, bManualReset, bInitialState, NULL);
	}

	CEvent::~CEvent()
	{
		CloseHandle(m_hEvent);
	}

	void CEvent::SetEvent()
	{
		::SetEvent(m_hEvent);
	}

	void CEvent::ResetEvent()
	{
		::ResetEvent(m_hEvent);
	}
}