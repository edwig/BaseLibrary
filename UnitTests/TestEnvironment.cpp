/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: XSDSchemaTests.cpp
//
// BaseLibrary: Indispensable general objects and functions
//              UNIT TESTS for the BaseLibrary
// 
// Copyright (c) 2014-2025 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include "pch.h"
#include "CppUnitTest.h"
#include <Environment.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
TEST_CLASS(TestEnvironment)
{
public:

  void TestBefore()
  {
    Logger::WriteMessage("BEFORE: Environment");

    TCHAR buffer[2000];
    int len = GetEnvironmentVariable(_T("TEST"),buffer,2000);
    buffer[len] = 0;
    XString msg;
    msg.Format(_T("TEST=%s\n"),buffer);

    Logger::WriteMessage(msg);

    len = GetEnvironmentVariable(_T("PATH"),buffer,2000);
    buffer[len] = 0;
    msg.Format(_T("PATH=%s\n"),buffer);

    Logger::WriteMessage(msg);
  }

  void TestAfter()
  {
    Logger::WriteMessage("AFTER: Environment");

    TCHAR buffer[2000];
    int len = GetEnvironmentVariable(_T("TEST"),buffer,2000);
    buffer[len] = 0;
    XString foundenv;
    XString expected(_T("TEST=bb"));
    foundenv.Format(_T("TEST=%s"),buffer);

    // Test assignment
    Logger::WriteMessage(foundenv);
    Assert::AreEqual(expected.GetString(),foundenv.GetString());


    // Test assignment
    len = GetEnvironmentVariable(_T("APPLEPIE"),buffer,2000);
    buffer[len] = 0;
    foundenv.Format(_T("APPLEPIE=%s"),buffer);
    expected = _T("APPLEPIE=whippedcream");

    Logger::WriteMessage(foundenv);
    Assert::AreEqual(expected.GetString(),foundenv.GetString());

    // print changed %PATH%
    len = GetEnvironmentVariable(_T("PATH"),buffer,2000);
    buffer[len] = 0;
    foundenv.Format(_T("PATH=%s"),buffer);
    Logger::WriteMessage(foundenv);

    // Test that we got something pre-pended to the path variable
    foundenv = foundenv.Left(12);
    expected = _T("PATH=C:\\TMP;");
    Assert::AreEqual(expected.GetString(),foundenv.GetString());
  }

  // Beware: Visual Studio must be run in 'admin' mode
  //         for the build and this test to succeed!!!
  TEST_METHOD(TestEnvironmentFile)
  {
    Logger::WriteMessage("Testing automatic 'Environment.env' file");

    // Show variables before running
    TestBefore();
    // process our 'module.env' file
    DoProcessEnvironment();
    // Show variables after running
    TestAfter();
  }
};
}