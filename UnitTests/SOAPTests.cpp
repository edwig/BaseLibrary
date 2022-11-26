/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: SOAPTests.cpp
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

#include <SOAPMessage.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
	TEST_CLASS(SOAPMessageTests)
	{
	public:
		
		TEST_METHOD(TestSoapCleanup1)
		{
      Logger::WriteMessage("Testing SOAPMessage Cleanup 1");

      XString namesp("http://docs.hercules.nl/wocas");
      XString method("NewDocument");

      SOAPMessage msg(namesp,method);
      msg.SetParameter("One",1);
      msg.SetParameter("Two",2);
      msg.SetParameter("Three","");
      msg.SetParameter("Four","This is four");
      msg.SetParameter("Five","");

      msg.CleanUp();

      XString total = msg.GetSoapMessage();

      XString expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                         "<s:Envelope xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:a=\"http://www.w3.org/2005/08/addressing\" xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\n"
                         "  <s:Header>\n"
                         "    <a:Action s:mustUnderstand=\"true\">http://docs.hercules.nl/wocas/NewDocument</a:Action>\n"
                         "  </s:Header>\n"
                         "  <s:Body>\n"
                         "    <NewDocument xmlns=\"http://docs.hercules.nl/wocas\">\n"
                         "      <One>1</One>\n"
                         "      <Two>2</Two>\n"
                         "      <Four>This is four</Four>\n"
                         "    </NewDocument>\n"
                         "  </s:Body>\n"
                         "</s:Envelope>\n";

      Assert::AreEqual(expected.GetString(),total.GetString());
		}

    TEST_METHOD(TestSoapCleanup2)
    {
      Logger::WriteMessage("Testing SOAPMessage Cleanup 2");

      XString namesp("http://docs.hercules.nl/wocas");
      XString method("NewDocument");

      SOAPMessage msg(namesp,method);
      msg.SetParameter("One",1);
      msg.SetParameter("Two",2);
      msg.SetParameter("Three","");
      msg.SetParameter("Four","This is four");
      msg.SetParameter("Five","");
      
      XMLElement* param = msg.GetParameterObjectNode();
      XMLElement* six   = msg.AddElement(param,"Six",XDT_String,"");
      XMLElement* six2  = msg.AddElement(six,"Six2",XDT_String,"S2");

      msg.CleanUp();

      XString total = msg.GetSoapMessage();

      XString expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                         "<s:Envelope xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:a=\"http://www.w3.org/2005/08/addressing\" xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\n"
                         "  <s:Header>\n"
                         "    <a:Action s:mustUnderstand=\"true\">http://docs.hercules.nl/wocas/NewDocument</a:Action>\n"
                         "  </s:Header>\n"
                         "  <s:Body>\n"
                         "    <NewDocument xmlns=\"http://docs.hercules.nl/wocas\">\n"
                         "      <One>1</One>\n"
                         "      <Two>2</Two>\n"
                         "      <Four>This is four</Four>\n"
                         "      <Six>\n"
                         "        <Six2>S2</Six2>\n"
                         "      </Six>\n"
                         "    </NewDocument>\n"
                         "  </s:Body>\n"
                         "</s:Envelope>\n";

      Assert::AreEqual(expected.GetString(),total.GetString());
    }

    TEST_METHOD(TestSoapCleanup3)
    {
      Logger::WriteMessage("Testing SOAPMessage Cleanup 3");

      XString namesp("http://docs.hercules.nl/wocas");
      XString method("NewDocument");

      SOAPMessage msg(namesp,method);
      msg.SetParameter("One",1);
      msg.SetParameter("Two",2);
      msg.SetParameter("Three","");
      msg.SetParameter("Four","This is four");
      msg.SetParameter("Five","");
      
      XMLElement* param = msg.GetParameterObjectNode();
      XMLElement* six   = msg.AddElement(param,"Six",XDT_String,"");
      XMLElement* six2  = msg.AddElement(six,"Six2",XDT_String,"S2");

      XMLElement* seven  = msg.AddElement(param, "Seven", XDT_String,"");
      XMLElement* seven2 = msg.AddElement(seven, "Seven2",XDT_String,"");
      XMLElement* seven3 = msg.AddElement(seven2,"Seven3",XDT_String,"");
      XMLElement* seven4 = msg.AddElement(seven3,"Seven4",XDT_String,"");

      msg.CleanUp();
      XString total = msg.GetSoapMessage();

      XString expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                         "<s:Envelope xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:a=\"http://www.w3.org/2005/08/addressing\" xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\n"
                         "  <s:Header>\n"
                         "    <a:Action s:mustUnderstand=\"true\">http://docs.hercules.nl/wocas/NewDocument</a:Action>\n"
                         "  </s:Header>\n"
                         "  <s:Body>\n"
                         "    <NewDocument xmlns=\"http://docs.hercules.nl/wocas\">\n"
                         "      <One>1</One>\n"
                         "      <Two>2</Two>\n"
                         "      <Four>This is four</Four>\n"
                         "      <Six>\n"
                         "        <Six2>S2</Six2>\n"
                         "      </Six>\n"
                         "    </NewDocument>\n"
                         "  </s:Body>\n"
                         "</s:Envelope>\n";

      Assert::AreEqual(expected.GetString(),total.GetString());
    }
  };
}
