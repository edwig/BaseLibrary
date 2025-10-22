/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: TestXStringBuilder.cpp
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
#include <XStringBuilder.h>
#include <HPFCounter.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define NUM_TESTS  10000  // 1E+04
#define NUM_RESULT 1169883

namespace BaseLibraryUnitTests
{
TEST_CLASS(XStringBuilderTests)
{
public:
  TEST_METHOD(TestXStringBuilderPerformance)
  {
    Logger::WriteMessage("Testing XString Builder Performance.");

    XString total;
    XString newstring(_T("This is a new string added to the rest\n"));

    HPFCounter count;
    XString ext = newstring + newstring;
    for(int x = 1;x < NUM_TESTS; ++x)
    {
      total += newstring;
      total += ext;
    }
    size_t size = total.GetLength();
    count.Stop();
    Assert::IsTrue(size == NUM_RESULT);

    XString msg;
    msg.Format(_T("Adding all XStrings took: %8.6f ms"),count.GetCounter());
    Logger::WriteMessage(msg);

    XStringBuilder build;

    HPFCounter count2;
    XString ext2 = newstring + newstring;
    for(int x = 1;x < NUM_TESTS; ++x)
    {
      build.Append(newstring);
      build.Append(ext2);
    }
    total = build.ToString();
    size = total.GetLength();
    count2.Stop();

    msg.Format(_T("XStringBuilder took: %8.6f ms"),count2.GetCounter());
    Logger::WriteMessage(msg);

    Assert::IsTrue(size == NUM_RESULT);
  }
};

}
