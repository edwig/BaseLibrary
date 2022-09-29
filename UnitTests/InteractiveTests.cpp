#include "pch.h"
#include "CppUnitTest.h"
#include <WideMessageBox.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
TEST_CLASS(InteractiveTests)
{
public:

  TEST_METHOD(TestWideMessageBox)
  {
    Logger::WriteMessage("Testing WideMessageBox");

    int res = WideMessageBox(NULL,"This is a test with a very wide message box surpassing the standard MS-Window width","WIDE",MB_OK|MB_SETFOREGROUND|MB_ICONASTERISK);

    Assert::AreEqual(res,IDOK);
  }
};
}
