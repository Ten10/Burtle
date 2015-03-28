#pragma once
#include "SyncObject.h"

class CSingleLock
{
	CSyncObject& m_Object;
public:
	CSingleLock(CSyncObject& Object);
	~CSingleLock();
};