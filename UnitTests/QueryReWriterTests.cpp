#include "pch.h"
#include "CppUnitTest.h"
#include <QueryReWriter.h>
#include <map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BaseLibraryUnitTests
{

static FCodes g_specials;
static FCodes g_codes;
static FCodes g_odbc;

void FillCodes()
{
  if(!g_codes.empty())
  {
    return;
  }

  g_codes["NVL"]     = "ISNULL";
  g_codes["UPPER"]   = "UCASE";
  g_codes["LOWER"]   = "LCASE";
  g_codes["SYSDATE"] = "CURRENT_TIMESTAMP";
}

void FillODBCEscapes()
{
  if(!g_odbc.empty())
  {
    return;
  }
  g_odbc["LOWER"]    = "LCASE";
  g_odbc["UPPER"]    = "UCASE";
  g_odbc["NVL"]      = "ISNULL";
  g_odbc["DATABASE"] = "DATABASE";

  for(auto& odbc : g_odbc)
  {
    XString trans = odbc.second;
    FCodes::iterator it = g_odbc.find(trans);
    if(it == g_odbc.end())
    {
      g_odbc[trans] = trans;
    }
  }
}

void FillSpecials()
{
  if(!g_specials.empty())
  {
    return;
  }
  g_specials["BRIEF_EW"] = "metaschema";
}

TEST_CLASS(QueryReWriterTests)
{
public:

  TEST_METHOD(RewriterTestAddSchema)
  {
    Logger::WriteMessage("Testing Schema Rewriter add schema");

    QueryReWriter re("other");

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

    QueryReWriter re("other");

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

    QueryReWriter re("other");

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

    QueryReWriter re("other");

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

    QueryReWriter re("other");
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
    QueryReWriter re("other");
    re.SetRewriteCodes(&g_codes);

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
    QueryReWriter re("other");
    re.SetRewriteCodes(&g_codes);

    XString input = "SELECT sysdate FROM dual";
    XString output = re.Parse(input);
    XString expect = "SELECT CURRENT_TIMESTAMP FROM other.dual";

    // Compare MUST be zero
    Assert::IsFalse(expect.Compare(output));
  }

  TEST_METHOD(RewriterTestODBCEscapes1)
  {
    Logger::WriteMessage("Testing Schema Rewriter ODBC Escapes conversion");

    FillODBCEscapes();
    QueryReWriter re("other");
    re.SetODBCFunctions(&g_odbc);

    XString input = "SELECT lower(nvl(one,'')) FROM dual;";
    XString output = re.Parse(input);
    XString expect = "SELECT {fn LCASE({fn ISNULL(one,'')})} FROM other.dual;";

    // Compare MUST be zero
    Assert::IsFalse(expect.Compare(output));
  }

  TEST_METHOD(RewriterTestODBCEscapes2)
  {
    Logger::WriteMessage("Testing Schema Rewriter ODBC Escapes conversion");

    FillODBCEscapes();
    QueryReWriter re("other");
    re.SetODBCFunctions(&g_odbc);

    XString input = "SELECT lower(instr(name,'some')) FROM dual;";
    XString output = re.Parse(input);
    XString expect = "SELECT {fn LCASE(instr(name,'some'))} FROM other.dual;";

    // Compare MUST be zero
    Assert::IsFalse(expect.Compare(output));
  }

  TEST_METHOD(RewriterTestSpecials)
  {
    Logger::WriteMessage("Testing Schema Rewriter special schema treatment");

    FillSpecials();
    QueryReWriter re("myschema");
    re.SetSchemaSpecial(&g_specials);

    XString input = "SELECT brief_ew(name,12345) FROM mytable";
    XString output = re.Parse(input);
    XString expect = "SELECT metaschema.brief_ew(name,12345) FROM myschema.mytable";

    // Compare MUST be zero
    Assert::IsFalse(expect.Compare(output));
  }
};
}
