/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: JSONTests.cpp
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

#include <JSONMessage.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
TEST_CLASS(JSONMessageTests)
{
public:

  TEST_METHOD(TestJSONSortingArray)
  {
    Logger::WriteMessage("Testing JSONMessage array sorting");

    XString input(_T("{\n"
                     "\t\"myarray\":[\n"
                     "\t\"zz\",\n"
                     "\t\"cc\",\n"
                     "\t\"bb\",\n"
                     "\t\"aa\"\n"
                     "\t]\n"
                     "}"));
    JSONMessage json;
    json.ParseMessage(input);

    json.SetWhitespace(true); // to be sure
    XString output = json.GetJsonMessage();

    Assert::IsTrue(output.Compare(input) == 0);
  }

  TEST_METHOD(TestJSONSortingObject)
  {
    Logger::WriteMessage("Testing JSONMessage object sorting");

    XString input(_T("{\n"
                     "\t\"myobject\":\"my\",\n"
                     "\t\"aa\":11,\n"
                     "\t\"zz\":99\n"
                     "}"));
    JSONMessage json;
    json.ParseMessage(input);

    json.SetWhitespace(true); // to be sure
    XString output = json.GetJsonMessage();

    Assert::IsTrue(output.Compare(input) == 0);
  }
};
}