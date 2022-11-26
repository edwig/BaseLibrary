#include "pch.h"
#include "CppUnitTest.h"
#include <XStringBuilder.h>
#include <HPFCounter.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define NUM_TESTS 100000  // 1E+05

namespace BaseLibraryUnitTests
{
TEST_CLASS(XStringBuilderTests)
{
public:
  TEST_METHOD(TestXStringBuilderPerformance)
  {
    Logger::WriteMessage("Testing XString Builder Performance.");

    XString total;
    XString newstring("This is a new string added to the rest\n");

    HPFCounter count;
    for(int x = 1;x < NUM_TESTS; ++x)
    {
      total += newstring;
      CString ext = newstring + newstring;
      total += ext;
    }
    size_t size = total.GetLength();
    count.Stop();

    XString msg;
    msg.Format("Adding all XStrings took: %8.6f ms",count.GetCounter());
    Logger::WriteMessage(msg);

    XStringBuilder build;

    HPFCounter count2;
    for(int x = 1;x < NUM_TESTS; ++x)
    {
      build.Append(newstring);
      CString ext = newstring + newstring;
      total += ext;
    }
    total = build.ToString();
    size = total.GetLength();
    count2.Stop();

    msg.Format("XStringBuilder took: %8.6f ms",count2.GetCounter());
    Logger::WriteMessage(msg);
  }
};
}