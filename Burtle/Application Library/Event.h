#pragma once
#include "SyncObject.h"

namespace Synchronization
{
	class CEvent
	{
		HANDLE m_hEvent = nullptr;
	public:
		CEvent(bool bManualReset, bool bInitialState, LPSECURITY_ATTRIBUTES pAttributes = nullptr);
		~CEvent();
		CEvent(const CEvent&) = delete;

		void SetEvent();
		void ResetEvent();

		operator HANDLE() const { return m_hEvent; }
	};
};