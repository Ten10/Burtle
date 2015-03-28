#include "stdafx.h"
#include "SingleLock.h"

CSingleLock::CSingleLock(CSyncObject& Object)
	: m_Object(Object)
{
	m_Object.Lock();
}

CSingleLock::~CSingleLock()
{
	m_Object.Unlock();
}