/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: TestAuthenticate.cpp
//
// BaseLibrary: Indispensable general objects and functions
//              UNIT TESTS for the BaseLibrary
// 
// Copyright (c) 2014-2022 ir. W.E. Huisman
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
#include <AuthenticationHeader.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
TEST_CLASS(AuthenticateTests)
{
public:

  TEST_METHOD(TestAuthenticateHeader)
  {
    Logger::WriteMessage("Testing AuthenticateHeader");

    XString user(_T("jan.klaassen@organisatie.nl"));
    XString pass(_T("Th1$1$MyP@$$w03dF03T0d@y"));

    XString auth = CreateAuthentication(user,pass);

    XString finduser;
    XString findpass;
    if(DecodeAuthentication(auth,finduser,findpass))
    {
      Assert::AreEqual(finduser.GetString(),user.GetString());
      Assert::AreEqual(findpass.GetString(),pass.GetString());
    }
    else
    {
      Assert::Fail(L"Could not decode the authentication header");
    }
    Logger::WriteMessage("Your Authenticate header: " + auth);
  }

  TEST_METHOD(TestAuthenticateHeaderDiacrits)
  {
    Logger::WriteMessage("Testing AuthenticateHeader with diacrits");

    XString user(_T("edwig.huisman@organisatie.nl"));
    XString pass(_T("Thí$ì$MÿPä$wö3dFò3Tôdây"));

    XString auth = CreateAuthentication(user,pass);

    XString finduser;
    XString findpass;
    if(DecodeAuthentication(auth,finduser,findpass))
    {
      Assert::AreEqual(finduser.GetString(),user.GetString());
      Assert::AreEqual(findpass.GetString(),pass.GetString());
    }
    else
    {
      Assert::Fail(L"Could not decode the authentication header");
    }
    Logger::WriteMessage("Your Authenticate header: " + auth);
  }

  TEST_METHOD(TestAuthenticateHeaderSeperator)
  {
    Logger::WriteMessage("Testing AuthenticateHeader with extra separators");

    XString user(_T("edwig.huisman@organisatie.nl"));
    XString pass(_T("Thí$~ì$~Mÿ~Pä$wö3d~Fò3~Tôdây~"));

    XString auth = CreateAuthentication(user,pass);

    XString finduser;
    XString findpass;
    if(DecodeAuthentication(auth,finduser,findpass))
    {
      Assert::AreEqual(finduser.GetString(),user.GetString());
      Assert::AreEqual(findpass.GetString(),pass.GetString());
    }
    else
    {
      Assert::Fail(L"Could not decode the authentication header");
    }
    Logger::WriteMessage("Your Authenticate header: " + auth);
  }
};
}