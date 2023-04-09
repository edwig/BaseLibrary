/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: XSDSchemaTests.cpp
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
#include <XSDSchema.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
TEST_CLASS(XSDSchemaTests)
{
public:

  TEST_METHOD(XSDSchema_Reading)
  {
    Logger::WriteMessage("Reading an XSDSchema");

    XSDSchema schema;
    XsdError result = schema.ReadXSDSchema("..\\..\\UnitTests\\Schemas\\TestSchema_SALES005.xsd");

    Assert::AreEqual((int)XsdError::XSDE_NoError,(int)result);
  }

  TEST_METHOD(XSDSchema_Writing)
  {
    Logger::WriteMessage("Reading an XSDSchema");

    XSDSchema schema;
    XsdError result = schema.ReadXSDSchema("..\\..\\UnitTests\\Schemas\\TestSchema_SALES005.xsd");

    Assert::AreEqual((int) XsdError::XSDE_NoError,(int) result);

    bool saved = schema.WriteXSDSchema("..\\..\\UnitTests\\Schemas\\TestSchema_Copy.xsd");
    Assert::AreEqual(saved,true);
  }

  TEST_METHOD(XSDSchema_Validate_OK1)
  {
    Logger::WriteMessage("Validating minimal XMLMessage with XSDSchema");

    XSDSchema schema;
    XsdError result_xsd = schema.ReadXSDSchema("..\\..\\UnitTests\\Schemas\\TestSchema_SALES005.xsd");

    XMLMessage msg;
    msg.LoadFile("..\\..\\UnitTests\\Schemas\\TestMessage1_SALES005.xml");
    XmlError result_xml = msg.GetInternalError();

    Assert::AreEqual((int) XsdError::XSDE_NoError,(int) result_xsd);
    Assert::AreEqual((int) XmlError::XE_NoError,  (int) result_xml);

    XString errors;
    XsdError validation = schema.ValidateXML(msg,errors);
    if(validation != XsdError::XSDE_NoError)
    {
      Logger::WriteMessage("ERRORS: " + errors);
      Assert::Fail();
    }
  }

  TEST_METHOD(XSDSchema_Validate_OK2)
  {
    Logger::WriteMessage("Validating minimal XMLMessage with XSDSchema");

    XSDSchema schema;
    XsdError result_xsd = schema.ReadXSDSchema("..\\..\\UnitTests\\Schemas\\TestSchema_SALES005.xsd");

    XMLMessage msg;
    msg.LoadFile("..\\..\\UnitTests\\Schemas\\TestMessage2_SALES005.xml");
    XmlError result_xml = msg.GetInternalError();

    Assert::AreEqual((int) XsdError::XSDE_NoError,(int) result_xsd);
    Assert::AreEqual((int) XmlError::XE_NoError,(int) result_xml);

    XString errors;
    XsdError validation = schema.ValidateXML(msg,errors);
    if(validation != XsdError::XSDE_NoError)
    {
      Logger::WriteMessage("ERRORS: " + errors);
      Assert::Fail();
    }
  }

};

}