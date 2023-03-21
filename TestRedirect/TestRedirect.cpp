/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: TestRedirect.cpp
//
// Marlin Component: Internet server/client
// 
// Copyright (c) 2014-2023 ir. W.E. Huisman
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
#include <RunRedirect.h>

CString CreateMessage()
{
  CString msg;

  CString line   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyzx0123456789\n";
  CString stripe = "-------------------------------------------------------------------------\n";

  msg = stripe;
  for(int index = 0;index < 100; ++index)
  {
    msg += line;
  }
  // five times
  msg = msg + line + msg + line + msg + line + + msg + line + msg;
  // end marker
  msg += "*** END ***\n";

  return msg;
}

bool  TestOnce()
{
  XString message = CreateMessage();
  int lengthbefore = message.GetLength();

  // TO PROPERLY TEST THIS!!
  // UNCOMMENT THE Sleep(1) statement in the following method:
  // Redirect::StdOutThread(HANDLE hStdOutRead)

  XString result;
  int waittime = 25000;
  int res = CallProgram_For_String("TestFilter","noparam",message.GetString(),result,waittime);

  result.Remove('\r');
  int lengthafter = result.GetLength();

  lengthbefore *= 2;
  if(lengthafter != lengthbefore)
  {
    printf("We did NOT read the entire output!\n");
    return false;
  }
  return true;
}

int main()
{
  int errors = 0;

  for(int index = 0;index < 10000; ++index)
  {
    printf("Testing number: %d\n",index);
    if(!TestOnce())
    {
      ++errors;
    }
  }
  printf("Total number of errors: %d\n",errors);
  return errors;
}

