// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"

#include <tchar.h>
#include <map>
#include <set>
#include <vector>
#include <exception>
#include <string>
#include <future>
#include "types.h"

using namespace ::std;
//using namespace ATL;

#pragma comment(lib, "Wininet")

#include <tchar.h>

#include "../BugzillaLibrary/Bugzilla.h"
#include "../Application Library/Application.h"
#include "../User Interface Library/UI.h"
using namespace Bugzilla;