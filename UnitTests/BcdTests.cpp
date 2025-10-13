/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: BCDTests.cpp
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
#include <bcd.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
TEST_CLASS(BcdTests)
{
public:

  TEST_METHOD(TestRound)
  {
    Logger::WriteMessage("Testing BCD Rounding");

    bcd value(_T("1.9"));
    value.Round(0);

    XString expected = _T("2");
    XString testval = value.AsString(bcd::Format::Bookkeeping,false,0);

    Assert::AreEqual(expected.GetString(),testval.GetString());
  }

  TEST_METHOD(TestNaturalLog)
  {
	  Logger::WriteMessage("Testing BCD Natural Log 0.5");

    bcd value(_T("0.5"));
    bcd nlog = value.Log();

    XString expected = _T("-0.693147180559945309417232121458176568023");
    XString testval  = nlog.AsString(bcd::Format::Bookkeeping,false,30);

	  Assert::AreEqual(expected.GetString(), testval.GetString());

  }

  TEST_METHOD(TestComparisons)
  {
    Logger::WriteMessage("Testing BCD negative value comparisons");

    bcd value0;
    bcd value1(-5);
    bcd value2(-7);
    bcd value3(-155);
    bcd value4(5);
    bcd value5(7);
    bcd value6(155);

    Assert::IsTrue(value2 < value1);  //   -7 <   -5
    Assert::IsTrue(value3 < value1);  // -155 <   -5
    Assert::IsTrue(value1 < value0);  //   -5 <    0
    Assert::IsTrue(value1 < value4);  //   -5 <    5
    Assert::IsTrue(value1 < value5);  //   -5 <    7
    Assert::IsTrue(value1 < value6);  //   -5 <  155
    Assert::IsTrue(value4 < value5);  //    5 <    7
    
    Assert::IsTrue(value1 > value2);  //   -5 >   -7
    Assert::IsTrue(value1 > value3);  //   -5 > -155
    Assert::IsTrue(value0 > value1);  //    0 >   -5
    Assert::IsTrue(value4 > value1);  //    5 >   -5
    Assert::IsTrue(value5 > value1);  //    7 >   -5
    Assert::IsTrue(value5 > value4);  //    7 >    5

    Assert::IsFalse(value2 > value1);  //   -7 <   -5
    Assert::IsFalse(value3 > value1);  // -155 <   -5
    Assert::IsFalse(value1 > value0);  //   -5 <    0
    Assert::IsFalse(value1 > value4);  //   -5 <    5
    Assert::IsFalse(value1 > value5);  //   -5 <    7
    Assert::IsFalse(value1 > value6);  //   -5 <  155
    Assert::IsFalse(value4 > value5);  //    5 <    7

    Assert::IsFalse(value1 < value2);  //   -5 >   -7
    Assert::IsFalse(value1 < value3);  //   -5 > -155
    Assert::IsFalse(value0 < value1);  //    0 >   -5
    Assert::IsFalse(value4 < value1);  //    5 >   -5
    Assert::IsFalse(value5 < value1);  //    7 >   -5
    Assert::IsFalse(value5 < value4);  //    7 >    5
  }
};
}
