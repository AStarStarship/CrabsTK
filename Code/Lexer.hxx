// Copyright AStarship <https://astarship.net>.
#include <_Config.h>
#include <string.h>   /* memcmp, size_t for raw-string delimiter compare */
#include "Lexer.h"
namespace CTK {
namespace Code {

/* Keyword table: C + C++11 + C++20 keywords. Sorted alphabetically
for binary search. Derived from Clang TokenKinds.def (BSD license).
The table is a flat array of (text, length) pairs. */
struct TCodeKeyword {
  const CHA* text;
  IUB length;
};

static const TCodeKeyword CodeKeywordTable[] = {
  {"alignas", 7},      {"alignof", 7},      {"asm", 3},
  {"auto", 4},         {"BOL", 4},         {"break", 5},
  {"case", 4},         {"catch", 5},        {"CHR", 4},
  {"char16_t", 8},     {"char32_t", 8},     {"char8_t", 7},
  {"class", 5},        {"co_await", 8},     {"co_return", 9},
  {"co_yield", 8},     {"concept", 7},      {"const", 5},
  {"const_cast", 10},  {"constexpr", 9},    {"continue", 8},
  {"decltype", 8},     {"default", 7},      {"delete", 6},
  {"do", 2},           {"double", 6},       {"dynamic_cast", 12},
  {"else", 4},         {"enum", 4},         {"explicit", 8},
  {"export", 6},       {"extern", 6},       {"false", 5},
  {"FPC", 5},        {"for", 3},          {"friend", 6},
  {"goto", 4},         {"if", 2},           {"inline", 6},
  {"ISC", 3},          {"long", 4},         {"mutable", 7},
  {"namespace", 9},    {"new", 3},          {"noexcept", 8},
  {"nullptr", 7},      {"operator", 8},     {"private", 7},
  {"protected", 9},    {"public", 6},       {"register", 8},
  {"reinterpret_cast", 16}, {"requires", 8}, {"restrict", 8},
  {"return", 6},       {"short", 5},        {"signed", 6},
  {"sizeof", 6},       {"static", 6},       {"static_assert", 13},
  {"static_cast", 11}, {"struct", 6},       {"switch", 6},
  {"template", 8},     {"this", 4},         {"thread_local", 12},
  {"throw", 5},        {"true", 4},         {"try", 3},
  {"typedef", 7},      {"typeid", 6},       {"typename", 8},
  {"union", 5},        {"unsigned", 8},     {"using", 5},
  {"virtual", 7},      {"void", 4},         {"volatile", 8},
  {"wchar_t", 7},      {"while", 5},
};

static const ISC CodeKeywordCount =
  ISC(sizeof(CodeKeywordTable) / sizeof(CodeKeywordTable[0]));

/* Binary search the keyword table. Returns true if text[0..length) is
a known C++ keyword. Case-sensitive exact match. A length is a
measurement, so it is ISC (pinned 32-bit), not ISC. */
BOL CodeIsKeyword(const CHA* text, ISC length) {
  ISC lo = 0, hi = CodeKeywordCount - 1;
  while (lo <= hi) {
    ISC mid = (lo + hi) / 2;
    const TCodeKeyword& kw = CodeKeywordTable[mid];
    /* strcmp-style comparison: compare up to min(length), then
    resolve by length if all compared chars match. */
    ISC n = (length < ISC(kw.length)) ? length : ISC(kw.length);
    ISC cmp = 0;
    for (ISC i = 0; i < n; ++i) {
      if (text[i] != kw.text[i]) {
        cmp = (text[i] < kw.text[i]) ? -1 : 1;
        break;
      }
    }
    if (cmp == 0) {
      /* All compared chars match — resolve by length. */
      if (length == ISC(kw.length)) return true;
      if (length < ISC(kw.length)) hi = mid - 1;
      else lo = mid + 1;
    } else if (cmp < 0) {
      hi = mid - 1;
    } else {
      lo = mid + 1;
    }
  }
  return false;
}

/* Token spelling table for punctuation operators.
Index: CODE_TOK_LPAREN .. CODE_TOK_RSHIFT_ASSIGN. */
static const CHA* CodePunctSpelling[] = {
  "(",      /* CODE_TOK_LPAREN */
  ")",      /* CODE_TOK_RPAREN */
  "[",      /* CODE_TOK_LBRACKET */
  "]",      /* CODE_TOK_RBRACKET */
  "{",      /* CODE_TOK_LBRACE */
  "}",      /* CODE_TOK_RBRACE */
  ";",      /* CODE_TOK_SEMICOLON */
  ":",      /* CODE_TOK_COLON */
  "::",     /* CODE_TOK_DBL_COLON */
  ",",      /* CODE_TOK_COMMA */
  ".",      /* CODE_TOK_DOT */
  ".*",     /* CODE_TOK_DOT_STAR */
  "->",     /* CODE_TOK_ARROW */
  "->*",    /* CODE_TOK_ARROW_STAR */
  "...",    /* CODE_TOK_ELLIPSIS */
  "?",      /* CODE_TOK_QUESTION */
  "@",      /* CODE_TOK_AT */
  "=",      /* CODE_TOK_ASSIGN */
  "+",      /* CODE_TOK_PLUS */
  "-",      /* CODE_TOK_MINUS */
  "*",      /* CODE_TOK_STAR */
  "/",      /* CODE_TOK_SLASH */
  "%",      /* CODE_TOK_PERCENT */
  "&",      /* CODE_TOK_AMP */
  "|",      /* CODE_TOK_PIPE */
  "^",      /* CODE_TOK_CARET */
  "~",      /* CODE_TOK_TILDE */
  "!",      /* CODE_TOK_BANG */
  "<",      /* CODE_TOK_LT */
  ">",      /* CODE_TOK_GT */
  "<=",     /* CODE_TOK_LE */
  ">=",     /* CODE_TOK_GE */
  "==",     /* CODE_TOK_EQ */
  "!=",     /* CODE_TOK_NE */
  "&&",     /* CODE_TOK_AND */
  "||",     /* CODE_TOK_OR */
  "++",     /* CODE_TOK_INC */
  "--",     /* CODE_TOK_DEC */
  "<<",     /* CODE_TOK_LSHIFT */
  ">>",     /* CODE_TOK_RSHIFT */
  "<<<",    /* CODE_TOK_LSHIFT3 */
  ">>>",    /* CODE_TOK_RSHIFT3 */
  "<=>",    /* CODE_TOK_SPACESHIP */
  "^^",     /* CODE_TOK_CARET_CARET */
  "+=",     /* CODE_TOK_PLUS_ASSIGN */
  "-=",     /* CODE_TOK_MINUS_ASSIGN */
  "*=",     /* CODE_TOK_STAR_ASSIGN */
  "/=",     /* CODE_TOK_SLASH_ASSIGN */
  "%=",     /* CODE_TOK_PERCENT_ASSIGN */
  "&=",     /* CODE_TOK_AMP_ASSIGN */
  "|=",     /* CODE_TOK_PIPE_ASSIGN */
  "^=",     /* CODE_TOK_CARET_ASSIGN */
  "<<=",    /* CODE_TOK_LSHIFT_ASSIGN */
  ">>=",    /* CODE_TOK_RSHIFT_ASSIGN */
  "#",      /* CODE_TOK_HASH */
  "##",     /* CODE_TOK_HASH_HASH */
  "#@",     /* CODE_TOK_HASH_AT */
};

static const IUB CodePunctFirst = CODE_TOK_LPAREN;
static const IUB CodePunctLast  = CODE_TOK_HASH_AT;

const CHA* CodeTokenSpelling(IUB kind) {
  if (kind >= CodePunctFirst && kind <= CodePunctLast) {
    return CodePunctSpelling[kind - CodePunctFirst];
  }
  return _NilP;
}

/* Token names for diagnostics. Designated initialisers pin each name to its
   kind so the table can never drift out of step with ECodeToken; punctuation
   kinds are named via CodeTokenSpelling() and left nil here. */
static const CHA* CodeTokenNames[CODE_TOK_COUNT] = {
  [CODE_TOK_EOF]           = "eof",
  [CODE_TOK_IDENT]         = "identifier",
  [CODE_TOK_KEYWORD]       = "keyword",
  [CODE_TOK_INT]           = "int_literal",
  [CODE_TOK_FLOAT]         = "float_literal",
  [CODE_TOK_CHAR]          = "char_literal",
  [CODE_TOK_STRING]        = "string_literal",
  [CODE_TOK_USER_STRING]   = "user_string",
  [CODE_TOK_COMMENT_LINE]  = "line_comment",
  [CODE_TOK_COMMENT_BLOCK] = "block_comment",
  [CODE_TOK_SPACE]         = "space",
  [CODE_TOK_NEWLINE]       = "newline",
};

const CHA* CodeTokenName(IUB kind) {
  if (kind >= CODE_TOK_COUNT) return "unknown";
  if (kind >= CodePunctFirst && kind <= CodePunctLast) {
    const CHA* sp = CodeTokenSpelling(kind);
    if (sp) return sp;
  }
  const CHA* name = CodeTokenNames[kind];
  return name ? name : "punct";
}

/* --- Character classification helpers --- */

static BOL CodeIsIdentStart(CHA c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static BOL CodeIsIdentChar(CHA c) {
  return CodeIsIdentStart(c) || (c >= '0' && c <= '9');
}

static BOL CodeIsDigit(CHA c) {
  return c >= '0' && c <= '9';
}

static BOL CodeIsHexDigit(CHA c) {
  return CodeIsDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static BOL CodeIsSpace(CHA c) {
  return c == ' ' || c == '\t' || c == '\r';
}

/* --- Lexer initialization --- */

ISC CodeLexerInit(TCodeLexer* lexer, const CHA* source, IUD source_length,
                  TCodeToken* tokens, ISC token_capacity) {
  if (IsError(lexer) || IsError(tokens) || token_capacity <= 0) return -1;
  if (IsError(source) && source_length > 0) return -1;
  lexer->source = source;
  lexer->source_length = source_length;
  lexer->pos = 0;
  lexer->line = 1;
  lexer->column = 1;
  lexer->tokens = tokens;
  lexer->token_count = 0;
  lexer->token_capacity = token_capacity;
  lexer->full = false;
  return 0;
}

/* --- Internal: record a token --- */

static void CodeRecordToken(TCodeLexer* lexer, IUB kind, ISC start_pos,
                            ISC start_line, ISC start_col) {
  if (lexer->token_count >= lexer->token_capacity) {
    lexer->full = true;
    return;
  }
  TCodeToken* token = &lexer->tokens[lexer->token_count];
  token->kind = kind;
  token->byte_start = start_pos;
  token->byte_end = lexer->pos;
  token->line = start_line;
  token->column = start_col;
  ++lexer->token_count;
}

/* --- Internal: skip whitespace, producing SPACE and NEWLINE tokens --- */

static void CodeSkipWhitespace(TCodeLexer* lexer) {
  while (lexer->pos < (ISC)lexer->source_length) {
    CHA c = lexer->source[lexer->pos];
    if (c == '\n' || c == '\r') {
      ISC start = lexer->pos;
      ISC start_line = lexer->line;
      ISC start_col = lexer->column;
      if (c == '\r' && lexer->pos + 1 < (ISC)lexer->source_length &&
          lexer->source[lexer->pos + 1] == '\n') {
        lexer->pos += 2;
      } else {
        ++lexer->pos;
      }
      ++lexer->line;
      lexer->column = 1;
      CodeRecordToken(lexer, CODE_TOK_NEWLINE, start, start_line, start_col);
    } else if (CodeIsSpace(c)) {
      ISC start = lexer->pos;
      ISC start_line = lexer->line;
      ISC start_col = lexer->column;
      while (lexer->pos < (ISC)lexer->source_length &&
             CodeIsSpace(lexer->source[lexer->pos])) {
        ++lexer->pos;
        ++lexer->column;
      }
      CodeRecordToken(lexer, CODE_TOK_SPACE, start, start_line, start_col);
    } else {
      break;
    }
  }
}

/* --- Internal: lex a string literal --- */
/* Handles: "normal", L"wide", u8"utf8", u"utf16", U"utf32",
   R"delim(raw)delim", L R"delim(raw)delim", etc. */

static void CodeLexString(TCodeLexer* lexer, ISC start_pos, ISC start_line,
                          ISC start_col) {
  const CHA* source = lexer->source;
  const IUD length = lexer->source_length;
  ISC pos = lexer->pos;

  /* Skip the encoding prefix (u8 / L / u / U / R) to reach the opening quote.
     Without this, L"..." tokenized as the 2-byte string L" and the remainder
     of the file was swallowed as one giant string. */
  ISC qpos = pos;
  while ((IUD)qpos < length &&
         (source[qpos] == 'L' || source[qpos] == 'u' || source[qpos] == 'U' ||
          source[qpos] == 'R' || source[qpos] == '8'))
    ++qpos;
  if ((IUD)qpos >= length || source[qpos] != '"') {
    /* Not a string after all: consume just the prefix char and move on. */
    lexer->pos = (qpos > pos) ? qpos : pos + 1;
    lexer->column = start_col + (ISC)(lexer->pos - start_pos);
    CodeRecordToken(lexer, CODE_TOK_STRING, start_pos, start_line, start_col);
    return;
  }
  const BOL is_raw = (qpos > pos && source[qpos - 1] == 'R');

  /* Raw string: R"delim( ... )delim" */
  if (is_raw) {
    ISC delim_start = qpos + 1;
    ISC delim_end = delim_start;
    while ((IUD)delim_end < length && source[delim_end] != '(') ++delim_end;
    if ((IUD)delim_end < length) {
      ++delim_end; /* skip ( */
      while ((IUD)delim_end < length) {
        if (source[delim_end] == ')' &&
            memcmp(source + delim_end + 1, source + delim_start,
                   (size_t)(delim_end - delim_start)) == 0) {
          delim_end += (delim_end - delim_start) + 2; /* )delim" */
          break;
        }
        ++delim_end;
      }
      lexer->pos = ((IUD)delim_end < length) ? delim_end : (ISC)length;
      /* Recount line/column: raw strings can span multiple lines. */
      lexer->line = start_line;
      lexer->column = start_col;
      for (ISC i = start_pos; (IUD)i < length && i < lexer->pos; ++i) {
        if (source[i] == '\n') { ++lexer->line; lexer->column = 1; }
        else ++lexer->column;
      }
      CodeRecordToken(lexer, CODE_TOK_STRING, start_pos, start_line, start_col);
      return;
    }
  }

  /* Normal string: from the opening quote to the closing quote, honoring
     backslash escapes. */
  pos = qpos + 1; /* skip opening quote */
  while ((IUD)pos < length && source[pos] != '"') {
    if (source[pos] == '\\' && (IUD)(pos + 1) < length) {
      pos += 2; /* skip escape */
    } else {
      ++pos;
    }
  }
  if ((IUD)pos < length) ++pos; /* skip closing quote */
  lexer->pos = pos;
  lexer->column = start_col + (ISC)(pos - start_pos);
  CodeRecordToken(lexer, CODE_TOK_STRING, start_pos, start_line, start_col);
}

/* --- Internal: lex a character literal --- */

static void CodeLexChar(TCodeLexer* lexer, ISC start_pos, ISC start_line,
                        ISC start_col) {
  const CHA* source = lexer->source;
  IUD length = lexer->source_length;
  ISC pos = lexer->pos + 1; /* skip opening quote */
  while (pos < (ISC)length && source[pos] != '\'') {
    if (source[pos] == '\\' && pos + 1 < (ISC)length) pos += 2;
    else ++pos;
  }
  if (pos < (ISC)length) ++pos; /* skip closing quote */
  lexer->pos = pos;
  lexer->column = start_col + (ISC)(pos - start_pos);
  CodeRecordToken(lexer, CODE_TOK_CHAR, start_pos, start_line, start_col);
}

/* --- Internal: lex a number --- */

static void CodeLexNumber(TCodeLexer* lexer, ISC start_pos, ISC start_line,
                          ISC start_col) {
  const CHA* source = lexer->source;
  const IUD length = lexer->source_length;
  ISC pos = lexer->pos;
  BOL is_float = false;
  BOL have_prefix = false;

  /* Hex, octal, binary prefix. */
  if (source[pos] == '0' && (IUD)pos + 1 < length) {
    CHA next = source[pos + 1];
    if (next == 'x' || next == 'X') {
      have_prefix = true;
      pos += 2;
      while ((IUD)pos < length && CodeIsHexDigit(source[pos])) ++pos;
      /* Hex FPC: 0x1.8p3 */
      if ((IUD)pos < length && source[pos] == '.') {
        is_float = true;
        ++pos;
        while ((IUD)pos < length && CodeIsHexDigit(source[pos])) ++pos;
      }
      if ((IUD)pos < length && (source[pos] == 'p' || source[pos] == 'P')) {
        is_float = true;
        ++pos;
        if ((IUD)pos < length && (source[pos] == '+' || source[pos] == '-')) ++pos;
        while ((IUD)pos < length && CodeIsDigit(source[pos])) ++pos;
      }
    } else if (next >= '0' && next <= '7') {
      have_prefix = true;
      pos += 2;
      while ((IUD)pos < length && source[pos] >= '0' && source[pos] <= '7') ++pos;
    } else if (next == 'b' || next == 'B') {
      have_prefix = true;
      pos += 2;
      while ((IUD)pos < length && (source[pos] == '0' || source[pos] == '1')) ++pos;
    }
  }

  if (!have_prefix) {
    /* Decimal integer or FPC. Covers bare '0' (no base prefix) so pos
       always advances at least one digit. */
    while ((IUD)pos < length && CodeIsDigit(source[pos])) ++pos;
    if ((IUD)pos < length && source[pos] == '.') {
      is_float = true;
      ++pos;
      while ((IUD)pos < length && CodeIsDigit(source[pos])) ++pos;
    }
    if ((IUD)pos < length && (source[pos] == 'e' || source[pos] == 'E')) {
      is_float = true;
      ++pos;
      if ((IUD)pos < length && (source[pos] == '+' || source[pos] == '-')) ++pos;
      while ((IUD)pos < length && CodeIsDigit(source[pos])) ++pos;
    }
  }

  /* Type suffix: u, U, l, L, ll, LL, f, F. */
  while ((IUD)pos < length) {
    CHA c = source[pos];
    if (c == 'u' || c == 'U' || c == 'l' || c == 'L' ||
        c == 'f' || c == 'F') {
      ++pos;
    } else {
      break;
    }
  }

  lexer->pos = pos;
  lexer->column = start_col + (ISC)(pos - start_pos);
  CodeRecordToken(lexer, is_float ? CODE_TOK_FLOAT : CODE_TOK_INT,
                  start_pos, start_line, start_col);
}

/* --- Internal: lex an identifier or keyword --- */

static void CodeLexIdent(TCodeLexer* lexer, ISC start_pos, ISC start_line,
                         ISC start_col) {
  const CHA* source = lexer->source;
  IUD length = lexer->source_length;
  ISC pos = lexer->pos;

  /* String literal prefixes: L" , u8" , u" , U" , R" */
  if (pos < (ISC)length && source[pos] == 'L' && pos + 1 < (ISC)length &&
      source[pos + 1] == '"') {
    lexer->pos = pos; /* let the string lexer handle it */
    CodeLexString(lexer, start_pos, start_line, start_col);
    return;
  }
  if (pos < (ISC)length && source[pos] == 'U' && pos + 1 < (ISC)length &&
      source[pos + 1] == '"') {
    lexer->pos = pos;
    CodeLexString(lexer, start_pos, start_line, start_col);
    return;
  }
  if (pos < (ISC)length && source[pos] == 'u' && pos + 1 < (ISC)length) {
    if (source[pos + 1] == '"') {
      lexer->pos = pos;
      CodeLexString(lexer, start_pos, start_line, start_col);
      return;
    }
    if (source[pos + 1] == '8' && pos + 2 < (ISC)length && source[pos + 2] == '"') {
      lexer->pos = pos;
      CodeLexString(lexer, start_pos, start_line, start_col);
      return;
    }
  }
  if (pos < (ISC)length && source[pos] == 'R' && pos + 1 < (ISC)length &&
      source[pos + 1] == '"') {
    lexer->pos = pos;
    CodeLexString(lexer, start_pos, start_line, start_col);
    return;
  }

  /* Consume identifier characters. */
  while (pos < (ISC)length && CodeIsIdentChar(source[pos])) ++pos;
  ISC ident_length = pos - start_pos;
  lexer->pos = pos;
  lexer->column = start_col + ident_length;

  IUB kind = CODE_TOK_IDENT;
  if (CodeIsKeyword(source + start_pos, ident_length)) kind = CODE_TOK_KEYWORD;

  /* User-defined string literal: "hello"_suffix */
  CodeRecordToken(lexer, kind, start_pos, start_line, start_col);

  /* Check for user-defined literal suffix on the NEXT token.
  The Fitter will handle "str"_lit as STRING + IDENT. */
}

/* --- Main lexer step --- */

IUB CodeLexerNext(TCodeLexer* lexer) {
  if (lexer->full) return CODE_TOK_EOF;

  /* Skip whitespace and comments. */
  while (lexer->pos < (ISC)lexer->source_length) {
    CHA c = lexer->source[lexer->pos];
    if (CodeIsSpace(c) || c == '\n' || c == '\r') {
      CodeSkipWhitespace(lexer);
      continue;
    }
    break;
  }

  if (lexer->pos >= (ISC)lexer->source_length) {
    CodeRecordToken(lexer, CODE_TOK_EOF, lexer->pos, lexer->line,
                    lexer->column);
    return CODE_TOK_EOF;
  }

  ISC start = lexer->pos;
  ISC start_line = lexer->line;
  ISC start_col = lexer->column;
  CHA c = lexer->source[lexer->pos];

  /* Comments. */
  if (c == '/' && lexer->pos + 1 < (ISC)lexer->source_length) {
    CHA next = lexer->source[lexer->pos + 1];
    if (next == '/') {
      /* Line comment: // ... \n */
      lexer->pos += 2;
      while (lexer->pos < (ISC)lexer->source_length &&
             lexer->source[lexer->pos] != '\n') {
        ++lexer->pos;
        ++lexer->column;
      }
      /* Don't consume the newline; the next call will. */
      CodeRecordToken(lexer, CODE_TOK_COMMENT_LINE, start, start_line,
                      start_col);
      return CODE_TOK_COMMENT_LINE;
    }
    if (next == '*') {
      /* Block comment: open-brace-star ... star-close-brace */
      lexer->pos += 2;
      lexer->column += 2;
      while (lexer->pos + 1 < (ISC)lexer->source_length) {
        if (lexer->source[lexer->pos] == '*' &&
            lexer->source[lexer->pos + 1] == '/') {
          lexer->pos += 2;
          lexer->column += 2;
          break;
        }
        if (lexer->source[lexer->pos] == '\n') {
          ++lexer->line;
          lexer->column = 1;
        } else {
          ++lexer->column;
        }
        ++lexer->pos;
      }
      CodeRecordToken(lexer, CODE_TOK_COMMENT_BLOCK, start, start_line,
                      start_col);
      return CODE_TOK_COMMENT_BLOCK;
    }
  }

  /* String / CHR literal (with optional prefix already consumed as ident). */
  if (c == '"') {
    CodeLexString(lexer, start, start_line, start_col);
    return CODE_TOK_STRING;
  }
  if (c == '\'') {
    CodeLexChar(lexer, start, start_line, start_col);
    return CODE_TOK_CHAR;
  }

  /* Number. */
  if (CodeIsDigit(c)) {
    CodeLexNumber(lexer, start, start_line, start_col);
    return CODE_TOK_INT; /* or FLOAT, determined inside */
  }

  /* Identifier / keyword. */
  if (CodeIsIdentStart(c)) {
    CodeLexIdent(lexer, start, start_line, start_col);
    /* Return the actual kind from the recorded token. */
    if (lexer->token_count > 0) {
      return lexer->tokens[lexer->token_count - 1].kind;
    }
    return CODE_TOK_IDENT;
  }

  /* Multi-CHR operators (longest match first). */
  const CHA* source = lexer->source;
  IUD length = lexer->source_length;

  /* Three-CHR: <<<, >>>, <<=, >>=, <=>, ... */
  if (lexer->pos + 2 < (ISC)length) {
    CHA c2 = source[lexer->pos + 1], c3 = source[lexer->pos + 2];
    if (c == '<' && c2 == '<' && c3 == '<') {
      lexer->pos += 3; lexer->column += 3;
      CodeRecordToken(lexer, CODE_TOK_LSHIFT3, start, start_line, start_col);
      return CODE_TOK_LSHIFT3;
    }
    if (c == '>' && c2 == '>' && c3 == '>') {
      lexer->pos += 3; lexer->column += 3;
      CodeRecordToken(lexer, CODE_TOK_RSHIFT3, start, start_line, start_col);
      return CODE_TOK_RSHIFT3;
    }
    if (c == '<' && c2 == '=' && c3 == '>') {
      lexer->pos += 3; lexer->column += 3;
      CodeRecordToken(lexer, CODE_TOK_SPACESHIP, start, start_line, start_col);
      return CODE_TOK_SPACESHIP;
    }
    if (c == '.' && c2 == '.' && c3 == '.') {
      lexer->pos += 3; lexer->column += 3;
      CodeRecordToken(lexer, CODE_TOK_ELLIPSIS, start, start_line, start_col);
      return CODE_TOK_ELLIPSIS;
    }
  }

  /* Two-CHR operators. */
  if (lexer->pos + 1 < (ISC)length) {
    CHA c2 = source[lexer->pos + 1];
    IUB two_char = CODE_TOK_EOF;
    if (c == ':' && c2 == ':') two_char = CODE_TOK_DBL_COLON;
    else if (c == '.' && c2 == '*') two_char = CODE_TOK_DOT_STAR;
    else if (c == '-' && c2 == '>') {
      if (lexer->pos + 2 < (ISC)length && source[lexer->pos + 2] == '*') {
        /* ->* is three chars; handle as ARROW_STAR. */
        lexer->pos += 3; lexer->column += 3;
        CodeRecordToken(lexer, CODE_TOK_ARROW_STAR, start, start_line, start_col);
        return CODE_TOK_ARROW_STAR;
      }
      two_char = CODE_TOK_ARROW;
    }
    else if (c == '=' && c2 == '=') two_char = CODE_TOK_EQ;
    else if (c == '!' && c2 == '=') two_char = CODE_TOK_NE;
    else if (c == '<' && c2 == '=') two_char = CODE_TOK_LE;
    else if (c == '>' && c2 == '=') two_char = CODE_TOK_GE;
    else if (c == '<' && c2 == '<') {
      if (lexer->pos + 2 < (ISC)length && source[lexer->pos + 2] == '=') {
        lexer->pos += 3; lexer->column += 3;
        CodeRecordToken(lexer, CODE_TOK_LSHIFT_ASSIGN, start, start_line, start_col);
        return CODE_TOK_LSHIFT_ASSIGN;
      }
      two_char = CODE_TOK_LSHIFT;
    }
    else if (c == '>' && c2 == '>') {
      if (lexer->pos + 2 < (ISC)length && source[lexer->pos + 2] == '=') {
        lexer->pos += 3; lexer->column += 3;
        CodeRecordToken(lexer, CODE_TOK_RSHIFT_ASSIGN, start, start_line, start_col);
        return CODE_TOK_RSHIFT_ASSIGN;
      }
      two_char = CODE_TOK_RSHIFT;
    }
    else if (c == '&' && c2 == '&') two_char = CODE_TOK_AND;
    else if (c == '|' && c2 == '|') two_char = CODE_TOK_OR;
    else if (c == '+' && c2 == '+') two_char = CODE_TOK_INC;
    else if (c == '-' && c2 == '-') two_char = CODE_TOK_DEC;
    else if (c == '^' && c2 == '^') two_char = CODE_TOK_CARET_CARET;
    else if (c == '#' && c2 == '#') two_char = CODE_TOK_HASH_HASH;
    else if (c == '#' && c2 == '@') two_char = CODE_TOK_HASH_AT;
    else if (c == '+' && c2 == '=') two_char = CODE_TOK_PLUS_ASSIGN;
    else if (c == '-' && c2 == '=') two_char = CODE_TOK_MINUS_ASSIGN;
    else if (c == '*' && c2 == '=') two_char = CODE_TOK_STAR_ASSIGN;
    else if (c == '/' && c2 == '=') two_char = CODE_TOK_SLASH_ASSIGN;
    else if (c == '%' && c2 == '=') two_char = CODE_TOK_PERCENT_ASSIGN;
    else if (c == '&' && c2 == '=') two_char = CODE_TOK_AMP_ASSIGN;
    else if (c == '|' && c2 == '=') two_char = CODE_TOK_PIPE_ASSIGN;
    else if (c == '^' && c2 == '=') two_char = CODE_TOK_CARET_ASSIGN;

    if (two_char != CODE_TOK_EOF) {
      lexer->pos += 2;
      lexer->column += 2;
      CodeRecordToken(lexer, two_char, start, start_line, start_col);
      return two_char;
    }
  }

  /* Single-CHR tokens. */
  IUB single = CODE_TOK_EOF;
  switch (c) {
    case '(': single = CODE_TOK_LPAREN; break;
    case ')': single = CODE_TOK_RPAREN; break;
    case '[': single = CODE_TOK_LBRACKET; break;
    case ']': single = CODE_TOK_RBRACKET; break;
    case '{': single = CODE_TOK_LBRACE; break;
    case '}': single = CODE_TOK_RBRACE; break;
    case ';': single = CODE_TOK_SEMICOLON; break;
    case ':': single = CODE_TOK_COLON; break;
    case ',': single = CODE_TOK_COMMA; break;
    case '.': single = CODE_TOK_DOT; break;
    case '?': single = CODE_TOK_QUESTION; break;
    case '@': single = CODE_TOK_AT; break;
    case '=': single = CODE_TOK_ASSIGN; break;
    case '+': single = CODE_TOK_PLUS; break;
    case '-': single = CODE_TOK_MINUS; break;
    case '*': single = CODE_TOK_STAR; break;
    case '/': single = CODE_TOK_SLASH; break;
    case '%': single = CODE_TOK_PERCENT; break;
    case '&': single = CODE_TOK_AMP; break;
    case '|': single = CODE_TOK_PIPE; break;
    case '^': single = CODE_TOK_CARET; break;
    case '~': single = CODE_TOK_TILDE; break;
    case '!': single = CODE_TOK_BANG; break;
    case '<': single = CODE_TOK_LT; break;
    case '>': single = CODE_TOK_GT; break;
    case '#': single = CODE_TOK_HASH; break;
    default: break;
  }

  if (single != CODE_TOK_EOF) {
    ++lexer->pos;
    ++lexer->column;
    CodeRecordToken(lexer, single, start, start_line, start_col);
    return single;
  }

  /* Unknown character: record as IDENT (single CHR) to avoid
  crashing the pipeline. The Fitter will flag it. */
  ++lexer->pos;
  ++lexer->column;
  CodeRecordToken(lexer, CODE_TOK_IDENT, start, start_line, start_col);
  return CODE_TOK_IDENT;
}

}  //< namespace Code
}  //< namespace CTK
