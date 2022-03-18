Introduction and purpose
========================

This is the EDO Baselibrary. Version 1.0.0

Other projects in this (Edwig) GitHub repository had have overlapping
classes for a long time. This library encapsulates them all.
So the duplicates can be removed from the following general libraries and frameworks:

- Marlin web server
- SQLComponents
- CXHibernate

Various other (smaller) libraries that were started to develop base components
have also been included in this base library. They are:

- BCD 		: Binary Coded Decimal
- WinFile	: Everything ( :-) ) you can do with a MS-Windows File
- WinPing	: Ping solution
- SMX_String: Alle MFC tricks blended into std::string

You can use the base library in conjunction with MFC (Microsoft Foundation Classes)
but this is not neccesary a requirement. To configure the use, simply use one of the 
following two methods:

**METHOD 1: NO MFC, use the C++ standard library only**

Include the baselibrary header file in your standard "pch.h" header file that's get
included in every *.cpp file of your project. Just use as in the following example:
```C
// pch.h This is a precompiled header file
#include "targetver.h"

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <BaseLibrary.h>
````
As long as the standard include path has the 'BaseLibrary' path 'on-board' and your
project uses the 'Lib' directory in the solution as the standard collection place for
all of the libaries and components that you are using, your good to go.

XString is now defined as a derived class of std::string

**METHOD 2: Use in conjunction with MFC**

Include the baselibrary header file in your standard "stdafx.h" header file that's get
included in every *.cpp file of your project. Just use as in the following example:
````C
// stdafx.h : include file for standard system include files,
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _AFX_ALL_WARNINGS
#include <afx.h>
#include <afxwin.h>             // MFC core and standard components

#include <BaseLibrary.h>
````
As long as the standard include path has the 'BaseLibrary' path 'on-board' and your
project uses the 'Lib' directory in the solution as the standard collection place for
all of the libaries and components that you are using, your good to go.

XString is now defined as being the standard CString from MFC.
