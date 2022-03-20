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


**METHOD 3: Using a framework.h file**

In order to be able to switch the working between MFC and std::string, we can put the configurtion
of the framework to use in a separate file named 'framework.h' as is done in many Microsoft Visual Studio wizards.
In this framework.h file we can configure all the MFC parts we want to use (or not). In environments 
where multiple libraries are used an excursion to a 'framework.h' file in the root directory can provide
the configuration for all libraries and projects in your solution. For an example, seen the SQLComponents project
This is done in the following sequence:

````C
// stdafx.h : include file for standard system include files,
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"
#include "framework.h"

#include <BaseLibrary.h>
````

Now in the projects 'framework.h' we configure the use of the MFC framework:

````C
//////////////////////////////////////////////////////////////////////////
//
// Getting the framework settings for this library/program
// Uses the $(SolutionDir)Framework.h file for configuration of 
// the MFC XString or the std::string SMX_String
//
#pragma once

#define WIN32_LEAN_AND_MEAN                     // Exclude rarely-used stuff from Windows headers

// remove support for MFC controls in dialogs
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         
// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include "..\framework.h"

//////////////////////////////////////////////////////////////////////////
//
// Can be extended beyond this point with extra MFC requirements
//
//////////////////////////////////////////////////////////////////////////

// Extras needed for MFC in this program
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>         // MFC support for Windows Common Controls
````

And lastly, in the ROOT directory of our solution we have our solution wide 'framework.h' file:

````C
/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: Framework.h
// This file determines wheter your project is using MFC CString for a string
// or uses the SMX_String (derived from std::string) class.

#pragma once

// Included in an Microsoft Application Framework eXtension environment (MFC)
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include <afx.h>
````


REMARK:
For larger projects, method 3 with a solution-wide framework.h is the most reliable way to go!

