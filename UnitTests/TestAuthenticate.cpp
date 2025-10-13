/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: TestAuthenticate.cpp
//
// BaseLibrary: Indispensable general objects and functions
//              UNIT TESTS for the BaseLibrary
// 
// Created: 2014-2025 ir. W.E. Huisman
// MIT License
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
#include <HTTPMessage.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
TEST_CLASS(AuthenticateTests)
{
private:
  int TestCookie(HTTPMessage* p_msg,XString p_meta,XString p_expect)
  {
    XString value = p_msg->GetCookieValue(_T("SESSIONCOOKIE"),p_meta);
    return value.Compare(p_expect) != 0;
  }

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
      Assert::AreEqual(user.GetString(),finduser.GetString());
      Assert::AreEqual(pass.GetString(),findpass.GetString());
    }
    else
    {
      Assert::Fail(L"Could not decode the authentication header");
    }
    Logger::WriteMessage(XString("Your Authenticate header: ") + auth);
  }

  TEST_METHOD(TestAuthenticateHeaderDiacrits)
  {
    Logger::WriteMessage("Testing AuthenticateHeader with diacritics");

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
    Logger::WriteMessage(XString("Your Authenticate header: ") + auth);
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
    Logger::WriteMessage(XString("Your Authenticate header: ") + auth);
  }

  TEST_METHOD(TestCookiesOverwrite)
  {
    int errors = 0;

    // Standard values
    XString url;
    url.Format(_T("http://localhost:1201/MarlinTest/CookieTest/"));

    // Test 1
    HTTPMessage* msg = alloc_new HTTPMessage(HTTPCommand::http_put,url);

    msg->SetCookie(_T("SESSIONCOOKIE"),_T("123456"),_T("meta"),false,true);
    errors += TestCookie(msg,_T("meta"),_T("123456"));

    msg->SetCookie(_T("SESSIONCOOKIE"),_T(""),_T(""),false,true);
    errors += TestCookie(msg,_T(""),_T(""));

    msg->SetCookie(_T("SESSIONCOOKIE"),_T("654321"),_T(""),false,true);
    errors += TestCookie(msg,_T(""),_T("654321"));

    delete msg;

    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(TestDecryptCookie)
  {
    int errors = 0;

    HTTPMessage msg;
    msg.SetCookie(_T("MYCOOKIE"),_T("ea415e3b69bc8383ff4342ad736d8f3d"),_T("webenab-03-01-2019 10:02:43"),true,true);
    XString val = msg.GetCookieValue(_T("MYCOOKIE"),_T("webenab-03-01-2019 10:02:43"));

    errors += val.Compare(_T("ea415e3b69bc8383ff4342ad736d8f3d")) != 0;

    Assert::AreEqual(0,errors);
  }
};
}