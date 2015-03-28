#pragma once
#include <Unknwn.h>
#include "COM.h"

class CBurtleFactory : public IClassFactory
{
	static ULONG ServerLocks;
public:
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreateInstance(
		/* [unique][in] */ IUnknown *pUnkOuter,
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void **ppvObject);

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE LockServer(
		/* [in] */ BOOL fLock);

	static bool CanUnload() { return ServerLocks == 0; }

	BEGIN_COM_MAP()
		COM_INTERFACE_ENTRY(IClassFactory)
	END_COM_MAP()
};