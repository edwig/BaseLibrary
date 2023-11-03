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
#include <XMLMessage.h>
#include <XPath.h>
#include <SOAPMessage.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace BaseLibraryUnitTests
{
TEST_CLASS(XMLTests)
{
public:

  TEST_METHOD(TestXML_XPath)
  {
    Logger::WriteMessage("Testing XPath");

    XString xml;
    XMLMessage msg;

    xml = _T("<bookstore>\n")
          _T("\n")
          _T("  <book category=\"cooking\">\n")
          _T("    <title lang=\"en\">Everyday Italian</title>\n")
          _T("    <author>Giada De Laurentiis</author>\n")
          _T("    <year>2005</year>\n")
          _T("    <price>30.00</price>\n")
          _T("  </book>\n")
          _T("\n")
          _T("  <book category=\"children\">\n")
          _T("    <title lang=\"en\">Harry Potter</title>\n")
          _T("    <author>J K. Rowling</author>\n")
          _T("    <year>2005</year>\n")
          _T("    <price>29.99</price>\n")
          _T("  </book>\n")
          _T("\n")
          _T("  <book category=\"web\">\n")
          _T("    <title lang=\"EN-US\">XQuery Kick Start</title>\n")
          _T("    <author>James McGovern</author>\n")
          _T("    <author>Per Bothner</author>\n")
          _T("    <author>Kurt Cagle</author>\n")
          _T("    <author>James Linn</author>\n")
          _T("    <author>Vaidyanathan Nagarajan</author>\n")
          _T("    <year>2003</year>\n")
          _T("    <price>49.99</price>\n")
          _T("  </book>\n")
          _T("\n")
          _T("  <book category=\"web\">\n")
          _T("    <title>Start Learning XML</title>\n")
          _T("    <author>Erik T. Ray</author>\n")
          _T("    <year>2003</year>\n")
          _T("    <price>39.95</price>\n")
          _T("  </book>\n")
          _T("\n")
          _T("</bookstore>\n");

    msg.ParseMessage(xml);
    if(msg.GetInternalError() != XmlError::XE_NoError)
    {
      Assert::Fail();
    }

    // Test array indexing (1-based!)
    XPath path(&msg,_T("/bookstore/book[2]"));
    if(path.GetNumberOfMatches() == 1)
    {
      if(path.GetFirstResult()->GetAttributes().front().m_value != _T("children"))
      {
        Assert::Fail();
      }
    }
    else Assert::Fail();


    // Check regular node value
    path.SetPath(_T("/bookstore/book[4]/price"));
    if(path.GetNumberOfMatches() == 1)
    {
      bcd price(path.GetFirstResult()->GetValue());
      if(price.AsString() != _T("39.95"))
      {
        Assert::Fail();
      }
    }
    else Assert::Fail();

    // Test finding all the attributes
    path.SetPath(_T("//title[@lang]"));
    if(path.GetNumberOfMatches() != 3)
    {
      Assert::Fail();
    }

    // Test getting the last of an array
    path.SetPath(_T("/bookstore/book[last()]"));
    if(path.GetNumberOfMatches() == 1)
    {
      bcd price(path.GetFirstResult()->GetChildren()[3]->GetValue());
      if(price.AsString() != _T("39.95"))
      {
        Assert::Fail();
      }
    }
    else Assert::Fail();

    // Test number expression
    path.SetPath(_T("/bookstore//book[price<35]"));
    if(path.GetNumberOfMatches() != 2)
    {
      Assert::Fail();
    }

    path.SetPath(_T("/bookstore/book[contains(title,'Start')]"));
    if(path.GetNumberOfMatches() != 2)
    {
      Assert::Fail();
    }

    path.SetPath(_T("/bookstore/book[starts-with(title,'Start')]"));
    if(path.GetNumberOfMatches() != 1)
    {
      Assert::Fail();
    }
    Logger::WriteMessage("XPath working normally.");
  }

  TEST_METHOD(TestXML_CDATA_UTF8)
  {
    Logger::WriteMessage("Testing XMLMessage with UTF-8 and CDATA");

    XMLMessage msg;
    bool result = false;
    // Message including UTF-8 encoding (2e node <Address>)
    XString text(_T("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n")
                 _T("<RootNode xmlns=\"one\">\n")
                 _T("  <a:Node1 atr1=\"12\" xmlns:a=\"http://one.two.com/\">testing</a:Node1>\n")
                 _T("  <Node2>two</Node2>\n")
                 _T("  <Node3 i:nil=\"true\"/>\n")
                 _T("  <a:Unit>\n")
                 _T("     <Number>12</Number>\n")
                 _T("     <Address>abc12</Address>\n")
                 _T("  </a:Unit>\n")
                 _T("  <a:Unit>\n")
                 _T("     <Number>14</Number>\n")
                 _T("     <Address>Sôn bôn de magnà el véder, el me fa minga mal. Mister Gênépèllëtje</Address>\n")
                 _T("  </a:Unit>\n")
                 _T("  <Extra><![CDATA[This is an extra piece of tekst]]></Extra>\n")
                 _T("</RootNode>\n"));

    msg.ParseMessage(text);
    if(msg.GetInternalError() != XmlError::XE_NoError)
    {
      Assert::Fail();
    }
    else
    {
      XMLElement* extraNode = msg.FindElement("Extra");
      if(extraNode)
      {
        XString extra = extraNode->GetValue();
        result = extra.Compare(_T("This is an extra piece of tekst")) == 0;
      }
    }
    Assert::AreEqual(true,result);
  }

  TEST_METHOD(TestXML_ErrorReporting)
  {
    Logger::WriteMessage("Testing the XML error reporting capabilities");

    XMLMessage msg;
    bool result = false;
    // Incomplete message
    XString text(_T("<?xml  version=\"1.0\" encoding=\"utf-8\" ?>\n")
                 _T("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope\">\n")
                 _T("<soap:Body>\n")
                 _T("  <TestSoapException_FirstOperation>\n")
                 _T("    <FirstOperationMessageIn>\n")
                 _T("      <Field01>Getal</Field01>\n")
                 _T("      <Field07>Getal</Field07>\n")
                 _T("    /FirstOperationMessageIn>\n")
                 _T("  </TestSoapException_FirstOperation>\n")
                 _T("</soap:Body>\n")
                 _T("</soap:Envelope>\n"));

    msg.ParseMessage(text);

    if(msg.GetInternalError() != XmlError::XE_NoError)
    {
      if(msg.GetInternalError() == XmlError::XE_NotAnXMLMessage &&
         msg.GetInternalErrorString() == _T("ERROR parsing XML:  Element [FirstOperationMessageIn] : /FirstOperationMessageIn>\n")
                                         _T("  </TestSoapException_FirstOperation>\n")
                                         _T("</soap:Body>\n")
                                         _T("</soap:Envelope>\n"))
      {
        result = true;
      }
    }
    Assert::AreEqual(true,result);
  }

  TEST_METHOD(TestXML_HexNumbersParsing)
  {
    Logger::WriteMessage("Testing XML hex number escaping characters");

    SOAPMessage msg;
    bool result = false;
    // Incomplete message
    XString text(_T("<?xml  version=\"1.0\" encoding=\"utf-8\" ?>\n")
                 _T("<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\"\n")
                 _T("       soap:encodingStyle=\"http://www.w3.org/2003/12/soap-encoding\">\n")
                 _T("<soap:Body>\n")
                 _T("  <CreateAddress>\n")
                 _T("    <CreateAddressRequest>\n")
                 _T("      <Address>\n")
                 _T("        <Streetname>a</Streetname>\n")
                 _T("        <Residential>b</Residential>\n")
                 _T("        <County>c</County>\n")
                 _T("        <Postcode>o&#xD;k</Postcode>\n")
                 _T("        <Housenumber>6</Housenumber>\n")
                 _T("        <ExtraLetter>e</ExtraLetter>\n")
                 _T("        <Extra>a&#xa;b</Extra>\n")
                 _T("        <Country>h</Country>\n")
                 _T("      </Address>\n")
                 _T("    </CreateAddressRequest>\n")
                 _T("  </CreateAddress>\n")
                 _T("</soap:Body>\n")
                 _T("</soap:Envelope>\n"));

    msg.ParseMessage(text);
    if(msg.GetInternalError() != XmlError::XE_NoError)
    {
      Assert::Fail();
    }
    else
    {
      XMLElement* aand = msg.FindElement(_T("Country"));
      XMLElement* post = msg.FindElement(_T("Postcode"));
      XMLElement* toev = msg.FindElement(_T("Extra"));
      if(aand && aand->GetValue() == _T("h"))
      {
        if(post && post->GetValue() == _T("o\rk"))
        {
          if(toev && toev->GetValue() == _T("a\nb"))
          {
            result = true;
          }
        }
      }
    }
    Assert::AreEqual(true,result);
  }

  TEST_METHOD(TestXML_UTF8)
  {
    Logger::WriteMessage("Testing UTF-8 support in XML (at least 4 codepages)");

    bool result = false;
    XString namesp = DEFAULT_NAMESPACE;
    XString action = _T("MyAction");

    SOAPMessage msg(namesp,action);
    msg.SetParameter(_T("PriceTag"),_T("€ 42,00"));
    msg.SetEncoding(Encoding::UTF8);

    XString test1 = msg.GetSoapMessage();

    SOAPMessage msg2(test1,true);
    XString priceTag = msg2.GetParameter(_T("PriceTag"));

    // Test for positive result
    XString expected(_T("€ 42,00"));
    expected = EncodeStringForTheWire(expected);
    if(priceTag == expected)
    {
      result = true;
    }
    Assert::AreEqual(true,result);
  }

  TEST_METHOD(TestXML_Windows1252)
  {
    Logger::WriteMessage("Testing default Active-Code-Page support in XML");

    bool result = false;
    XString namesp = DEFAULT_NAMESPACE;
    XString action = _T("MyAction");

    SOAPMessage msg(namesp,action);
    msg.SetParameter(_T("PriceTag"),_T("€ 42,00"));
    msg.SetEncoding(Encoding::Default);

    XString test1 = msg.GetSoapMessage();

    SOAPMessage msg2(test1,true);
    XString priceTag = msg2.GetParameter(_T("PriceTag"));

    // Test for positive result
    if(priceTag == _T("€ 42,00"))
    {
      result = true;
    }
    Assert::AreEqual(true,result);
  }
};
}