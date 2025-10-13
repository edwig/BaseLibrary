/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: TestWinFile.cpp
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define NUM_TESTS 100000  // 1E+05

namespace BaseLibraryUnitTests
{
TEST_CLASS(WinFileTests)
{
public:
  TEST_METHOD(TestWinFileUTF8)
  {
    Logger::WriteMessage("Testing WinFile UTF capabilities.");

    XString filename(_T("C:\\Tmp\\TestUTF_8.txt"));

    WinFile file(filename);
    file.CreateDirectory();
    file.SetEncoding(Encoding::UTF8);

    if(!file.Open(winfile_write | open_trans_text))
    {
      Assert::Fail(L"Failed to create UTF-8 text file!");
    }
    XString one(_T("This is an amount of € 10.00\n"));
    XString two(_T("The name is génèpëllêtje\n"));
    XString fin(_T("No final newline"));

    file.Write(one);
    file.Write(two);
    file.Write(fin);

    if(!file.Close())
    {
      Assert::Fail(L"File not closed!");
    }

    // Use new WinFile object, so no encoding known
    WinFile file2(filename);
    // Read the file back
    if(!file2.Open(winfile_read | open_trans_text))
    {
      Assert::Fail(L"Failed to open UTF-8 text file!");
    }
    XString string1,string2,string3;
    file2.Read(string1);
    file2.Read(string2);
    file2.Read(string3);

    if(one.Compare(string1) || two.Compare(string2) || fin.Compare(string3))
    {
      Assert::Fail(L"ERROR while reading back UTF-8 file!");
    }
    file2.Close();

    // Remove the file
    if(!file2.DeleteFile())
    {
      Assert::Fail(L"File not deleted!");
    }
  }

  TEST_METHOD(TestWinFileUTF16)
  {
    Logger::WriteMessage("Testing WinFile UTF capabilities.");

    XString filename(_T("C:\\Tmp\\TestUTF_16.txt"));

    WinFile file(filename);
    file.CreateDirectory();
    file.SetEncoding(Encoding::LE_UTF16);

    if(!file.Open(winfile_write | open_trans_text))
    {
      Assert::Fail(L"Failed to create UTF-16 text file!");
    }
    XString one(_T("This is an amount of € 10.00\n"));
    XString two(_T("The name is génèpëllêtje\n"));
#ifdef _UNICODE
    XString three(L"荷じば委集ヘモホア住75定ヱウコヨ保博芸よいんづ添具メツ質上たせばる模34連ユニ信方ロ知演レフサ幅性トヌソミ条上ヲコ政優裕ぶリば聞杉更打アチマロ井機スユ物彩ろぽ。面せんちや祭9賞巨ケノエ太謡そべぐ無波夫わゃぴ合督作ト疑芸リマネ載日作ヱシ乱夕のさルょ弁番を据記きべや降家リモメル絡敷詳木ラに。\n");
#endif
    XString fin(_T("No final newline"));

    file.Write(one);
    file.Write(two);
#ifdef _UNICODE
    file.Write(three);
#endif
    file.Write(fin);

    if(!file.Close())
    {
      Assert::Fail(L"File not closed!");
    }

    // Use new WinFile object, so no encoding known
    WinFile file2(filename);
    // Read the file back
    if(!file2.Open(winfile_read | open_trans_text))
    {
      Assert::Fail(L"Failed to open UTF-16 text file!");
    }
    XString string1,string2,string3,string4;
    file2.Read(string1);
    file2.Read(string2);
#ifdef _UNICODE
    file2.Read(string3);
#endif
    file2.Read(string4);

    if(one.Compare(string1) || two.Compare(string2) || fin.Compare(string4))
    {
      Assert::Fail(L"ERROR while reading back UTF-16 file!");
    }
#ifdef _UNICODE
    if(three.Compare(string3))
    {
      Assert::Fail(L"ERROR while reading back Japanese UTF-16 from the file file!");
    }
#endif
    file2.Close();

    // Remove the file
    if(!file2.DeleteFile())
    {
      Assert::Fail(L"File not deleted!");
    }
  }

};
}