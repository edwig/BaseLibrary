#include "pch.h"
#include "CppUnitTest.h"
#include <SchemaReWriter.h>
#include <map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{

static FCodes g_codes;


void FillCodes()
{
  g_codes["NVL"]     = "ISNULL";
  g_codes["UPPER"]   = "UCASE";
  g_codes["LOWER"]   = "LCASE";
  g_codes["SYSDATE"] = "CURRENT_TIMESTAMP";
}


TEST_CLASS(SchemaRewriterTests)
{
public:

  TEST_METHOD(RewriterTestAddSchema)
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

  TEST_METHOD(RewriterTestAddSchemaCondition)
  {
    Logger::WriteMessage("Testing Schema Rewriter add schema in a condition");

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

  TEST_METHOD(RewriterTestComments)
  {
    Logger::WriteMessage("Testing Schema Rewriter add schema with comments");

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

  TEST_METHOD(RewriterTestMetaCharacters)
  {
    Logger::WriteMessage("Testing Schema Rewriter with meta characters in table name");

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

  TEST_METHOD(RewriterTestStringConcatenation)
  {
    Logger::WriteMessage("Testing Schema Rewriter strings concatenation");

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

  TEST_METHOD(RewriterTestFunctionCodes)
  {
    Logger::WriteMessage("Testing Schema Rewriter code words conversion");

    FillCodes();
    SchemaReWriter re("other");
    re.SetRewriteCode(&g_codes);

    XString input = "SELECT lower(nvl(one,'')) FROM dual;";
    XString output = re.Parse(input);
    XString expect = "SELECT LCASE(ISNULL(one,'')) FROM other.dual;";

    // Compare MUST be zero
    Assert::IsFalse(expect.Compare(output));
  }

  TEST_METHOD(RewriterTestEndOfStatement)
  {
    Logger::WriteMessage("Testing Schema Rewriter end-of-statement without delimeter");

    FillCodes();
    SchemaReWriter re("other");
    re.SetRewriteCode(&g_codes);

    XString input = "SELECT sysdate FROM dual";
    XString output = re.Parse(input);
    XString expect = "SELECT CURRENT_TIMESTAMP FROM other.dual";

    // Compare MUST be zero
    Assert::IsFalse(expect.Compare(output));
  }
};
}
