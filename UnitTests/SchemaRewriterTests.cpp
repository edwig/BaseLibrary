#include "pch.h"
#include "CppUnitTest.h"
#include <SchemaReWriter.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{
TEST_CLASS(SchemaRewriterTests)
{
public:

  TEST_METHOD(RewriterTest1)
  {
    Logger::WriteMessage("Testing Schema Rewriter add schema");

    SchemaReWriter re("other");

    XString input = "SELECT t.one\n"
                    "      ,t.two\n"
                    "  FROM table t\n"
                    " WHERE t.three = 3";
    XString output = re.Parse(input);

    XString expect = "SELECT t.one\n"
                     "      ,t.two\n"
                     "  FROM other.table t\n"
                     " WHERE t.three = 3";

    // Compare MUST be zero
    Assert::IsFalse(expect.Compare(output));
  }

  TEST_METHOD(RewriterTest2)
  {
    Logger::WriteMessage("Testing Schema Rewriter add schema");

    SchemaReWriter re("other");

    XString input = "SELECT one\n"
                    "      ,two\n"
                    "  FROM table\n"
                    " WHERE table.three = 3";
    XString output = re.Parse(input);

    XString expect = "SELECT one\n"
                     "      ,two\n"
                     "  FROM other.table\n"
                     " WHERE table.three = 3";

    // Compare MUST be zero
    Assert::IsFalse(expect.Compare(output));
  }

  TEST_METHOD(RewriterTest3)
  {
    Logger::WriteMessage("Testing Schema Rewriter add schema");

    SchemaReWriter re("other");

    XString input = "SELECT one\n"
                    "      ,two\n"
                    "      ,'Strings are allowed'\n"
                    "       -- Added comment\n"
                    "  FROM \"table\"\n"
                    " WHERE table.three = 3";
    XString output = re.Parse(input);

    XString expect = "SELECT one\n"
                     "      ,two\n"
                     "      ,'Strings are allowed'\n"
                     "       -- Added comment\n"
                     "  FROM other.\"table\"\n"
                     " WHERE table.three = 3";

    // Compare MUST be zero
    Assert::IsFalse(expect.Compare(output));
  }

  TEST_METHOD(RewriterTest4)
  {
    Logger::WriteMessage("Testing Schema Rewriter concat test");

    SchemaReWriter re("other");

    XString input = "SELECT one || two\n"
                    "      ,'Strings are allowed'\n"
                    "       -- Added comment\n"
                    "  FROM \"table\"\n"
                    " WHERE table.three = 3";
    XString output = re.Parse(input);

    XString expect = "SELECT one || two\n"
                     "      ,'Strings are allowed'\n"
                     "       -- Added comment\n"
                     "  FROM other.\"table\"\n"
                     " WHERE table.three = 3";

    // Compare MUST be zero
    Assert::IsFalse(expect.Compare(output));
  }

  TEST_METHOD(RewriterTest5)
  {
    Logger::WriteMessage("Testing Schema Rewriter concat to add");

    SchemaReWriter re("other");
    re.SetOption(SROption::SRO_CONCAT_TO_ADD);

    XString input = "SELECT one || two\n"
                    "      ,'Strings are allowed'\n"
                    "       -- Added comment\n"
                    "  FROM \"table\"\n"
                    " WHERE table.three = 3";
    XString output = re.Parse(input);

    XString expect = "SELECT one + two\n"
                     "      ,'Strings are allowed'\n"
                     "       -- Added comment\n"
                     "  FROM other.\"table\"\n"
                     " WHERE table.three = 3";

    // Compare MUST be zero
    Assert::IsFalse(expect.Compare(output));
  }
};
}