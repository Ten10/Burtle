// dllmain.h : Declaration of module class.
#pragma once
#include "COM.h"

class CBurtleModule : public CComModule
{
public:
	CBurtleModule();
};

extern CBurtleModule theModule;
/*class CBurtleModule : public ATL::CAtlDllModuleT< CBurtleModule >
{
public :
	DECLARE_LIBID(LIBID_BurtleLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BURTLE, "{358A5172-87E1-4541-9015-AFC5A3D28E5F}")
};

extern class CBurtleModule _AtlModule;
*/