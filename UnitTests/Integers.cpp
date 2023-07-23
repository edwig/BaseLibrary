#include "pch.h"
#include "CppUnitTest.h"
#include <safeint.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace msl::utilities;

namespace BaseLibraryUnitTests
{
TEST_CLASS(IntegerTests)
{
public:

  TEST_METHOD(IntegerSafty1)
  {
    Logger::WriteMessage("Integer safty overflow");

    // No error bells!
    int num1 = 0x7FFFFFFE;
    int num2 = num1 + 2;

    Assert::IsTrue(num2 < 0);

    // Cannot work!
    SafeInt<int> number1 { 0x7FFFFFFE };
    SafeInt<int> number2 { 2 };

    try
    {
      number2 = number1 + 2;
    }
    catch(SafeIntException& ex)
    {
      CString message;
      message.Format("Safe int error code: %d",ex.m_code);
      Logger::WriteMessage(message);
      message.Format("Number 2 still is: %d",*number2.Ptr());
      Logger::WriteMessage(message);

      Assert::IsTrue(ex.m_code == 1);
      Assert::IsTrue(number2   == 2);
      return;
    }
    // Should not come to here
    Assert::IsTrue(false);
  }

};

}