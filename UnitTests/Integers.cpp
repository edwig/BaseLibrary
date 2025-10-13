/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: Integers.cpp
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
#include <safeint.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace msl::utilities;

namespace BaseLibraryUnitTests
{
TEST_CLASS(IntegerTests)
{
public:

  TEST_METHOD(IntegerSafty1)
  {
    Logger::WriteMessage("Integer safty overflow");

    // No error bells!
    int num1 = 0x7FFFFFFE;
    int num2 = num1 + 2;

    Assert::IsTrue(num2 < 0);

    // Cannot work!
    SafeInt<int> number1 { 0x7FFFFFFE };
    SafeInt<int> number2 { 2 };

    try
    {
      number2 = number1 + 2;
    }
    catch(SafeIntException& ex)
    {
      XString message;
      message.Format(_T("Safe int error code: %d"),ex.m_code);
      Logger::WriteMessage(message);
      message.Format(_T("Number 2 still is: %d"),*number2.Ptr());
      Logger::WriteMessage(message);

      Assert::IsTrue(ex.m_code == 1);
      Assert::IsTrue(number2   == 2);
      return;
    }
    // Should not come to here
    Assert::IsTrue(false);
  }

};

}