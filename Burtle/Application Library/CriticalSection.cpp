#include "stdafx.h"
#include "CriticalSection.h"

using namespace std;

namespace Synchronization
{
	struct CCriticalSectionHandle : public CRITICAL_SECTION
	{
		CCriticalSectionHandle()
		{
			InitializeCriticalSectionEx(this, 0, 0);
		}

		~CCriticalSectionHandle()
		{
			DeleteCriticalSection(this);
		}
	};

	CCriticalSection::CCriticalSection()
	{
		m_spCriticalSectionHandle = make_unique<CCriticalSectionHandle>();
	}

	CCriticalSection::~CCriticalSection()
	{
	}

	void CCriticalSection::Lock()
	{
		EnterCriticalSection(m_spCriticalSectionHandle.get());
	}

	void CCriticalSection::Unlock()
	{
		LeaveCriticalSection(m_spCriticalSectionHandle.get());
	}
}