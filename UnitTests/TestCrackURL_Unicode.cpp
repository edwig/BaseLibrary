/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: TestCrackURL_Unicode.cpp
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
#include <CrackURL.h>
#include <ConvertWideString.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
TEST_CLASS(CrackURL_Unicode)
{
private:
  void PrintBytes(void* p_buffer,int p_length)
  {
    Logger::WriteMessage(_T("TESTING BUFFER:"));
    Logger::WriteMessage(_T("---------------"));

    for(int ind = 0; ind < p_length; ++ind)
    {
      unsigned ch = (reinterpret_cast<_TUCHAR*>(p_buffer))[ind];
      TCHAR buffer[100];
      _stprintf_s(buffer,100,_T("  byte: %02X %c\n"),ch,ch);
      Logger::WriteMessage(buffer);
    }
  }

public:

  TEST_METHOD(TestUnicode)
  {
    Logger::WriteMessage("Testing Unicode in MBCS applications");
    Logger::WriteMessage("Testing Unicode conversions on diacritic chars");
    int errors = 1;

#ifdef _UNICODE
    // We ARE the Unicode mode !!
    errors = 0;
#else
    //const char *text = "Sôn bôn de magnà el véder, el me fa minga mal.";
    BYTE* buffer = NULL;
    const TCHAR* text = _T("áàäâéèëêíìïîóòöôúùüûýÿçñõã");
    int   length = 0;
    bool  doBom = true;

    PrintBytes(reinterpret_cast<void*>(const_cast<TCHAR*>(text)),static_cast<int>(_tcslen(text)));

    // Try to convert to Unicode UTF-16
    if(TryCreateWideString(text,_T(""),doBom,&buffer,length))
    {
      PrintBytes(buffer,length);

      XString result;
      bool  foundBOM = false;
      if(TryConvertWideString(buffer,length,_T(""),result,foundBOM))
      {
        PrintBytes(reinterpret_cast<void*>(const_cast<TCHAR*>(result.GetString())),result.GetLength());
      }
      // Do not forget to free the buffer
      delete[] buffer;

      if(result.Compare(text) == 0)
      {
        --errors;
      }
    }
#endif
    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(TestURLChars)
  {
    // Number of tests
    int errors = 6;

    // TEST -> "http://<MARLIN_HOST>:1234/Marlin Test all/URL?first=abc&second=áàäâéèëêíìïîóòöôúùüûýÿçñõã"
    XString mustBeUrl(_T("https://localhost:1234/Marlin%20Test%20all/URL?first=abc&second=%C3%A1%C3%A0%C3%A4%C3%A2%C3%A9%C3%A8%C3%AB%C3%AA%C3%AD%C3%AC%C3%AF%C3%AE%C3%B3%C3%B2%C3%B6%C3%B4%C3%BA%C3%B9%C3%BC%C3%BB%C3%BD%C3%BF%C3%A7%C3%B1%C3%B5%C3%A3"));
    CrackedURL url1(mustBeUrl);
    XString encoded = url1.URL();

    // Getting the second parameter
    XString diacrits = url1.GetParameter(_T("second"));
    XString expected = _T("áàäâéèëêíìïîóòöôúùüûýÿçñõã");

    // Test if success
    if(encoded == mustBeUrl)
    {
      --errors;
    }
    else
    {
      Logger::WriteMessage(XString(_T("Orig URL: ")) + mustBeUrl);
      Logger::WriteMessage(XString(_T("Code URL: ")) + encoded);
    }
    if(diacrits == expected)
    {
      --errors;
    }
    else
    {
      Logger::WriteMessage(XString(_T("Diacrits: ")) + diacrits);
      Logger::WriteMessage(XString(_T("Expected: ")) + expected);
    }
    if(url1.m_port == 1234)
    {
      --errors;
    }
    else
    {
      XString portFound;
      portFound.Format(_T("Port number found: %d"), url1.m_port);
      Logger::WriteMessage(portFound);
    }
    if(url1.m_host.Compare(_T("localhost")) == 0)
    {
      --errors;
    }
    if(url1.m_scheme.Compare(_T("https")) == 0)
    {
      --errors;
    }

    // Encoding of higher order UTF-8 like the Euro signs
    XString urlString = _T("http://localhost/Een%20Twee%20drie?een=waarde=%3Crequest%3E%3Cpage%3Eklantenkaart%3C%2Fpage%3E%3Crequesttype%3Econtactmemoaanmaken%3C%2Frequesttype%3E%3Ccallback%3EdesktopCallback%3C%2Fcallback%3E%3Cparameters%3E%3Cparameter%3E%3Cname%3Econtactmemogegevens%3C%2Fname%3E%3Cvalue%3E%7B%22contact_id%22:151714,%22eerste_contact_id%22:151714,%22werknemer_id%22:2942,%22werknemer_naam%22:%22N.N.+Bhoera%22,%22beheerorgnaam%22:%22Edwin%22,%22richting%22:%22uit%22,%22onderwerp%22:%22H%C3%AF%C3%ABr+st%C3%A4%C3%A4n+%E2%82%AC+test%22%7D%3C%2Fvalue%3E%3C%2Fparameter%3E%3C%2Fparameters%3E%3C%2Frequest%3E");
    CrackedURL url2(urlString);
    encoded = url2.URL();

    // Test if success
    if(urlString == encoded)
    {
      --errors;
    }
    else
    {
      Logger::WriteMessage(XString(_T("URL string: ")) + urlString);
      Logger::WriteMessage(XString(_T("URL decode: ")) + encoded);
    }
    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(CrackURLExtension)
  {
    int errors = 3;

    XString urlString1(_T("https://localhost/One/Two/tim.berners-lee/room/error404"));
    XString urlString2(_T("https://server/One/Two/tim.berners-lee/room/error404.html"));
    XString urlString3(_T("https://otherServer/One/Two/tim.berners-lee/room/error404.html?par1=cern&par2=geneve"));
    CrackedURL url1(urlString1);
    CrackedURL url2(urlString2);
    CrackedURL url3(urlString3);

    if(url1.GetExtension().IsEmpty())
    {
      --errors;
    }
    if(url2.GetExtension() == _T("html"))
    {
      --errors;
    }
    if(url3.GetExtension() == _T("html"))
    {
      --errors;
    }
    Assert::AreEqual(0,errors);
  }

  TEST_METHOD(CrackURLParametersUTF8)
  {
    Logger::WriteMessage("Testing full roundtrip parameter translation with diacritics");

    XString urlString1(_T("http://localhost:80/MarlinTest/Extra/MarlinSecond?Translation=Français"));
    XString urlString2(_T("http://localhost:80/MarlinTest/Extra/MarlinSecond?Translation=Fran%C3%A7ais"));
    
    CrackedURL url1(urlString1);
    CrackedURL url2(urlString2);

    XString language1 = url1.GetParameter(_T("Translation"));
    XString expected1 = _T("Français");

    Assert::AreEqual(expected1.GetString(),language1.GetString());

    XString url1Full = url1.URL();
    XString url2Full = url2.URL();

    XString language2 = url2.GetParameter(_T("Translation"));
    XString expected2 = _T("Français");

    Assert::AreEqual(expected2.GetString(),language2.GetString());

    Assert::AreEqual(url1Full.GetString(),url2Full.GetString());
  }
};
}