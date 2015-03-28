#pragma once
#include <memory>
#include "SyncObject.h"

namespace Synchronization
{
	struct CCriticalSectionHandle;
	class CCriticalSection : public CSyncObject
	{
		::std::unique_ptr<CCriticalSectionHandle> m_spCriticalSectionHandle;

	public:
		CCriticalSection();
		~CCriticalSection();

		void Lock() throw() override;
		void Unlock() throw() override;
	};
}