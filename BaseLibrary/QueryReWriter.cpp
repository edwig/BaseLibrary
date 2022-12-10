/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: QueryReWriter.cpp
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
#include "pch.h"
#include "QueryReWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const char* all_tokens[] =
{
   ""
  ,""
  ,""
  ,"'"
  ,"\""
  ,"."
  ,","
  ,"-"
  ,"/"
  ,"--"
  ,"/*"
  ,"//"
  ,"("
  ,")"
  ,"+"
  ,"||"
  ," "
  ,"\t"
  ,"\r"
  ,"\n"
  ,"SELECT"
  ,"INSERT"
  ,"UPDATE"
  ,"DELETE"
  ,"FROM"
  ,"JOIN"
  ,"WHERE"
  ,"GROUP"
  ,"ORDER"
  ,"HAVING"
  ,"INTO"
  ,"UNION"
};

// All registered SQL words including tokens and special registrations
static SQLWords g_allWords;

// Remove all words at the closing of the process
void QueryRewriterRemoveAllWords()
{
  g_allWords.clear();
}

///////////////////////////////////////////////////////////////////////////

QueryReWriter::QueryReWriter(XString p_schema)
               :m_schema(p_schema)
{
}

XString 
QueryReWriter::Parse(XString p_input)
{
  Reset();
  m_input = p_input;

  ParseStatement();

  if(m_level != 0)
  {
    throw StdException("Odd number of '(' and ')' tokens in the statement");
  }
  return m_output;
}

void
QueryReWriter::SetOption(SROption p_option)
{
  m_options |= (int) p_option;
}

bool
QueryReWriter::AddSQLWord(XString p_word
                         ,XString p_replacement
                         ,XString p_schema /*= ""*/
                         ,Token   p_token  /*= Token::TK_EOS*/
                         ,OdbcEsc p_odbc   /*= OdbcEsc::ODBCESC_None*/)
{
  // Must be sure we have the tokens beforehand
  Initialization();

  SQLWord word;
  word.m_word        = p_word;
  word.m_replacement = p_replacement;
  word.m_schema      = p_schema;
  word.m_token       = p_token;
  word.m_odbcEscape  = p_odbc;

  if(g_allWords.find(p_word) != g_allWords.end())
  {
    return false;
  }
  g_allWords.insert(std::make_pair(p_word,word));
  return true;
}

bool
QueryReWriter::AddSQLWord(SQLWord& p_word)
{
  // Must be sure we have the tokens beforehand
  Initialization();

  if(g_allWords.find(p_word.m_word) != g_allWords.end())
  {
    return false;
  }
  g_allWords.insert(std::make_pair(p_word.m_word,p_word));
  return true;
}

