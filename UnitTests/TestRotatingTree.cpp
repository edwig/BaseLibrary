/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: TestRotatingTree.cpp
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
#include <RotatingTree.h>
#include <stdlib.h>
#include <map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define STORING      100
#define NUM_TESTS 100000  // 1E+05
#define NUM_KEEP  (NUM_TESTS / STORING)

namespace BaseLibraryUnitTests
{

int RTCompareNumbers(const RTKEY p_left,const RTKEY p_right)
{
  return (int) ((INT64)p_left - (INT64)p_right);
}

typedef struct _keep
{
  INT64    index;
  unsigned value;
}
KEEP;

TEST_CLASS(RotatingTreeTests)
{
public:
  TEST_METHOD(TestInsertAndFind)
  {
    Logger::WriteMessage("Testing RotatingTree");

    // Tree to test
    RotatingTree tree(RTCompareNumbers);
    KEEP tokeep[NUM_KEEP];
    int  keeping = 0;

    // Init the random number generator
    srand(clock());

    // Insert into a large tree
    for(INT64 i = 0;i < NUM_TESTS;++i)
    {
      unsigned value = (unsigned) rand();
      tree.Insert((RTKEY)i,(PAYLOAD)(INT64)value);

      // Store 1:STORING test values
      if((i % STORING) == 0)
      {
        tokeep[keeping].index = i;
        tokeep[keeping].value = value;
        ++keeping;
      }
    }

    // Check that we find the right values dat we stored
    for(int i = 0;i < NUM_KEEP;++i)
    {
      INT64 val = (INT64) tree.Find((RTKEY)(INT64) tokeep[i].index);
      Assert::IsTrue((val - tokeep[i].value) == 0);
    }

    // Check the count
    Assert::AreEqual((int)tree.Size(),(int)NUM_TESTS);

    // Clean the tree
    tree.RemoveAll();
    Assert::AreEqual((int)tree.Size(),0);
  }

  TEST_METHOD(STLMapInsertAndFind)
  {
    Logger::WriteMessage("Testing RotatingTree");

    // Tree to test
    std::map<RTKEY,PAYLOAD> tree;
    KEEP tokeep[NUM_KEEP];
    int  keeping = 0;

    // Init the random number generator
    srand(clock());

    // Insert into a large tree
    for(INT64 i = 0;i < NUM_TESTS;++i)
    {
      unsigned value = (unsigned)rand();
      tree[(RTKEY)i] = (PAYLOAD)(INT64)value;

      // Store 1:STORING test values
      if((i % STORING) == 0)
      {
        tokeep[keeping].index = i;
        tokeep[keeping].value = value;
        ++keeping;
      }
    }

    // Check that we find the right values dat we stored
    for(int i = 0;i < NUM_KEEP;++i)
    {
      INT64 val = (INT64)tree[(RTKEY)(INT64)tokeep[i].index];
      Assert::IsTrue((val - tokeep[i].value) == 0);
    }

    // Check the count
    Assert::AreEqual((int)tree.size(),(int)NUM_TESTS);

    // Clean the tree
    tree.clear();
    Assert::AreEqual((int)tree.size(),0);
  }
};

}
