
// BurtleTest.h : main header file for the PROJECT_NAME application
//

#pragma once

/*#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif*/

#include "resource.h"		// main symbols
#include "..\User Interface Library\UI.h"

// CBurtleTestApp:
// See BurtleTest.cpp for the implementation of this class
//

class CBurtleTestApp : public CUIApplication
{
	CEventMap m_EventMap;
	IBugTraqProvider2* m_pBugTraqProvider = nullptr;
public:
	CBurtleTestApp();
	~CBurtleTestApp();

// Overrides
public:
	bool InitInstance(HINSTANCE hInstance) override;
	int ExitInstance() override;
	IBugTraqProvider2& GetBugTraqProvider();

// Implementation
};

extern CBurtleTestApp theApp;