// Returns true if ALL words in parameter are added successfully
bool
QueryReWriter::AddSQLWords(SQLWords& p_words)
{
  // Must be sure we have the tokens beforehand
  Initialization();

  bool status(true);
  for(auto& word : p_words)
  {
    if(g_allWords.find(word.first) != g_allWords.end())
    {
      status = false;
    }
    g_allWords.insert(std::make_pair(word.first,word.second));
  }
  return status;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
QueryReWriter::Reset()
{
  // Primary in- and output
  m_input.Empty();
  m_output.Empty();
  // Processing data
  m_tokenString.Empty();
  m_token       = Token::TK_EOS;
  m_inStatement = Token::TK_EOS;
  m_inFrom      = false;
  m_nextTable   = false;
  m_position    = 0;
  m_level       = 0;
  m_ungetch     = 0;
  m_replaced    = 0;

  // Leave the following members untouched!
  // m_schema
  // m_options
  // m_codes / m_odbcfuncs / m_specials
  Initialization();
}

// Be carefully when called from outside that you call it 
// only once with the 'force' option
void
QueryReWriter::Initialization()
{
  // Only do the initialization once
  if(!g_allWords.empty())
  {
    return;
  }

  // At a minimum, we need all tokens
  for(int ind = 0; ind < sizeof(all_tokens) / sizeof(const char*); ++ind)
  {
    SQLWord word;
    word.m_word = all_tokens[ind];
    word.m_token = (Token) ind;
    word.m_odbcEscape = OdbcEsc::ODBCESC_None;

    g_allWords.insert(std::make_pair(all_tokens[ind],word));
  }

  // Remove when process dies
  atexit(QueryRewriterRemoveAllWords);
}

void
QueryReWriter::ParseStatement(bool p_closingEscape /*= false*/)
{
  bool begin = true;
  bool odbc  = false;

  ++m_level;
  while(true)
  {
    m_token = GetToken();
    if(m_token == Token::TK_EOS)
    {
      break;
    }

    SkipSpaceAndComment();

    // First token in the statement
    if(begin && (m_token == Token::TK_SELECT || 
                 m_token == Token::TK_INSERT || 
                 m_token == Token::TK_UPDATE || 
                 m_token == Token::TK_DELETE))
    {
      m_inStatement = m_token;
      begin = false;
    }

    // Next level statement
    if(m_token == Token::TK_PAR_OPEN)
    {
      PrintToken();
      Token oldStatement = m_inStatement;
      bool  oldFrom      = m_inFrom;
      m_inStatement      = Token::TK_EOS;
      ParseStatement(odbc);
      m_inStatement      = oldStatement;
      m_inFrom           = oldFrom;
      continue;
    }
    else if(m_token == Token::TK_PLAIN_ODBC)
    {
      odbc = true;
    }

    // Append schema
    if(m_nextTable)
    {
      AppendSchema();
    }

    // Find next table for appending a schema
    if(m_inStatement == Token::TK_SELECT && (m_token == Token::TK_FROM || m_token == Token::TK_JOIN))
    {
      m_nextTable = true;
      if(m_token == Token::TK_FROM)
      {
        m_inFrom = true;
      }
    }
    if(m_inStatement == Token::TK_SELECT && m_inFrom && m_token == Token::TK_COMMA)
    {
      m_nextTable = true;
    }
    if(m_inStatement == Token::TK_INSERT && m_token == Token::TK_INTO)
    {
      m_nextTable = true;
    }
    if(m_inStatement == Token::TK_UPDATE)
    {
      m_nextTable = true;
    }
    if(m_inStatement == Token::TK_DELETE && m_token == Token::TK_FROM)
    {
      m_nextTable = true;
    }

    // End of next table
    if(m_token == Token::TK_WHERE || m_token == Token::TK_GROUP || 
       m_token == Token::TK_ORDER || m_token == Token::TK_HAVING )
    {
      m_inFrom    = false;
      m_nextTable = false;
    }

    PrintToken();

    // Check reset on next UNION
    if(m_token == Token::TK_UNION)
    {
      begin         = true;
      m_inFrom      = false;
      m_nextTable   = false;
      m_inStatement = Token::TK_EOS;
    }

    // End of level
    if(m_token == Token::TK_PAR_CLOSE)
    {
      if(p_closingEscape)
      {
        m_output += "}";
      }
      break;
    }
  }
  --m_level;
}

void
QueryReWriter::PrintToken()
{
  switch(m_token)
  {
    case Token::TK_PLAIN:     [[fallthrough]];
    case Token::TK_PLAIN_ODBC:m_output += m_tokenString;
                              break;
    case Token::TK_SQUOTE:    m_output += '\'';
                              m_output += m_tokenString;
                              m_output += '\'';
                              break;
    case Token::TK_DQUOTE:    m_output += '\"';
                              m_output += m_tokenString;
                              m_output += '\"';
                              break;
    case Token::TK_COMM_SQL:  m_output += "--";
                              m_output += m_tokenString;
                              m_output += '\n';
                              break;
    case Token::TK_COMM_C:    m_output += "/*";
                              m_output += m_tokenString;
                              m_output += "/";
                              break;
    case Token::TK_COMM_CPP:  m_output += "//";
                              m_output += m_tokenString;
                              m_output += '\n';
                              break;
    case Token::TK_PAR_ADD:   m_output += (m_options & (int)SROption::SRO_ADD_TO_CONCAT) ? "||" : "+";
                              break;
    case Token::TK_PAR_CONCAT:m_output += (m_options & (int)SROption::SRO_CONCAT_TO_ADD) ? "+" : "||";
                              break;
    case Token::TK_POINT:     [[fallthrough]];
    case Token::TK_COMMA:     [[fallthrough]];
    case Token::TK_MINUS:     [[fallthrough]];
    case Token::TK_DIVIDE:    [[fallthrough]];
    case Token::TK_PAR_OPEN:  [[fallthrough]];
    case Token::TK_PAR_CLOSE: [[fallthrough]];
    case Token::TK_SPACE:     [[fallthrough]];
    case Token::TK_TAB:       [[fallthrough]];
    case Token::TK_CR:        [[fallthrough]];
    case Token::TK_NEWLINE:   [[fallthrough]];
    case Token::TK_SELECT:    [[fallthrough]];
    case Token::TK_INSERT:    [[fallthrough]];
    case Token::TK_UPDATE:    [[fallthrough]];
    case Token::TK_DELETE:    [[fallthrough]];
    case Token::TK_FROM:      [[fallthrough]];
    case Token::TK_JOIN:      [[fallthrough]];
    case Token::TK_WHERE:     [[fallthrough]];
    case Token::TK_GROUP:     [[fallthrough]];
    case Token::TK_ORDER:     [[fallthrough]];
    case Token::TK_HAVING:    [[fallthrough]];
    case Token::TK_INTO:      [[fallthrough]];
    case Token::TK_UNION:     m_output += all_tokens[(int)m_token];
                              break;
  }
}

// THIS IS WHY WE ARE HERE IN THIS CLASS!
void
QueryReWriter::AppendSchema()
{
  SkipSpaceAndComment();

  int ch = GetChar();

  if(ch == '.')
  {
    // There already was a schema name
    m_output += m_tokenString;
    m_output += '.';
    m_token   = GetToken();
  }
  else
  {
    // Reset token
    UnGetChar(ch);

    if(!m_schema.IsEmpty())
    {
      ++m_replaced;
      m_output += m_schema;
      m_output += '.';
    }
  }

  m_nextTable = false;

  // Reset for update
  if(m_inStatement == Token::TK_UPDATE)
  {
    m_inStatement = Token::TK_EOS;
  }
}

void
QueryReWriter::SkipSpaceAndComment()
{
  while(true)
  {
    if(m_token == Token::TK_SPACE || m_token == Token::TK_TAB || m_token == Token::TK_CR || m_token == Token::TK_NEWLINE)
    {
      PrintToken();
      m_token = GetToken();
      continue;
    }
    if(m_token == Token::TK_COMM_SQL || m_token == Token::TK_COMM_C || m_token == Token::TK_COMM_CPP)
    {
      PrintToken();
      m_token = GetToken();
      continue;
    }
    return;
  }
}


Token
QueryReWriter::GetToken()
{
  m_tokenString.Empty();
  int ch = 0;

  if((ch = GetChar()) != (int)Token::TK_EOS)
  {
    switch(ch)
    {
      case '\'':  QuoteString(ch);
                  return Token::TK_SQUOTE;
      case '\"':  QuoteString(ch);
                  return Token::TK_DQUOTE;
      case '-':   return CommentSQL();
      case '/':   return CommentCPP();
      case '|':   return StringConcatenate();
      case '.':   return Token::TK_POINT;
      case ',':   return Token::TK_COMMA;
      case '(':   return Token::TK_PAR_OPEN;
      case ')':   return Token::TK_PAR_CLOSE;
      case '+':   return Token::TK_PAR_ADD;
      case ' ':   return Token::TK_SPACE;
      case '\t':  return Token::TK_TAB;
      case '\r':  return Token::TK_CR;
      case '\n':  return Token::TK_NEWLINE;
    }
    if(isdigit(ch))
    {
      m_tokenString = XString((char)ch,1);
      return Token::TK_PLAIN;
    }
    if(isalpha(ch))
    {
      m_tokenString = XString((char) ch,1);
      ch = GetChar();
      while(isalnum(ch) || ch == '_')
      {
        m_tokenString += (char) ch;
        ch = GetChar();
      }
      if(ch)
      {
        UnGetChar(ch);
      }
      return FindToken();
    }
    m_tokenString = XString((char) ch,1);
    return Token::TK_PLAIN;
  }
  return Token::TK_EOS;
}

Token
QueryReWriter::FindToken()
{
  // The one-and-only lookup for a token
  SQLWords::iterator tok = g_allWords.find(m_tokenString);
  if(tok == g_allWords.end())
  {
    return Token::TK_PLAIN;
  }

  if(tok->second.m_odbcEscape != OdbcEsc::ODBCESC_None)
  {
    if(tok->second.m_odbcEscape == OdbcEsc::ODBCESC_Function)
    {
      m_tokenString = "{fn " + tok->second.m_replacement;
    }
    return Token::TK_PLAIN_ODBC;
  }
  else
  {
    // Possibly a replacement token and schema
    if(!tok->second.m_replacement.IsEmpty())
    {
      m_tokenString = tok->second.m_replacement;
    }
    if(!tok->second.m_schema.IsEmpty())
    {
      m_tokenString = tok->second.m_schema + "." + m_tokenString;
    }
  }

  // Return as a specific token or just a plain word
  if(tok->second.m_token != Token::TK_EOS)
  {
    return tok->second.m_token;
  }
  return Token::TK_PLAIN;
}

Token
QueryReWriter::CommentSQL()
{
  int ch = GetChar();
  if(ch == '-')
  {
    while(true)
    {
      ch = GetChar();
      if(ch == 0 || ch == '\n')
      {
        break;
      }
      m_tokenString += (char) ch;
    }
    return Token::TK_COMM_SQL;
  }
  UnGetChar(ch);
  return Token::TK_MINUS;
}

Token
QueryReWriter::CommentCPP()
{
  int ch = GetChar();
  if(ch == '/')
  {
    while(true)
    {
      ch = GetChar();
      if(ch == 0 || ch == '\n')
      {
        break;
      }
      m_tokenString += (char) ch;
    }
    return Token::TK_COMM_CPP;
  }
  else if(ch == '*')
  {
    int lastchar = 0;
    while(true)
    {
      ch = GetChar();
      if(ch == 0 || (ch == '/' && lastchar == '*'))
      {
        break;
      }
      m_tokenString += (char) ch;
      lastchar = ch;
    }
    return Token::TK_COMM_C;
  }
  else
  {
    UnGetChar(ch);
    return Token::TK_DIVIDE;
  }
}

Token
QueryReWriter::StringConcatenate()
{
  int ch = GetChar();
  if(ch == '|')
  {
    return Token::TK_PAR_CONCAT;
  }
  UnGetChar(ch);
  return Token::TK_PLAIN;
}

void
QueryReWriter::QuoteString(int p_ending)
{
  int ch = 0;

  while(true)
  {
    ch = GetChar();
    if(ch == p_ending || ch == 0)
    {
      return;
    }
    m_tokenString += (char) ch;
  } 
}

void
QueryReWriter::UnGetChar(int p_char)
{
  if(m_ungetch == 0)
  {
    m_ungetch = p_char;
  }
}

int
QueryReWriter::GetChar()
{
  if(m_ungetch)
  {
    int ch = m_ungetch;
    m_ungetch = 0;
    return ch;
  }
  if(m_position < m_input.GetLength())
  {
    return m_input.GetAt(m_position++);
  }
  return 0;
}
