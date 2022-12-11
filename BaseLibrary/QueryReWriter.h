/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: QueryReWriter.h
//
// BaseLibrary: Indispensable general objects and functions
// 
// Copyright (c) 2014-2022 ir. W.E. Huisman
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
#pragma once
#include <map>

enum class Token
{
  TK_EOS    = 0     // END-OF-STRING
 ,TK_PLAIN          // Plain character string
 ,TK_PLAIN_ODBC     // Plain ODBC replacement function
 ,TK_SQUOTE         // Single quote string
 ,TK_DQUOTE         // Double quote string
 ,TK_POINT          // .
 ,TK_COMMA          // ,
 ,TK_MINUS          // -
 ,TK_DIVIDE         // /
 ,TK_COMM_SQL       // --    Comments SQL wise
 ,TK_COMM_C         // /*    Comments C   wise
 ,TK_COMM_CPP       // //    Comments C++ wise
 ,TK_PAR_OPEN       // (
 ,TK_PAR_CLOSE      // )
 ,TK_PAR_OUTER      // (+)   Oracle outer join
 ,TK_PAR_ADD        // +
 ,TK_PAR_CONCAT     // ||    SQL String concatenation
 ,TK_SPACE          // ' '   Single space
 ,TK_TAB            // '\t'  Tabulate character
 ,TK_CR             // '\r'  Carriage return
 ,TK_NEWLINE        // '\n'  Newline
  // Complete SQL words
 ,TK_SELECT
 ,TK_INSERT
 ,TK_UPDATE
 ,TK_DELETE
 ,TK_FROM
 ,TK_JOIN
 ,TK_WHERE
 ,TK_GROUP
 ,TK_ORDER
 ,TK_HAVING 
 ,TK_INTO
 ,TK_UNION
};

enum class SROption
{
   SRO_NO_OPTION      = 0x0000
  ,SRO_CONCAT_TO_ADD  = 0x0001   // ISO SQL || to MS-SQL + for two strings
  ,SRO_ADD_TO_CONCAT  = 0x0002   // MS-SQL + to ISO SQL || for two strings
  ,SRO_WARN_OUTER     = 0x0004   // Warn for Oracle (+) Outer joins
};

enum class OdbcEsc
{
   ODBCESC_None      = 0
  ,ODBCESC_Function  = 1
  ,ODBCESC_Date      = 2
  ,ODBCESC_Timestamp = 3
};

typedef struct _sqlwords
{
  XString m_word;         // Word to recognize in the SQL statement
  Token   m_token;        // Possibly a token
  XString m_replacement;  // Replacement word/function
  XString m_schema;       // Possibly prefix with this schema
  OdbcEsc m_odbcEscape;   // Possibly an ODBC escape sequence
}
SQLWord;

// Map with case-insensitive string compare
struct StringICompare
{
  bool operator()(const XString lhs,const XString rhs) const
  {
    return lhs.CompareNoCase(rhs) < 0;
  }
};
using SQLWords = std::map<XString,SQLWord,StringICompare>;

class QueryReWriter
{
public:
  QueryReWriter(XString p_schema);
  // Our primary function
  XString Parse(XString p_input);

  // Settings 
  void    SetOption(SROption p_option);
  bool    AddSQLWord(XString p_word,XString p_replacement,XString p_schema = "",Token p_token = Token::TK_EOS,OdbcEsc p_odbc = OdbcEsc::ODBCESC_None);
  bool    AddSQLWord(SQLWord& p_word);
  bool    AddSQLWords(SQLWords& p_words);
  // Getters
  int     GetReplaced() { return m_replaced; };
  int     GetOptions()  { return m_options;  };

private:
  void    Reset();
  void    Initialization();
  void    ParseStatement(bool p_closingEscape = false);
  // Token parsing
  Token   GetToken();
  void    PrintToken();
  void    PrintOuterJoin();
  Token   FindToken();
  void    AppendSchema();
  void    SkipSpaceAndComment();
  Token   CommentSQL();
  Token   CommentCPP();
  Token   StringConcatenate();
  Token   Parenthesis();
  void    QuoteString(int p_ending);
  void    UnGetChar(int p_char);
  int     GetChar();

  // Primary data
  XString   m_schema;
  XString   m_input;
  XString   m_output;
  // Options for processing
  SQLWords  m_words;
  int       m_options     { 0       };
  // Processing data
  int       m_position    { 0       };
  Token     m_token       { Token::TK_EOS };
  XString   m_tokenString;
  int       m_level       { 0       };
  int       m_ungetch     { 0       };
  Token     m_inStatement { Token::TK_EOS };
  bool      m_inFrom      { false   };
  bool      m_nextTable   { false   };
  int       m_replaced    { 0       };
};
