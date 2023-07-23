/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: TestFilter.cpp
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
//////////////////////////////////////////////////////////////////////////
//
// TEST PROGRAM THAT DUPLICATES THE STDNARD INPUT
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define BUFFERSIZE     1
#define MEGABYTE       (1024 * 1024)
#define OUTPUT_TIMES   2

char* buffer = nullptr;

void AllocateBuffer()
{
  buffer = new char[BUFFERSIZE * MEGABYTE];
}

void DeallocateBuffer()
{
  delete[] buffer;
  buffer = nullptr;
}


void ReadStandardInput()
{
  char* bufpointer = buffer;
  long  amount = 0;

  while(true)
  {
    int ch = fgetc(stdin);
    if(ch == EOF)
    {
      break;
    }
    *bufpointer++ = (char)ch;
    if(++amount == (BUFFERSIZE * MEGABYTE))
    {
      break;
    }
  }
  // Delimit the input buffer
  *bufpointer = 0;
}

void WriteStandardOutput(bool p_asError)
{
  for(int index = 0; index < OUTPUT_TIMES; ++index)
  {
    char* bufpointer = buffer;
    while(*bufpointer)
    {
      fputc(*bufpointer++,p_asError ? stderr : stdout);
    }
  }
}

int main(const int argc,const char* argv[])
{
  bool sendToError = false;

  if(argc >= 2)
  {
    if(_stricmp(argv[1],"to-error") == 0)
    {
      sendToError = true;
    }
  }

  AllocateBuffer();

  ReadStandardInput();
  WriteStandardOutput(sendToError);

  DeallocateBuffer();

  // Write a specific number
  // so we can recognize it in the debugger
  return 1234;
}

