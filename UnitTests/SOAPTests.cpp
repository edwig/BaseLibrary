/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: SOAPTests.cpp
//
// BaseLibrary: Indispensable general objects and functions
//              UNIT TESTS for the BaseLibrary
// 
// Copyright (c) 2014-2025 ir. W.E. Huisman
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
#include <Namespace.h>
#include <ServiceQuality.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
	TEST_CLASS(SOAPMessageTests)
	{
  private:
    void PrintSoapFault(SOAPMessage& p_msg)
    {
      Logger::WriteMessage(_T("\n---- Error ---------------------------------"));
      Logger::WriteMessage(p_msg.GetFault());
      Logger::WriteMessage(_T("\n--------------------------------------------"));
    }

    void Testsplit(XString p_soapAction,XString p_expect)
    {
      int result = 0;
      XString namesp,action;
      if(SplitNamespaceAndAction(p_soapAction,namesp,action))
      {
        if(!(namesp == p_expect && action == _T("command")))
        {
          Assert::Fail();
        }
      }
      else
      {
        if(action != _T("command"))
        {
          Assert::Fail();
        }
      }
    }
  
  public:
    TEST_METHOD(TestSoapCleanup1)
    {
      Logger::WriteMessage("Testing SOAPMessage Cleanup 1");

      XString namesp(_T("http://docs.hercules.nl/wocas"));
      XString method(_T("NewDocument"));

      SOAPMessage msg(namesp,method);
      msg.SetParameter(_T("One"),1);
      msg.SetParameter(_T("Two"),2);
      msg.SetParameter(_T("Three"),_T(""));
      msg.SetParameter(_T("Four"), _T("This is four"));
      msg.SetParameter(_T("Five"), _T(""));

      msg.CleanUp();

      XString total = msg.GetSoapMessage();

      XString expected = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
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
                         "</s:Envelope>\n");

      Assert::AreEqual(expected.GetString(),total.GetString());
		}

    TEST_METHOD(TestSoapCleanup2)
    {
      Logger::WriteMessage("Testing SOAPMessage Cleanup 2");

      XString namesp(_T("http://docs.hercules.nl/wocas"));
      XString method(_T("NewDocument"));

      SOAPMessage msg(namesp,method);
      msg.SetParameter(_T("One"),1);
      msg.SetParameter(_T("Two"),2);
      msg.SetParameter(_T("Three"),_T(""));
      msg.SetParameter(_T("Four"), _T("This is four"));
      msg.SetParameter(_T("Five"), _T(""));
      
      XMLElement* param = msg.GetParameterObjectNode();
      XMLElement* six   = msg.AddElement(param,_T("Six"), XDT_String,_T(""));
      XMLElement* six2  = msg.AddElement(six,  _T("Six2"),XDT_String,_T("S2"));

      msg.CleanUp();

      XString total = msg.GetSoapMessage();

      XString expected = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
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
                         "</s:Envelope>\n");

      Assert::AreEqual(expected.GetString(),total.GetString());
    }

    TEST_METHOD(TestSoapCleanup3)
    {
      Logger::WriteMessage("Testing SOAPMessage Cleanup 3");

      XString namesp(_T("http://docs.hercules.nl/wocas"));
      XString method(_T("NewDocument"));

      SOAPMessage msg(namesp,method);
      msg.SetParameter(_T("One"),1);
      msg.SetParameter(_T("Two"),2);
      msg.SetParameter(_T("Three"),_T(""));
      msg.SetParameter(_T("Four"), _T("This is four"));
      msg.SetParameter(_T("Five"), _T(""));
      
      XMLElement* param = msg.GetParameterObjectNode();
      XMLElement* six   = msg.AddElement(param,_T("Six"),XDT_String,_T(""));
      XMLElement* six2  = msg.AddElement(six, _T("Six2"),XDT_String,_T("S2"));

      XMLElement* seven  = msg.AddElement(param, _T("Seven"), XDT_String,_T(""));
      XMLElement* seven2 = msg.AddElement(seven, _T("Seven2"),XDT_String,_T(""));
      XMLElement* seven3 = msg.AddElement(seven2,_T("Seven3"),XDT_String,_T(""));
      XMLElement* seven4 = msg.AddElement(seven3,_T("Seven4"),XDT_String,_T(""));

      msg.CleanUp();
      XString total = msg.GetSoapMessage();

      XString expected = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
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
                         "</s:Envelope>\n");

      Assert::AreEqual(expected.GetString(),total.GetString());
    }

    TEST_METHOD(TestPOS)
    {
      Logger::WriteMessage(_T("TESTING Plain-Old-Soap"));

      bool result = false;
      XString soap(_T("<FirstCommand>\n")
                   _T("   <Param1>first</Param1>\n")
                   _T("   <Param2 />\n")
                   _T("   <Param3>third</Param3>\n")
                   _T("</FirstCommand>\n"));


      SOAPMessage msg(soap);
      if(msg.GetErrorState())
      {
        PrintSoapFault(msg);
      }
      else
      {
        XString action = msg.GetSoapAction();
        XString param1 = msg.GetParameter(_T("Param1"));
        XString param2 = msg.GetParameter(_T("Param2"));
        XString param3 = msg.GetParameter(_T("Param3"));

        Logger::WriteMessage(_T("Soap action: ") + action);
        Logger::WriteMessage(_T("Parameter 1: ") + param1);
        Logger::WriteMessage(_T("Parameter 2: ") + param2);
        Logger::WriteMessage(_T("Parameter 3: ") + param3);
        if(action == _T("FirstCommand") &&
           param1 == _T("first") &&
           param2.IsEmpty() &&
           param3 == _T("third") &&
           msg.GetSoapVersion() == SoapVersion::SOAP_POS)
        {
          result = true;
        }
      }
      Assert::AreEqual(true,result);
    }

    TEST_METHOD(TestSOAPNoWSAddressing)
    {
      Logger::WriteMessage("Testing SOAP 1.1 without WS-Addressing");

      bool result = false;
      XString soap(_T("<Envelope>\n")
                   _T("  <Header>\n")
                   _T("    <MessageID>12345</MessageID>\n")
                   _T("  </Header>\n")
                   _T("<Body>\n")
                   _T("  <FirstCommand>\n")
                   _T("    <Param1>first</Param1>\n")
                   _T("    <Param2 />\n")
                   _T("    <Param3>third</Param3>\n")
                   _T("  </FirstCommand>\n")
                   _T("</Body>\n")
                   _T("</Envelope>\n"));

      SOAPMessage msg(soap);
      if(msg.GetErrorState())
      {
        PrintSoapFault(msg);
      }
      else
      {
        XString action = msg.GetSoapAction();
        XString param1 = msg.GetParameter(_T("Param1"));
        XString param2 = msg.GetParameter(_T("Param2"));
        XString param3 = msg.GetParameter(_T("Param3"));

        Logger::WriteMessage(_T("Soap action: ") + action);
        Logger::WriteMessage(_T("Parameter 1: ") + param1);
        Logger::WriteMessage(_T("Parameter 2: ") + param2);
        Logger::WriteMessage(_T("Parameter 3: ") + param3);
        if(action == _T("FirstCommand") &&
           param1 == _T("first") &&
           param2.IsEmpty() &&
           param3 == _T("third") &&
           msg.GetSoapVersion() == SoapVersion::SOAP_11)
        {
          result = true;
        }
      }
      Assert::AreEqual(true,result);
    }

    TEST_METHOD(TestSOAPWithWSAddressing)
    {
      Logger::WriteMessage("Testing SOAP 1.2 with WS-Addressing");

      bool result = false;
      XString soap(_T("<Envelope>\n")
                   _T("  <Header>\n")
                   _T("    <Action mustUnderstand=\"1\">http://services.marlin.org/Test/SmallObject</Action>\n")
                   _T("  </Header>\n")
                   _T("<Body>\n")
                   _T("  <SmallObject xmlns=\"http://services.marlin.org/Test\">\n")
                   _T("    <Param1>first</Param1>\n")
                   _T("    <Param2 />\n")
                   _T("    <Param3>third</Param3>\n")
                   _T("  </SmallObject>\n")
                   _T("</Body>\n")
                   _T("</Envelope>\n"));

      SOAPMessage msg(soap);
      if(msg.GetErrorState())
      {
        PrintSoapFault(msg);
      }
      else
      {
        XString action = msg.GetSoapAction();
        XString param1 = msg.GetParameter(_T("Param1"));
        XString param2 = msg.GetParameter(_T("Param2"));
        XString param3 = msg.GetParameter(_T("Param3"));

        Logger::WriteMessage(_T("Soap action: ") + action);
        Logger::WriteMessage(_T("Parameter 1: ") + param1);
        Logger::WriteMessage(_T("Parameter 2: ") + param2);
        Logger::WriteMessage(_T("Parameter 3: ") + param3);
        if(action == _T("SmallObject") &&
           param1 == _T("first") &&
           param2.IsEmpty() &&
           param3 == _T("third") &&
           msg.GetSoapVersion() == SoapVersion::SOAP_12)
        {
          result = true;
        }
      }
      Assert::AreEqual(true,result);
    }

    TEST_METHOD(TestSOAPWithWSAddressingFull)
    {
      Logger::WriteMessage("Testing SOAP 1.2 with full WS-Addressing");

      bool result = false;
      XString soap(_T("<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:a=\"http://www.w3.org/2005/08/addressing\">\n")
                   _T("  <s:Header>\n")
                   _T("    <a:Action s:mustUnderstand=\"1\">http://schemas.marlin.org/general/services/2015/IContainer/CreateContainerResponse</a:Action>\n")
                   _T("    <a:RelatesTo>urn:uuid:b7681987-20f4-4fb5-9a74-383142da799c</a:RelatesTo>\n")
                   _T("  </s:Header>\n")
                   _T("  <s:Body>\n")
                   _T("    <ResultReference xmlns=\"http://schemas.marlin.org/general/services/2015\">\n")
                   _T("      <EventId>0</EventId>\n")
                   _T("      <Reference   xmlns:b=\"http://schemas.marlin.org/general/elementen/2015\"\n")
                   _T("                   xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\">\n")
                   _T("        <b:Repository>DocContainer</b:Repository>\n")
                   _T("        <b:ApplicationReference>K2B:30008</b:ApplicationReference>\n")
                   _T("        <b:DocumentType i:nil=\"true\"/>\n")
                   _T("        <b:CaseTypeCode>Key2Brief</b:CaseTypeCode>\n")
                   _T("        <b:DcIdentifier/>\n")
                   _T("        <b:FolderIdentifier>FLD-2015-001470</b:FolderIdentifier>\n")
                   _T("        <b:DocumentId i:nil=\"true\"/>\n")
                   _T("        <b:DossierId>2015-001470</b:DossierId>\n")
                   _T("        <b:FileName i:nil=\"true\"/>\n")
                   _T("        <b:RecordId i:nil=\"true\"/>\n")
                   _T("        <b:CaseId/>\n")
                   _T("      </Reference>\n")
                   _T("      <ResultCode>Ok</ResultCode>\n")
                   _T("      <ResultValue>Folder is created the DocContainer</ResultValue>\n")
                   _T("    </ResultReference>\n")
                   _T("  </s:Body>\n")
                   _T("</s:Envelope>"));

      SOAPMessage msg(soap);
      if(msg.GetErrorState())
      {
        PrintSoapFault(msg);
      }
      else
      {
        XString action = msg.GetSoapAction();
        XString resCode = msg.GetParameter(_T("ResultCode"));
        XString folder;

        Logger::WriteMessage(_T("Soap action: ") + action);
        Logger::WriteMessage(_T("Result code: ") + resCode);

        XMLElement* refer = msg.FindElement(NULL,_T("Reference"),true);
        if(refer)
        {
          XMLElement* ident = msg.FindElement(refer,_T("FolderIdentifier"));
          if(ident)
          {
            folder = ident->GetValue();
            Logger::WriteMessage(_T("Folder: %s\n") + folder);
          }
        }
        if(action  == _T("IContainer/CreateContainerResponse") &&
           resCode == _T("Ok") &&
           folder  == _T("FLD-2015-001470"))
        {
          result = true;
        }
      }
      Assert::AreEqual(true,result);
    }

    TEST_METHOD(TestSOAP12Full_request)
    {
      Logger::WriteMessage("Testing SOAP 1.2 with Full WS-Addressing and identity (request only)");

      bool result = false;
      XString soap(_T("<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:a=\"http://www.w3.org/2005/08/addressing\">\n")
                   _T("  <s:Header>\n")
                   _T("    <a:Action s:mustUnderstand=\"1\">http://schemas.marlin.org/general/services/2012/IContent/GetContent</a:Action>\n")
                   _T("    <h:Command xmlns:h=\"http://schemas.marlin.org/general/services/2012\"\n")
                   _T("                 xmlns=\"http://schemas.marlin.org/general/services/2012\">Find</h:Command>\n")
                   _T("    <h:Identity xsi:type=\"q1:LogOnData\" xmlns:h=\"http://schemas.marlin.org/general/services/2012\"\n")
                   _T("                xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n")
                   _T("                xmlns=\"http://schemas.marlin.org/general/services/2012\"\n")
                   _T("                xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n")
                   _T("                xmlns:q1=\"http://schemas.marlin.org/general/security/2012\">\n")
                   _T("      <q1:Context xsi:nil=\"true\"/>\n")
                   _T("      <q1:Domain>its</q1:Domain>\n")
                   _T("      <q1:LogOnType>Anonymous</q1:LogOnType>\n")
                   _T("      <q1:Password/>\n")
                   _T("      <q1:UserName>ehuisman</q1:UserName>\n")
                   _T("    </h:Identity>\n")
                   _T("    <a:MessageID>urn:uuid:82aca07f-88a8-4e05-be69-2c897e6b93bb</a:MessageID>\n")
                   _T("    <a:ReplyTo>\n")
                   _T("      <a:Address>http://www.w3.org/2005/08/addressing/anonymous</a:Address>\n")
                   _T("    </a:ReplyTo>\n")
                   _T("    <a:To s:mustUnderstand=\"1\">http://localhost/general/DocumentManager.svc</a:To>\n")
                   _T("  </s:Header>\n")
                   _T("  <s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\n")
                   _T("    <ContentCommandRequest xmlns=\"http://schemas.marlin.org/general/services/2012\">\n")
                   _T("      <Reference>\n")
                   _T("        <Repository xsi:nil=\"true\" xmlns=\"http://schemas.marlin.org/general/elementen/2012\"/>\n")
                   _T("        <ApplicationCode xsi:nil=\"true\" xmlns=\"http://schemas.marlin.org/general/elementen/2012\"/>\n")
                   _T("        <ApplicationReference xsi:nil=\"true\" xmlns=\"http://schemas.marlin.org/general/elementen/2012\"/>\n")
                   _T("        <DocumentType xsi:nil=\"true\" xmlns=\"http://schemas.marlin.org/general/elementen/2012\"/>\n")
                   _T("        <DcIdentifier xsi:nil=\"true\" xmlns=\"http://schemas.marlin.org/general/elementen/2012\"/>\n")
                   _T("        <FolderIdentifier xsi:nil=\"true\" xmlns=\"http://schemas.marlin.org/general/elementen/2012\"/>\n")
                   _T("        <DocumentId xmlns=\"http://schemas.marlin.org/general/elementen/2012\">12005679</DocumentId>\n")
                   _T("        <DossierId xsi:nil=\"true\" xmlns=\"http://schemas.marlin.org/general/elementen/2012\"/>\n")
                   _T("        <FileName xsi:nil=\"true\" xmlns=\"http://schemas.marlin.org/general/elementen/2012\"/>\n")
                   _T("        <RecordId xsi:nil=\"true\" xmlns=\"http://schemas.marlin.org/general/elementen/2012\"/>\n")
                   _T("      </Reference>\n")
                   _T("  </ContentCommandRequest>\n")
                   _T("  </s:Body>\n")
                   _T("</s:Envelope>\n"));

      SOAPMessage msg(soap);
      if(msg.GetErrorState())
      {
        Assert::Fail();
      }
      else
      {
        XString documentID;
        XString    action = msg.GetSoapAction();
        XMLElement* docid = msg.FindElement("DocumentId",true);
        if(docid)
        {
          documentID = docid->GetValue();
        }

        result = (action == _T("IContent/GetContent")) &&
                 (documentID == _T("12005679"));
      }
      Assert::AreEqual(true,result);
    }

    TEST_METHOD(TestSOAP12Full_response)
    {
      Logger::WriteMessage("Testing SOAP 1.2 with Full WS-Addressing and identity (response only)");

      bool result = false;
      XString soap(_T("<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:a=\"http://www.w3.org/2005/08/addressing\">\n")
                   _T("  <s:Header>\n")
                   _T("    <a:Action s:mustUnderstand=\"1\">http://schemas.marlin.org/general/services/2012/IContent/GetContentResponse</a:Action>\n")
                   _T("    <a:RelatesTo>urn:uuid:82aca07f-88a8-4e05-be69-2c897e6b93bb</a:RelatesTo>\n")
                   _T("  </s:Header>\n")
                   _T("  <s:Body>\n")
                   _T("    <ContentResponse xmlns=\"http://schemas.marlin.org/general/services/2012\">\n")
                   _T("      <Content xmlns:b=\"http://schemas.marlin.org/general/elementen/2012\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\">\n")
                   _T("        <b:DocumentId i:nil=\"true\"/>\n")
                   _T("        <b:RecordId i:nil=\"true\"/>\n")
                   _T("        <b:dcIdentifier i:nil=\"true\"/>\n")
                   _T("      </Content>\n")
                   _T("      <ContentReference xmlns:b=\"http://schemas.marlin.org/general/elementen/2012\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\">\n")
                   _T("        <b:DocumentType>Nota</b:DocumentType>\n")
                   _T("        <b:CasetypeCode i:nil=\"true\"/>\n")
                   _T("        <b:DcIdentifier>2015-001442/HalloWorld.txt</b:DcIdentifier>\n")
                   _T("      </ContentReference>\n")
                   _T("      <Result>Ok</Result>\n")
                   _T("      <ResultMessage>Found : General.Elements.DcmiReference</ResultMessage>\n")
                   _T("    </ContentResponse>\n")
                   _T("  </s:Body>\n")
                   _T("</s:Envelope>\n"));

      SOAPMessage msg;
      msg.ParseMessage(soap);
      if(msg.GetInternalError() != XmlError::XE_NoError)
      {
        Assert::Fail();
      }
      else
      {
        XString action = msg.GetSoapAction();
        result = (action == _T("IContent/GetContentResponse"));
      }
      Assert::AreEqual(true,result);
    }

    TEST_METHOD(TestSOAPXML_Namespaces)
    {
      Logger::WriteMessage("Testing namespace functions for SOAP/XML");

      int errors = 0;
      XString left (_T("http://Name.Test.lower\\something"));
      XString right(_T("https://NAME.test.LOWER/SomeThing/"));

      int result = CompareNamespaces(left,right);
      Assert::AreEqual(0,result);

      Testsplit(_T("\"http://server/uri/command/\""),_T("http://server/uri"));
      Testsplit(_T("http://server/uri/some#command"),_T("http://server/uri/some"));
      Testsplit(_T("command"),_T(""));
    }

    TEST_METHOD(TestNamespacesInSOAP11)
    {
      Logger::WriteMessage("Testing namespaces in SOAP 1.1");

      double num(77.88);
      XString namesp(_T("http://www.myname.org/interface/"));
      XString action(_T("FunctionCall"));
      SOAPMessage msg(namesp,action,SoapVersion::SOAP_11);
      msg.SetParameter(_T("ParameterOne"),_T("One"));
      msg.SetParameter(_T("ParameterTwo"),_T("Two"));
      msg.SetParameter(_T("Number"),num);

      XString message = msg.GetSoapMessage();
      XString expected = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
                         _T("<s:Envelope xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\n")
                         _T("  <s:Header />\n")
                         _T("  <s:Body>\n")
                         _T("    <FunctionCall xmlns=\"http://www.myname.org/interface/\">\n")
                         _T("      <ParameterOne>One</ParameterOne>\n")
                         _T("      <ParameterTwo>Two</ParameterTwo>\n")
                         _T("      <Number>77.88</Number>\n")
                         _T("    </FunctionCall>\n")
                         _T("  </s:Body>\n")
                         _T("</s:Envelope>\n");

      Assert::AreEqual(expected.GetString(),message.GetString());
    }

    TEST_METHOD(TestNamespacesInSOAP12)
    {
      Logger::WriteMessage("Testing namespaces in SOAP 1.2");

      double num(77.88);
      XString namesp(_T("http://www.myname.org/interface/"));
      XString action(_T("FunctionCall"));
      SOAPMessage msg(namesp,action);
      msg.SetParameter(_T("ParameterOne"),_T("One"));
      msg.SetParameter(_T("ParameterTwo"),_T("Two"));
      msg.SetParameter(_T("Number"),num);

      XString message = msg.GetSoapMessage();
      XString expected = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
                         _T("<s:Envelope xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:a=\"http://www.w3.org/2005/08/addressing\" xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\n")
                         _T("  <s:Header>\n")
                         _T("    <a:Action s:mustUnderstand=\"true\">http://www.myname.org/interface/FunctionCall</a:Action>\n")
                         _T("  </s:Header>\n")
                         _T("  <s:Body>\n")
                         _T("    <FunctionCall xmlns=\"http://www.myname.org/interface/\">\n")
                         _T("      <ParameterOne>One</ParameterOne>\n")
                         _T("      <ParameterTwo>Two</ParameterTwo>\n")
                         _T("      <Number>77.88</Number>\n")
                         _T("    </FunctionCall>\n")
                         _T("  </s:Body>\n")
                         _T("</s:Envelope>\n");
      Assert::AreEqual(expected.GetString(),message.GetString());
    }

    TEST_METHOD(TestNamespacesInSOAP_Parsed)
    {
      Logger::WriteMessage("Test parsing of namespaces in SOAP");

      int errors = 0;
      XString internal = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
                         _T("<s:Envelope xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:a=\"http://www.w3.org/2005/08/addressing\" xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\n")
                         _T("  <s:Header>\n")
                         _T("    <a:Action s:mustUnderstand=\"true\">http://www.myname.org/interface/FunctionCall</a:Action>\n")
                         _T("  </s:Header>\n")
                         _T("  <s:Body>\n")
                         _T("    <FunctionCall xmlns=\"http://www.myname.org/interface/\">\n")
                         _T("      <ParameterOne>One</ParameterOne>\n")
                         _T("      <ParameterTwo>Two</ParameterTwo>\n")
                         _T("      <Number>77.88</Number>\n")
                         _T("    </FunctionCall>\n")
                         _T("  </s:Body>\n")
                         _T("</s:Envelope>\n");
      SOAPMessage msg(internal,false);
      XString action = msg.GetSoapAction();
      XString namesp = msg.GetNamespace();

      errors += action.Compare(_T("FunctionCall")) != 0;
      errors += namesp.Compare(_T("http://www.myname.org/interface/")) != 0;

      Assert::AreEqual(0,errors);
    }

    TEST_METHOD(TestNamespacesInSOAP_Reparsed)
    {
      Logger::WriteMessage("Test namespaces in SOAP when re-parsing the <Body>");

      int errors = 0;
      double num(77.88);
      XString namesp(_T("http://www.myname.org/interface/"));
      XString action(_T("FunctionCall"));
      SOAPMessage msg(namesp, action);
      msg.SetParameter(_T("ParameterOne"), _T("One"));
      msg.SetParameter(_T("ParameterTwo"), _T("Two"));
      msg.SetParameter(_T("Number"), num);
      msg.CompleteTheMessage();

      XString newmsg = _T("<MyFunction>\n")
                       _T("   <One>1</One>\n")
                       _T("   <Two>2</Two>\n")
                       _T("</MyFunction>");
      msg.ParseAsBody(newmsg);
      msg.SetNamespace(_T("http://www.other.au/api/"));
      XString newaction(_T("MyTestFunction"));
      msg.SetSoapAction(newaction);
      msg.SetEncoding(Encoding::UTF8);
      XString message = msg.GetSoapMessage();

      XString expected = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
                         _T("<s:Envelope xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:a=\"http://www.w3.org/2005/08/addressing\" xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\n")
                         _T("  <s:Header>\n")
                         _T("    <a:Action s:mustUnderstand=\"true\">http://www.other.au/api/MyTestFunction</a:Action>\n")
                         _T("  </s:Header>\n")
                         _T("  <s:Body>\n")
                         _T("    <MyFunction xmlns=\"http://www.other.au/api/\">\n")
                         _T("      <One>1</One>\n")
                         _T("      <Two>2</Two>\n")
                         _T("    </MyFunction>\n")
                         _T("  </s:Body>\n")
                         _T("</s:Envelope>\n");
      errors = expected.Compare(message) != 0;

      Assert::AreEqual(0,errors);
    }

    TEST_METHOD(TestSOAPHeaders)
    {
      Logger::WriteMessage("Testing SOAPAction in Content-Type for SOAP 1.2");

      int errors = 0;

      XString namesp(DEFAULT_NAMESPACE);
      XString action(_T("FirstAction"));
      SOAPMessage msg(namesp,action,SoapVersion::SOAP_12);

      msg.AddHeader(_T("Content-Type"),_T("application/soap+xml; charset=UTF-8; action=\"FirstAction\""));

      XString charset = FindCharsetInContentType(msg.GetHeader(_T("content-type")));
      XString checkac = FindFieldInHTTPHeader   (msg.GetHeader(_T("content-type")),_T("Action"));

      errors += charset.CompareNoCase(_T("utf-8")) != 0;
      errors += checkac.Compare(_T("FirstAction")) != 0;

      // Test empty
      XString test1 = FindFieldInHTTPHeader(msg.GetHeader(_T("DoNotExist")),_T("ABC"));
      errors += !test1.IsEmpty();

      // HTTP Headers
      XString header(_T("application/soap+xml; charset=UTF-8; action=\"FirstAction\""));
      XString newheader1 = SetFieldInHTTPHeader(header,_T("action"),_T("http://test.marlin.org/Function"));
      XString newheader2 = SetFieldInHTTPHeader(header,_T("charset"),_T("UTF16"));
      XString newheader3 = SetFieldInHTTPHeader(_T("first"),_T("field"),_T("The Value"));
      XString newheader4 = SetFieldInHTTPHeader(_T("first"),_T("field"),_T("something"));
      errors += newheader1.Compare(_T("application/soap+xml; charset=UTF-8; action=\"http://test.marlin.org/Function\"")) != 0;
      errors += newheader2.Compare(_T("application/soap+xml; charset=UTF16; action=\"FirstAction\"")) != 0;
      errors += newheader3.Compare(_T("first; field=\"The Value\"")) != 0;
      errors += newheader4.Compare(_T("first; field=something")) != 0;


      Assert::AreEqual(0,errors);
    }

    TEST_METHOD(TestServiceQuality)
    {
      Logger::WriteMessage("Testing ServiceqQuality in accept header");

      int errors = 0;

      XString header = _T("text/*;q=0.3, text/html;q=0.7, text/html;level=1, text/html;level=2;q=0.4, */*;q=0.5");
      ServiceQuality serv(header);

      // Check first option
      QualityOption* opt = serv.GetOptionByPreference(0);
      if(opt)
      {
        errors += opt->m_field    .CompareNoCase(_T("text/html")) != 0;
        errors += opt->m_extension.CompareNoCase(_T("level")) != 0;
        errors += opt->m_value    .CompareNoCase(_T("1")) != 0;
      }
      else ++errors;

      // Check find by name
      int pref = serv.GetPreferenceByName(_T("*/*"));
      errors += pref != 50;

      XString second = serv.GetStringByPreference(1);
      errors += second.Compare(_T("text/html"));

      Assert::AreEqual(0,errors);
    }
  };
}
