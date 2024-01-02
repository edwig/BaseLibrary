/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: TestToken.cpp
//
// BaseLibrary: Indispensable general objects and functions
//              UNIT TESTS for the BaseLibrary
// 
// Copyright (c) 2014-2024 ir. W.E. Huisman
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
#include <PrintToken.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{

TEST_CLASS(TestTokens)
{
public:
  TEST_METHOD(TestUserToken)
  {
    Logger::WriteMessage("Test current user token");

    HANDLE token_handle;
    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS_P,&token_handle))
    {
      Assert::Fail(L"Cannot read the TOKEN of the current process");
    }
    XString userName;
    if(!GetTokenOwner(token_handle,userName))
    {
      Assert::Fail(L"Cannot read the current user name");
    }
    Logger::WriteMessage("Current process user: " + userName);

    XString list;
    if(!DumpToken(list,token_handle))
    {
      Assert::Fail(L"Cannot dump the TOKEN contents");
    }
    Logger::WriteMessage(list);
  }
};

}
