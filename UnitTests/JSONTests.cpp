#include "pch.h"
#include "CppUnitTest.h"

#include <JSONMessage.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
TEST_CLASS(JSONMessageTests)
{
public:

  TEST_METHOD(TestJSONSortingArray)
  {
    Logger::WriteMessage("Testing JSONMessage array sorting");

    XString input("{\n"
                  "\t\"myarray\":[\n"
                  "\t\"zz\",\n"
                  "\t\"cc\",\n"
                  "\t\"bb\",\n"
                  "\t\"aa\"\n"
                  "\t]\n"
                  "}");
    JSONMessage json;
    json.ParseMessage(input);

    json.SetWhitespace(true); // to be sure
    XString output = json.GetJsonMessage();

    Assert::IsTrue(output.Compare(input) == 0);
  }

  TEST_METHOD(TestJSONSortingObject)
  {
    Logger::WriteMessage("Testing JSONMessage object sorting");

    XString input("{\n"
                  "\t\"myobject\":\"my\",\n"
                  "\t\"aa\":11,\n"
                  "\t\"zz\":99\n"
                  "}");
    JSONMessage json;
    json.ParseMessage(input);

    json.SetWhitespace(true); // to be sure
    XString output = json.GetJsonMessage();

    Assert::IsTrue(output.Compare(input) == 0);
  }
};
}