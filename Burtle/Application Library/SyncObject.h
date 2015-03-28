#pragma once

class CSyncObject
{
public:
	CSyncObject()
	{
	}

	CSyncObject(const CSyncObject& Object) = delete;

	virtual void Lock() throw() = 0;
	virtual void Unlock() throw() = 0;
};