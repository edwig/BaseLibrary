/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: JSONTests.cpp
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
#include <JSONMessage.h>
#include <SOAPMessage.h>
#include <JSONPointer.h>
#include <JSONPath.h>
#include <MultiPartBuffer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
TEST_CLASS(JSONMessageTests)
{
private:
  int TestValue1(XString p_object)
  {
    JSONMessage json(p_object);

    if(json.GetErrorState())
    {
      return 1;
    }

    // Reprint the message
    XString object = json.GetJsonMessage();

    if(object != p_object)
    {
      return 1;
    }
    return 0;
  }

  int TestValue1Double(XString p_object)
  {
    JSONMessage json(p_object);

    if(json.GetErrorState())
    {
      return 1;
    }

    // Reprint the message
    XString object = json.GetJsonMessage();

    // Engineering breaking criterion on small IEEE number
    if(abs(_ttof(object) - _ttof(p_object)) > 1.0E-13)
    {
      return 1;
    }
    return 0;
  }

  int TestArray(XString p_array)
  {
    JSONMessage json(p_array);

    if(json.GetErrorState())
    {
      return 1;
    }

    XString array = json.GetJsonMessage();

    if(p_array != array)
    {
      return 1;
    }
    return 0;
  }

  int TestObject(XString p_object)
  {
    JSONMessage json(p_object);

    if(json.GetErrorState())
    {
      return 1;
    }

    XString object = json.GetJsonMessage();

    if(object != p_object)
    {
      return 1;
    }
    return 0;
  }


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

  TEST_METHOD(TestJSONSupportMBCS)
  {
    Logger::WriteMessage("Testing MBCS support in JSON messages");

    bool  result = false;
    XString text = _T("{\"PriceTag\":\"€ 42,00\"}");
    JSONMessage msg(text);
    XString test = msg.GetJsonMessage();

    // msg.SetEncoding(JsonEncoding::JENC_UTF8);
    // test = msg.GetJsonMessage(JsonEncoding::JENC_UTF8);

    if(test == text)
    {
      result = true;
    }
    Assert::AreEqual(true,result);
  }

  TEST_METHOD(TestJSONSupportUTF8)
  {
    Logger::WriteMessage("Testing UTF-8 support in JSON messages");

    int result = 2;

    XString text = _T("{\"PriceTag\":\"€ 42,00\"}");
    JSONMessage msg(text);
    XString test = msg.GetJsonMessage();
    test = EncodeStringForTheWire(test);
    XString mustbe = _T("{\"PriceTag\":\"\xe2\x82\xac 42,00\"}");

    if(test == mustbe)
    {
      --result;
    }

    JSONMessage msg2(mustbe);
    XString tt2 = msg2.GetJsonMessage();

    if(tt2 == mustbe)
    {
      --result;
    }
    Assert::AreEqual(0,result);
  }

  TEST_METHOD(TestJSONValues)
  {
    Logger::WriteMessage("Testing simple JSON values");

    int errors = 0;

    // Test whether we can make a JSON object
    errors += TestValue1(_T("null"));
    errors += TestValue1(_T("true"));
    errors += TestValue1(_T("false"));
    errors += TestValue1(_T(""));

    // Singular values
    errors += TestValue1(_T("\"This is a regular string\""));
    errors += TestValue1(_T("\"This is a string\\nwith\\r\\nseveral / and more \\\\ escape \\f sequences \\t in \\t it.\\r\\n\""));
    errors += TestValue1(_T("1234"));
    errors += TestValue1(_T("1234.56789"));
    errors += TestValue1(_T("-1234"));
    errors += TestValue1Double(_T("1.23456e12"));
    errors += TestValue1Double(_T("1.23456e-12"));
    // Singular array
    errors += TestArray(_T("[]"));
    errors += TestArray(_T("[true]"));
    errors += TestArray(_T("[true,\"string\"]"));
    // Singular object
    errors += TestObject(_T("{}"));
    errors += TestObject(_T("{\"one\":1}"));
    errors += TestObject(_T("{\"one\":1,\"two\":2}"));
    // Compounded test
    errors += TestObject(_T("{\"one\":1,\"two\":[1,2,3,4,5,\"six\",\"seven\"]}"));

    // TEST ROUNDTRIP SOAP -> JSON
    XString namesp(_T("http://test.marlin.org/interface"));
    XString action(_T("FirstAction"));
    SOAPMessage msg(namesp,action);
    msg.SetParameter(_T("First"),101);
    msg.SetParameter(_T("Second"),102);
    XMLElement* elem = msg.AddElement(NULL,_T("Third"),(XmlDataType) XDT_String,_T(""));
    if(elem)
    {
      msg.AddElement(elem,_T("Fortune"),XDT_Integer,_T("1000000"));
      msg.AddElement(elem,_T("Glory"),XDT_String,_T("Indiana Jones"));
    }
    JSONMessage json(&msg);
    XString str = json.GetJsonMessage();
    errors += TestObject(str);

    // And test the way back
    SOAPMessage soap(&json);

    // Test if the ROUNDDTRIP SOAP -> JSON -> SOAP did work
    int FirstAction1 = soap.GetParameterInteger(_T("First"));
    int FirstAction2 = soap.GetParameterInteger(_T("Second"));
    XMLElement* fort = soap.FindElement(_T("Fortune"));
    XMLElement* glor = soap.FindElement(_T("Glory"));
    int FirstFortune = fort ? _ttoi(fort->GetValue()) : 0;
    XString glory = glor ? glor->GetValue() : XString();
    if(FirstAction1 != 101 || FirstAction2 != 102 || FirstFortune != 1000000 || glory.Compare(_T("Indiana Jones")))
    {
      _tprintf(_T("ERROR: ROUND TRIP ENGINEERING SOAP -> JSON -> SOAP FAILED\n"));
      ++errors;
    }

    // Check that soap faults will survive SOAP/JSON/SOAP round trip conversions
    SOAPMessage wrong(_T("<body></body>"));
    wrong.SetSoapVersion(SoapVersion::SOAP_12);
    wrong.SetFault(_T("Client"),_T("Message"),_T("Wrong body"),_T("Empty Body"));
    JSONMessage wrongtoo(&wrong);
    SOAPMessage veryWrong(&wrongtoo);

    if(veryWrong.GetFaultCode()   != _T("Client") ||
       veryWrong.GetFaultActor()  != _T("Message") ||
       veryWrong.GetFaultString() != _T("Wrong body") ||
       veryWrong.GetFaultDetail() != _T("Empty Body"))
    {
      ++errors;
    }
    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(TestJSONArray)
  {
    Logger::WriteMessage("Testing the JSON Array capability");

    int errors = 0;
    XString msg1 = _T("<Applications>\n")
                   _T("  <Datasources>\n")
                   _T("     <Datasource>develop</Datasource>\n")
                   _T("     <Datasource>test</Datasource>\n")
                   _T("     <Datasource>accept</Datasource>\n")
                   _T("     <Datasource>production</Datasource>\n")
                   _T("  </Datasources>\n")
                   _T("  <Commercial>\n")
                   _T("     <Application>Financials</Application>\n")
                   _T("     <Application>Maintainance</Application>\n")
                   _T("     <Application>Rentals</Application>\n")
                   _T("  </Commercial>\n")
                   _T("  <Default>QL_Account</Default>\n")
                   _T("</Applications>");
    SOAPMessage soap1(msg1);
    JSONMessage json(&soap1);
    XString res = json.GetJsonMessage();
    XString expect1 = _T("{\n")
                      _T("\t\"Applications\":{\n")
                      _T("\t\t\"Datasources\":{\n")
                      _T("\t\t\t\"Datasource\":[\n")
                      _T("\t\t\t\"develop\",\n")
                      _T("\t\t\t\"test\",\n")
                      _T("\t\t\t\"accept\",\n")
                      _T("\t\t\t\"production\"\n")
                      _T("\t\t\t]\n")
                      _T("\t\t},\n")
                      _T("\t\t\"Commercial\":{\n")
                      _T("\t\t\t\"Application\":[\n")
                      _T("\t\t\t\"Financials\",\n")
                      _T("\t\t\t\"Maintainance\",\n")
                      _T("\t\t\t\"Rentals\"\n")
                      _T("\t\t\t]\n")
                      _T("\t\t},\n")
                      _T("\t\t\"Default\":\"QL_Account\"\n")
                      _T("\t}\n")
                      _T("}");

    // Check that the expected JSON matches what we now it must be
    errors += (res == expect1) ? 0 : 1;

    // Now transform back to SOAP
    SOAPMessage soap2(&json);
    XString msg2 = soap2.GetSoapMessage();
  
    XString expect2 = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
                      _T("<s:Envelope>\n")
                      _T("  <s:Header />\n")
                      _T("  <s:Body>\n")
                      _T("    <Applications>\n")
                      _T("      <Datasources>\n")
                      _T("        <Datasource>develop</Datasource>\n")
                      _T("        <Datasource>test</Datasource>\n")
                      _T("        <Datasource>accept</Datasource>\n")
                      _T("        <Datasource>production</Datasource>\n")
                      _T("      </Datasources>\n")
                      _T("      <Commercial>\n")
                      _T("        <Application>Financials</Application>\n")
                      _T("        <Application>Maintainance</Application>\n")
                      _T("        <Application>Rentals</Application>\n")
                      _T("      </Commercial>\n")
                      _T("      <Default>QL_Account</Default>\n")
                      _T("    </Applications>\n")
                      _T("  </s:Body>\n")
                      _T("</s:Envelope>\n");

    // Check that the resulting SOAP matches what it must be
    errors += (msg2 == expect2) ? 0 : 1;

    // Possibly 0,1 or 2 errors
    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(TestJSONObject)
  {
    Logger::WriteMessage("Testing the JSON Object capability");

    JSONMessage json;

    JSONobject  object;
    JSONpair    pair1(_T("one"));
    JSONpair    pair2(_T("two"));
    JSONpair    pair3(_T("three"));
    pair1.SetValue(1);
    pair2.SetValue(2);
    pair3.SetValue(3);

    object.push_back(pair1);
    object.push_back(pair2);
    object.push_back(pair3);

    json.AddNamedObject(_T("MyObject"),object);

    JSONobject object2;
    JSONpair   pair4(_T("four"));
    JSONpair   pair5(_T("five"));
    JSONpair   pair6(_T("six"));
    pair4.SetValue(4);
    pair5.SetValue(5);
    pair6.SetValue(6);

    object2.push_back(pair4);
    object2.push_back(pair5);
    object2.push_back(pair6);

    json.AddNamedObject(_T("MyObject"),object2,true);

    json.SetWhitespace(true);
    XString total = json.GetJsonMessage();

    XString expected = _T("{\n")
                       _T("\t\"MyObject\":[\n")
                       _T("\t\t{\n")
                       _T("\t\t\t\"one\":1,\n")
                       _T("\t\t\t\"two\":2,\n")
                       _T("\t\t\t\"three\":3\n")
                       _T("\t\t},\n")
                       _T("\t\t{\n")
                       _T("\t\t\t\"four\":4,\n")
                       _T("\t\t\t\"five\":5,\n")
                       _T("\t\t\t\"six\":6\n")
                       _T("\t\t}\n")
                       _T("\t]\n")
                       _T("}");
    // Check that the expected JSON matches what we now it must be
    int errors = (total == expected) ? 0 : 1;

    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(TestJSONFindValue)
  {
    Logger::WriteMessage("Testing the finding of a JSON value");

    int errors = 0;
    XString jsonstring = _T("{ \n")
                         _T("  \"name1\" : \"value1\"\n")
                         _T(" ,\"name2\" : \"value2\"\n")
                         _T(" ,\"name3\" : 33\n")
                         _T("}\n");
    JSONMessage json(jsonstring);

    JSONvalue* val2 = json.FindValue(_T("name2"));
    int val3 = json.GetValueInteger(_T("name3"));
    JSONvalue* obj1 = json.FindValue(_T("name2"),true,true);
    JSONpair*  pair = json.GetObjectElement(obj1,0);

    if(val2->GetString().Compare(_T("value2")))
    {
      // JSON FindValue cannot find the value by name
      ++errors;
    }
    if(val3 != 33)
    {
      // JSON GetValueInteger cannot find integer by name
      ++errors;
    }
    if(pair->m_name.Compare(_T("name1")) || pair->m_value.GetString().Compare(_T("value1")))
    {
      // JSON GetObjectElement cannot find element by index
      ++errors;
    }

    // Test arrays
    jsonstring = _T("{  \"two\":[201,202,203,204.5,205.6789]\n")
                 _T(",\"three\":[301,302,303,304.5,305.6789]\n")
                 _T(",\"four\" :[401,402,403,404.5,405.6789]\n")
                 _T("}");
    JSONMessage two(jsonstring);
    JSONpair*  ar = two.FindPair(_T("three"));
    JSONvalue* vv = two.GetArrayElement(&ar->m_value,3);
    bcd   howmuch = vv->GetNumberBcd();

    if(howmuch != bcd(_T("304.5")))
    {
      // JSON GetArrayElement cannot find element by index
      ++errors;
    }
    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(TestJSONPointer)
  {
    Logger::WriteMessage("Testing the JSONPointer capability");

    int errors = 0;
    XString jsonString = _T("{\n")
                         _T("\t\"MyObject\":[\n")
                         _T("\t\t{\n")
                         _T("\t\t\t\"one\":1,\n")
                         _T("\t\t\t\"two\":2,\n")
                         _T("\t\t\t\"three\":true\n")
                         _T("\t\t},\n")
                         _T("\t\t{\n")
                         _T("\t\t\t\"four\":\"The Fantastic 4\",\n")
                         _T("\t\t\t\"five\":5.17,\n")
                         _T("\t\t\t\"six\":6,\n")
                         _T("\t\t\t\"strange~ger\":42,\n")
                         _T("\t\t\t\"sp/1\":\"7.1.12\"\n")
                         _T("\t\t}\n")
                         _T("\t]\n")
                         _T("}");
    XString jsonCompressed(jsonString);
    jsonCompressed.Replace(_T("\t"),_T(""));
    jsonCompressed.Replace(_T("\n"),_T(""));

    JSONMessage json(jsonString);
    JSONPointer jp(&json,_T("/MyObject/0/two"));

    int result = jp.GetResultInteger();
    errors += result != 2;

    jp.SetPointer(_T("/MyObject/1/six"));
    result = jp.GetResultInteger();
    errors += result != 6;
    errors += jp.GetType()   != JsonType::JDT_number_int;
    errors += jp.GetStatus() != JPStatus::JP_Match_number_int;

    jp.SetPointer(_T(""));
    XString resultJson = jp.GetResultForceToString();
    errors += resultJson.Compare(jsonCompressed) != 0;

    resultJson = jp.GetResultForceToString(true);
    errors += jsonString.Compare(resultJson) != 0;
    errors += jp.GetType()   != JsonType::JDT_object;
    errors += jp.GetStatus() != JPStatus::JP_Match_wholedoc;

    jp.SetPointer(_T("/MyObject"));
    errors += jp.GetResultArray()->size() != 2;
    errors += jp.GetType()   != JsonType::JDT_array;
    errors += jp.GetStatus() != JPStatus::JP_Match_array;

    jp.SetPointer(_T("/MyObject/1"));
    errors += jp.GetResultObject()->size() != 5;
    errors += jp.GetType()   != JsonType::JDT_object;
    errors += jp.GetStatus() != JPStatus::JP_Match_object;

    jp.SetPointer(_T("/MyObject/1/five"));
    bcd testnum = jp.GetResultBCD();
    errors += testnum != bcd(_T("5.17"));
    errors += jp.GetType()   != JsonType::JDT_number_bcd;
    errors += jp.GetStatus() != JPStatus::JP_Match_number_bcd;

    jp.SetPointer(_T("/MyObject/1/four"));
    XString four = jp.GetResultString();
    errors += four.Compare(_T("The Fantastic 4")) != 0;
    errors += jp.GetType()   != JsonType::JDT_string;
    errors += jp.GetStatus() != JPStatus::JP_Match_string;

    jp.SetPointer(_T("/MyObject/0/three"));
    errors += jp.GetResultConstant() != JsonConst::JSON_TRUE;
    errors += jp.GetType()   != JsonType::JDT_const;
    errors += jp.GetStatus() != JPStatus::JP_Match_constant;

    // Escape of an '/' char in a name
    jp.SetPointer(_T("/MyObject/1/sp~11"));
    XString version = jp.GetResultString();
    errors += version.Compare(_T("7.1.12")) != 0;

    // Escape of an '~' char in a name
    jp.SetPointer(_T("/MyObject/1/strange~0ger"));
    int reason = jp.GetResultInteger();
    errors += reason != 42;

    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(TestJSONPathInPointer)
  {
    Logger::WriteMessage("Testing the extended paths in a JSONPointer");

    int errors = 0;
    XString jsonString = _T("{\n")
                         _T("\t\"MyObject\":[\n")
                         _T("\t\t{\n")
                         _T("\t\t\t\"one\":1,\n")
                         _T("\t\t\t\"two\":2,\n")
                         _T("\t\t\t\"three\":true\n")
                         _T("\t\t},\n")
                         _T("\t\t{\n")
                         _T("\t\t\t\"four\":\"The Fantastic 4\",\n")
                         _T("\t\t\t\"five\":5.17,\n")
                         _T("\t\t\t\"six\":6,\n")
                         _T("\t\t\t\"strange~ger\":42,\n")
                         _T("\t\t\t\"sp.1\":\"7.1.12\"\n")
                         _T("\t\t}\n")
                         _T("\t]\n")
                         _T("}");
    XString jsonCompressed(jsonString);
    jsonCompressed.Replace(_T("\t"),_T(""));
    jsonCompressed.Replace(_T("\n"),_T(""));

    JSONMessage json(jsonString);
    JSONPointer jp(&json,_T("$.MyObject.0.two"));

    int result = jp.GetResultInteger();
    errors += result != 2;

    jp.SetPointer(_T("$.MyObject.1.six"));
    result = jp.GetResultInteger();
    errors += result != 6;
    errors += jp.GetType() != JsonType::JDT_number_int;
    errors += jp.GetStatus() != JPStatus::JP_Match_number_int;

    jp.SetPointer(_T("$"));
    XString resultJson = jp.GetResultForceToString();
    errors += resultJson.Compare(jsonCompressed) != 0;

    resultJson = jp.GetResultForceToString(true);
    errors += jsonString.Compare(resultJson) != 0;
    errors += jp.GetType() != JsonType::JDT_object;
    errors += jp.GetStatus() != JPStatus::JP_Match_wholedoc;

    jp.SetPointer(_T("$.MyObject"));
    errors += jp.GetResultArray()->size() != 2;
    errors += jp.GetType() != JsonType::JDT_array;
    errors += jp.GetStatus() != JPStatus::JP_Match_array;

    jp.SetPointer(_T("$.MyObject.1"));
    errors += jp.GetResultObject()->size() != 5;
    errors += jp.GetType() != JsonType::JDT_object;
    errors += jp.GetStatus() != JPStatus::JP_Match_object;

    jp.SetPointer(_T("$.MyObject.1.five"));
    bcd testnum = jp.GetResultBCD();
    errors += testnum != bcd(_T("5.17"));
    errors += jp.GetType() != JsonType::JDT_number_bcd;
    errors += jp.GetStatus() != JPStatus::JP_Match_number_bcd;

    jp.SetPointer(_T("$.MyObject.1.four"));
    XString four = jp.GetResultString();
    errors += four.Compare(_T("The Fantastic 4")) != 0;
    errors += jp.GetType() != JsonType::JDT_string;
    errors += jp.GetStatus() != JPStatus::JP_Match_string;

    jp.SetPointer(_T("$.MyObject.0.three"));
    errors += jp.GetResultConstant() != JsonConst::JSON_TRUE;
    errors += jp.GetType() != JsonType::JDT_const;
    errors += jp.GetStatus() != JPStatus::JP_Match_constant;

    // Escape of an '/' char in a name
    jp.SetPointer(_T("$.MyObject.1.sp~11"));
    XString version = jp.GetResultString();
    errors += version.Compare(_T("7.1.12")) != 0;

    // Escape of an '~' char in a name
    jp.SetPointer(_T("$.MyObject.1.strange~0ger"));
    int reason = jp.GetResultInteger();
    errors += reason != 42;

    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(TestJSONUnicodeChars)
  {
    Logger::WriteMessage("Testing the JSON special Unicode UTF-16 chars");

    int errors = 0;
    XString jsonString = _T("{ \"dollar\" : \"\\u00E9\\u00E9\\u006E \\u20AC = one €\" }");
    JSONMessage json(jsonString);
    XString test = json.GetJsonMessage();
    XString expected(_T("{\"dollar\":\"één € = one €\"}"));
    errors = test.Compare(expected) != 0;

    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(TestJSONPath)
  {
    Logger::WriteMessage("Testing the JSONPath capabilities");

    int errors = 0;
    XString jsonString = _T("{\t\"array\": [\n")
                         _T("\t\t{\t\"one\"  : 1  },\n")
                         _T("\t\t{\t\"two\"  : 2  },\n")
                         _T("\t\t{\t\"three\": 3  },\n")
                         _T("\t\t{\t\"four\" : 4  },\n")
                         _T("\t\t{\t\"five\" : 5  },\n")
                         _T("\t\t{\t\"six\"  : 6  },\n")
                         _T("\t\t{\t\"seven\": 7  },\n")
                         _T("\t\t{\t\"eight\": 8  },\n")
                         _T("\t\t{\t\"nine\" : 9  },\n")
                         _T("\t\t{\t\"ten\"  : 10 },\n")
                         _T("\t\t{\t\"eleven\": [\n")
                         _T("\t\t\t\t{\n")
                         _T("\t\t\t\t\t\"renault\": \"twingo\",\n")
                         _T("\t\t\t\t\t\"citroen\": \"ds\",\n")
                         _T("\t\t\t\t\t\"peugeot\": \"8005\"\n")
                         _T("\t\t\t\t},\n")
                         _T("\t\t\t\t{\n")
                         _T("\t\t\t\t\t\"opel\": \"astra\",\n")
                         _T("\t\t\t\t\t\"bmw\": \"3series\",\n")
                         _T("\t\t\t\t\t\"mercedes\": \"amg\"\n")
                         _T("\t\t\t\t},\n")
                         _T("\t\t\t\t{\n")
                         _T("\t\t\t\t\t\"ford\": \"focus\",\n")
                         _T("\t\t\t\t\t\"fiat\": \"750\",\n")
                         _T("\t\t\t\t\t\"seat\": \"leon\"\n")
                         _T("\t\t\t\t}\n")
                         _T("\t\t\t]\n")
                         _T("\t\t}\n")
                         _T("\t]\n")
                         _T("}");
    JSONMessage json(jsonString);
    JSONPath path(json,_T("$.array[4].five"));

    int result = 0;
    if(path.GetStatus() == JPStatus::JP_Match_number_int)
    {
      result = path.GetFirstResult()->GetNumberInt();
    }
    errors = result != 5;

    path.SetPath(_T("$.array[10].eleven[2].ford"));
    if(path.GetStatus() != JPStatus::JP_INVALID)
    {
      XString name = path.GetFirstResult()->GetString();
      errors = name.Compare(_T("focus")) != 0;
    }

    path.SetPath(_T("$.array[0:8:2]")); // selects 0,2,4,6
    errors += path.GetNumberOfMatches() != 4;
    errors += path.GetResult(0)->GetObject()[0].GetNumberInt() != 1;
    errors += path.GetResult(1)->GetObject()[0].GetNumberInt() != 3;
    errors += path.GetResult(2)->GetObject()[0].GetNumberInt() != 5;
    errors += path.GetResult(3)->GetObject()[0].GetNumberInt() != 7;

    path.SetPath(_T("$.array[0:8:-2]")); // selects 8,6,4,2
    errors += path.GetNumberOfMatches() != 4;
    errors += path.GetResult(0)->GetObject()[0].GetNumberInt() != 8;
    errors += path.GetResult(1)->GetObject()[0].GetNumberInt() != 6;
    errors += path.GetResult(2)->GetObject()[0].GetNumberInt() != 4;
    errors += path.GetResult(3)->GetObject()[0].GetNumberInt() != 2;

    path.SetPath(_T("$.array[10].eleven[0,1]"));
    errors += path.GetNumberOfMatches() != 2;

    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(TestJSONMultiPartFormData1)
  {
    Logger::WriteMessage("Testing the JSON encapsulated in a MultiPart message");
    int errors = 0;
    const char* buffer = "------WebKitFormBoundarydZ38XIr1QTTS2IMb\r\n"
                         "Content-Disposition: form-data; name=\"277153\"\r\n"
                         "\r\n"
                         "{\"ID\":\"0\",\"Old: \":\"\",\"New\":\"\",\"Event\":\"Start\"}\r\n"
                         "------WebKitFormBoundarydZ38XIr1QTTS2IMb\r\n"
                         "Content-Disposition: form-data; name=\"44074\"\r\n"
                         "\r\n"
                         "{\"ID\":\"44074\",\"Old:\":\"bar2\",\"New\":\"bar5\",\"Event\":\"Blur\"}\r\n"
                         "------WebKitFormBoundarydZ38XIr1QTTS2IMb\r\n"
                         "Content-Disposition: form-data; name=\"277154\"\r\n"
                         "\r\n"
                         "{\"ID\":\"277153\",\"Old:\":\"\",\"New\":\"\",\"Event\":\"Click\"}\r\n"
                         "------WebKitFormBoundarydZ38XIr1QTTS2IMb\r\n"
                         "Content-Disposition: form-data; name=\"277155\"\r\n"
                         "\r\n"
                         "{\"ID\":\"1\",\"Old: \":\"\",\"New\":\"\",\"Event\":\"Close\"}\r\n"
                         "------WebKitFormBoundarydZ38XIr1QTTS2IMb--";

    int size = (int) strlen(buffer);

    FileBuffer buf;
    buf.AddBuffer((uchar*)buffer,size);
    XString contentType(_T("multipart/form-data; boundary=----WebKitFormBoundarydZ38XIr1QTTS2IMb"));

    // Test Windows-1252 or UTF-16 input
    MultiPartBuffer multi(FormDataType::FD_MULTIPART);
    multi.ParseBuffer(contentType,&buf); // <- DEFAULTS !!

    size_t parts = multi.GetParts();
    for(size_t ind = 0; ind < parts; ++ind)
    {
      MultiPart* part = multi.GetPart((int) ind);

      Logger::WriteMessage(_T("Part: ") + part->GetData());
    }
    XString test = multi.GetPart(3)->GetData();
    if(test.Compare(_T("{\"ID\":\"1\",\"Old: \":\"\",\"New\":\"\",\"Event\":\"Close\"}\r\n")))
    {
      // ERROR: Multipart buffer of 4 JSON's not working
      ++errors;
    }
    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(TestJSONMultiPartFormData2)
  {
    Logger::WriteMessage("Testing the JSON encapsulated in a MultiPart message");
    int errors = 0;
    const wchar_t* buffer = L"------WebKitFormBoundarydZ38XIr1QTTS2IMb\r\n"
                            L"Content-Disposition: form-data; name=\"277153\"\r\n"
                            L"\r\n"
                            L"{\"ID\":\"0\",\"Old: \":\"\",\"New\":\"\",\"Event\":\"Start\"}\r\n"
                            L"------WebKitFormBoundarydZ38XIr1QTTS2IMb\r\n"
                            L"Content-Disposition: form-data; name=\"44074\"\r\n"
                            L"\r\n"
                            L"{\"ID\":\"44074\",\"Old:\":\"bar2\",\"New\":\"bar5\",\"Event\":\"Blur\"}\r\n"
                            L"------WebKitFormBoundarydZ38XIr1QTTS2IMb\r\n"
                            L"Content-Disposition: form-data; name=\"277154\"\r\n"
                            L"\r\n"
                            L"{\"ID\":\"277153\",\"Old:\":\"\",\"New\":\"\",\"Event\":\"Click\"}\r\n"
                            L"------WebKitFormBoundarydZ38XIr1QTTS2IMb\r\n"
                            L"Content-Disposition: form-data; name=\"277155\"\r\n"
                            L"\r\n"
                            L"{\"ID\":\"1\",\"Old: \":\"\",\"New\":\"\",\"Event\":\"Close\"}\r\n"
                            L"------WebKitFormBoundarydZ38XIr1QTTS2IMb--";

    int size = (int) wcslen(buffer) * sizeof(wchar_t);

    FileBuffer buf;
    buf.AddBuffer((uchar*)buffer,size);
    XString contentType(_T("multipart/form-data; boundary=----WebKitFormBoundarydZ38XIr1QTTS2IMb"));

    // Test Windows-1252 or UTF-16 input
    MultiPartBuffer multi(FormDataType::FD_MULTIPART);
    multi.ParseBuffer(contentType,&buf,false,true); // <- OVERRIDE for charsize

    size_t parts = multi.GetParts();
    for(size_t ind = 0; ind < parts; ++ind)
    {
      MultiPart* part = multi.GetPart((int) ind);

      Logger::WriteMessage(_T("Part: ") + part->GetData());
    }
    XString test = multi.GetPart(3)->GetData();
    if(test.Compare(_T("{\"ID\":\"1\",\"Old: \":\"\",\"New\":\"\",\"Event\":\"Close\"}\r\n")))
    {
      // ERROR: Multipart buffer of 4 JSON's not working
      ++errors;
    }
    Assert::AreEqual(0,errors);
  }
};
}