/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: TestRedirect.cpp
//
// Marlin Component: Internet server/client
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
#include <RunRedirect.h>

XString CreateMessage()
{
  XString msg;

  XString line   = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyzx0123456789\n");
  XString stripe = _T("-------------------------------------------------------------------------\n");

  msg = stripe;
  for(int index = 0;index < 100; ++index)
  {
    msg += line;
  }
  // five times
  msg = msg + line + msg + line + msg + line + msg + line + msg;
  // end marker
  msg += _T("*** END ***\n");

  return msg;
}

bool  TestOnce(LPCTSTR p_param)
{
  XString message = CreateMessage();
  int lengthbefore = message.GetLength();

  // TO PROPERLY TEST THIS!!
  // UNCOMMENT THE Sleep(1) statement in the following method:
  // Redirect::StdOutThread(HANDLE hStdOutRead)

  XString result;
  XString error;
  int waittime = 25000;
  int res = CallProgram_For_String(_T("TestFilter"),p_param,message,result,error,waittime);

  result.Remove('\r');
  error.Remove('\r');
  int lengthafter = result.GetLength() + error.GetLength();

  lengthbefore *= 2;
  if(lengthafter != lengthbefore)
  {
    _tprintf(_T("We did NOT read the entire output!\n"));
    return false;
  }
  // Check that the return value came through
  return (res == 1234);
}

int main(const int argc,const char* argv[])
{
  int errors     = 0;
  int testamount = 10;

  // Different number of tests specified?
  if(argc >= 2)
  {
    int amount = atoi(argv[1]);
    if(amount >= 1)
    {
      testamount = amount;
    }
  }

  // Test with filter to 'stdout'
  for(int index = 0;index < testamount; ++index)
  {
    _tprintf(_T("Testing stdout number: %d\n"),index);
    if(!TestOnce(_T("")))
    {
      ++errors;
    }
  }

  // Test with filter to 'stderr'
  for(int index = 0;index < testamount; ++index)
  {
    _tprintf(_T("Testing stderr number: %d\n"),index);
    if(!TestOnce(_T("to-error")))
    {
      ++errors;
    }
  }

  _tprintf(_T("Total number of errors: %d\n"),errors);
  return errors;
}
