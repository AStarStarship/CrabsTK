// Copyright AStarship <https://astarship.net>.
#include "_Config.h"
#include <cstring>  // memcmp (2026-10-12: was missing)
#include "Lexer.h"

namespace _ {

/* Keyword table: C + C++11 + C++20 keywords. Sorted alphabetically
for binary search. Derived from Clang TokenKinds.def (BSD license).
The table is a flat array of (text, length) pairs. */
struct TCodeKeyword {
  const CHA* text;
  IUB len;
};

static const TCodeKeyword CodeKeywordTable[] = {
  {"alignas", 7},      {"alignof", 7},      {"asm", 3},
  {"auto", 4},         {"bool", 4},         {"break", 5},
  {"case", 4},         {"catch", 5},        {"char", 4},
  {"char16_t", 8},     {"char32_t", 8},     {"char8_t", 7},
  {"class", 5},        {"co_await", 8},     {"co_return", 9},
  {"co_yield", 8},     {"concept", 7},      {"const", 5},
  {"const_cast", 10},  {"constexpr", 9},    {"continue", 8},
  {"decltype", 8},     {"default", 7},      {"delete", 6},
  {"do", 2},           {"double", 6},       {"dynamic_cast", 12},
  {"else", 4},         {"enum", 4},         {"explicit", 8},
  {"export", 6},       {"extern", 6},       {"false", 5},
  {"float", 5},        {"for", 3},          {"friend", 6},
  {"goto", 4},         {"if", 2},           {"inline", 6},
  {"int", 3},          {"long", 4},         {"mutable", 7},
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

static const ISN CodeKeywordCount =
  (ISN)(sizeof(CodeKeywordTable) / sizeof(CodeKeywordTable[0]));

/* Binary search the keyword table. Returns true if text[0..len) is
a known C++ keyword. Case-sensitive exact match. */
BOL CodeIsKeyword(const CHA* text, ISN len) {
  ISN lo = 0, hi = CodeKeywordCount - 1;
  while (lo <= hi) {
    ISN mid = (lo + hi) / 2;
    const TCodeKeyword& kw = CodeKeywordTable[mid];
    /* strcmp-style comparison: compare up to min(len), then
    resolve by length if all compared chars match. */
    ISN n = (len < (ISN)kw.len) ? len : (ISN)kw.len;
    ISN cmp = 0;
    for (ISN i = 0; i < n; ++i) {
      if (text[i] != kw.text[i]) {
        cmp = (text[i] < kw.text[i]) ? -1 : 1;
        break;
      }
    }
    if (cmp == 0) {
      /* All compared chars match — resolve by length. */
      if (len == (ISN)kw.len) return true;
      if (len < (ISN)kw.len) hi = mid - 1;
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
  return NILP;
}

/* Token names for diagnostics. */
static const CHA* CodeTokenNames[] = {
  "eof",            /* CODE_TOK_EOF */
  "identifier",     /* CODE_TOK_IDENT */
  "keyword",        /* CODE_TOK_KEYWORD */
  "int_literal",    /* CODE_TOK_INT */
  "float_literal",  /* CODE_TOK_FLOAT */
  "char_literal",   /* CODE_TOK_CHAR */
  "string_literal", /* CODE_TOK_STRING */
  "user_string",    /* CODE_TOK_USER_STRING */
  /* Punctuation: use spelling as name. */
  "punct", "punct", "punct", "punct", "punct", "punct",
  "punct", "punct", "punct", "punct", "punct", "punct",
  "punct", "punct", "punct", "punct", "punct", "punct",
  "punct", "punct", "punct", "punct", "punct", "punct",
  "punct", "punct", "punct", "punct", "punct", "punct",
  "punct", "punct", "punct", "punct", "punct", "punct",
  "punct", "punct", "punct", "punct", "punct", "punct",
  "punct", "punct", "punct", "punct", "punct", "punct",
  "punct", "punct", "punct", "punct", "punct", "punct",
  "punct", "punct", "punct", "punct", "punct", "punct",
  "line_comment",   /* CODE_TOK_COMMENT_LINE */
  "block_comment",  /* CODE_TOK_COMMENT_BLOCK */
  "space",          /* CODE_TOK_SPACE */
  "newline",        /* CODE_TOK_NEWLINE */
};

const CHA* CodeTokenName(IUB kind) {
  if (kind < CODE_TOK_COUNT && kind < (IUB)(sizeof(CodeTokenNames) / sizeof(CHA*))) {
    if (kind >= CodePunctFirst && kind <= CodePunctLast) {
      const CHA* sp = CodeTokenSpelling(kind);
      if (sp) return sp;
    }
    return CodeTokenNames[kind];
  }
  return "unknown";
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

ISN CodeLexerInit(TCodeLexer* lexer, const CHA* source, IUD source_len,
                  TCodeToken* tokens, ISN token_capacity) {
  if (IsError(lexer) || IsError(tokens) || token_capacity <= 0) return -1;
  if (IsError(source) && source_len > 0) return -1;
  lexer->source = source;
  lexer->source_len = source_len;
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

static void CodeRecordToken(TCodeLexer* lexer, IUB kind, ISN start_pos,
                            ISN start_line, ISN start_col) {
  if (lexer->token_count >= lexer->token_capacity) {
    lexer->full = true;
    return;
  }
  TCodeToken* tok = &lexer->tokens[lexer->token_count];
  tok->kind = kind;
  tok->byte_start = start_pos;
  tok->byte_end = lexer->pos;
  tok->line = start_line;
  tok->column = start_col;
  ++lexer->token_count;
}

/* --- Internal: skip whitespace, producing SPACE and NEWLINE tokens --- */

static void CodeSkipWhitespace(TCodeLexer* lexer) {
  while (lexer->pos < (ISN)lexer->source_len) {
    CHA c = lexer->source[lexer->pos];
    if (c == '\n' || c == '\r') {
      ISN start = lexer->pos;
      ISN start_line = lexer->line;
      ISN start_col = lexer->column;
      if (c == '\r' && lexer->pos + 1 < (ISN)lexer->source_len &&
          lexer->source[lexer->pos + 1] == '\n') {
        lexer->pos += 2;
      } else {
        ++lexer->pos;
      }
      ++lexer->line;
      lexer->column = 1;
      CodeRecordToken(lexer, CODE_TOK_NEWLINE, start, start_line, start_col);
    } else if (CodeIsSpace(c)) {
      ISN start = lexer->pos;
      ISN start_line = lexer->line;
      ISN start_col = lexer->column;
      while (lexer->pos < (ISN)lexer->source_len &&
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

static void CodeLexString(TCodeLexer* lexer, ISN start_pos, ISN start_line,
                          ISN start_col) {
  const CHA* src = lexer->source;
  IUD len = lexer->source_len;
  ISN pos = lexer->pos;

  /* Check for raw string: R"delim( ... )delim" */
  if (pos < (ISN)len && src[pos] == 'R' &&
      pos + 1 < (ISN)len && src[pos + 1] == '"') {
    /* Find the delimiter. */
    ISN delim_start = pos + 2;
    ISN delim_end = delim_start;
    while (delim_end < (ISN)len && src[delim_end] != '(') ++delim_end;
    if (delim_end < (ISN)len) {
      /* Skip to the matching )delim" */
      ++delim_end; /* skip ( */
      while (delim_end < (ISN)len) {
        if (src[delim_end] == ')' &&
            memcmp(src + delim_end + 1, src + delim_start,
                   delim_end - delim_start) == 0) {
          delim_end += (delim_end - delim_start) + 2; /* )delim" */
          break;
        }
        ++delim_end;
      }
      lexer->pos = (delim_end < (ISN)len) ? delim_end : (ISN)len;
      /* Update line/column. */
      for (ISN i = lexer->pos - (lexer->pos - start_pos); i < lexer->pos; ++i) {
        /* This is approximate; raw strings can span multiple lines. */
      }
      /* Recount lines. */
      lexer->line = start_line;
      lexer->column = start_col;
      for (ISN i = start_pos; i < lexer->pos && i < (ISN)len; ++i) {
        if (src[i] == '\n') { ++lexer->line; lexer->column = 1; }
        else ++lexer->column;
      }
      CodeRecordToken(lexer, CODE_TOK_STRING, start_pos, start_line, start_col);
      return;
    }
  }

  /* Normal string: skip to closing quote, handling escapes. */
  ++pos; /* skip opening quote */
  while (pos < (ISN)len && src[pos] != '"') {
    if (src[pos] == '\\' && pos + 1 < (ISN)len) {
      pos += 2; /* skip escape */
    } else {
      ++pos;
    }
  }
  if (pos < (ISN)len) ++pos; /* skip closing quote */
  lexer->pos = pos;
  lexer->column = start_col + (ISN)(pos - start_pos);
  CodeRecordToken(lexer, CODE_TOK_STRING, start_pos, start_line, start_col);
}

/* --- Internal: lex a character literal --- */

static void CodeLexChar(TCodeLexer* lexer, ISN start_pos, ISN start_line,
                        ISN start_col) {
  const CHA* src = lexer->source;
  IUD len = lexer->source_len;
  ISN pos = lexer->pos + 1; /* skip opening quote */
  while (pos < (ISN)len && src[pos] != '\'') {
    if (src[pos] == '\\' && pos + 1 < (ISN)len) pos += 2;
    else ++pos;
  }
  if (pos < (ISN)len) ++pos; /* skip closing quote */
  lexer->pos = pos;
  lexer->column = start_col + (ISN)(pos - start_pos);
  CodeRecordToken(lexer, CODE_TOK_CHAR, start_pos, start_line, start_col);
}

/* --- Internal: lex a number --- */

static void CodeLexNumber(TCodeLexer* lexer, ISN start_pos, ISN start_line,
                          ISN start_col) {
  const CHA* src = lexer->source;
  IUD len = lexer->source_len;
  ISN pos = lexer->pos;
  BOL is_float = false;

  /* Hex, octal, binary prefix. */
  if (src[pos] == '0' && pos + 1 < (ISN)len) {
    CHA next = src[pos + 1];
    if (next == 'x' || next == 'X') {
      pos += 2;
      while (pos < (ISN)len && CodeIsHexDigit(src[pos])) ++pos;
      /* Hex float: 0x1.8p3 */
      if (pos < (ISN)len && src[pos] == '.') {
        is_float = true;
        ++pos;
        while (pos < (ISN)len && CodeIsHexDigit(src[pos])) ++pos;
      }
      if (pos < (ISN)len && (src[pos] == 'p' || src[pos] == 'P')) {
        is_float = true;
        ++pos;
        if (pos < (ISN)len && (src[pos] == '+' || src[pos] == '-')) ++pos;
        while (pos < (ISN)len && CodeIsDigit(src[pos])) ++pos;
      }
    } else if (next >= '0' && next <= '7') {
      pos += 2;
      while (pos < (ISN)len && src[pos] >= '0' && src[pos] <= '7') ++pos;
    } else if (next == 'b' || next == 'B') {
      pos += 2;
      while (pos < (ISN)len && (src[pos] == '0' || src[pos] == '1')) ++pos;
    }
  } else {
    /* Decimal integer or float. */
    while (pos < (ISN)len && CodeIsDigit(src[pos])) ++pos;
    if (pos < (ISN)len && src[pos] == '.') {
      is_float = true;
      ++pos;
      while (pos < (ISN)len && CodeIsDigit(src[pos])) ++pos;
    }
    if (pos < (ISN)len && (src[pos] == 'e' || src[pos] == 'E')) {
      is_float = true;
      ++pos;
      if (pos < (ISN)len && (src[pos] == '+' || src[pos] == '-')) ++pos;
      while (pos < (ISN)len && CodeIsDigit(src[pos])) ++pos;
    }
  }

  /* Type suffix: u, U, l, L, ll, LL, f, F. */
  while (pos < (ISN)len) {
    CHA c = src[pos];
    if (c == 'u' || c == 'U' || c == 'l' || c == 'L' ||
        c == 'f' || c == 'F') {
      ++pos;
    } else {
      break;
    }
  }

  lexer->pos = pos;
  lexer->column = start_col + (ISN)(pos - start_pos);
  CodeRecordToken(lexer, is_float ? CODE_TOK_FLOAT : CODE_TOK_INT,
                  start_pos, start_line, start_col);
}

/* --- Internal: lex an identifier or keyword --- */

static void CodeLexIdent(TCodeLexer* lexer, ISN start_pos, ISN start_line,
                         ISN start_col) {
  const CHA* src = lexer->source;
  IUD len = lexer->source_len;
  ISN pos = lexer->pos;

  /* String literal prefixes: L" , u8" , u" , U" , R" */
  if (pos < (ISN)len && src[pos] == 'L' && pos + 1 < (ISN)len &&
      src[pos + 1] == '"') {
    lexer->pos = pos; /* let the string lexer handle it */
    CodeLexString(lexer, start_pos, start_line, start_col);
    return;
  }
  if (pos < (ISN)len && src[pos] == 'U' && pos + 1 < (ISN)len &&
      src[pos + 1] == '"') {
    lexer->pos = pos;
    CodeLexString(lexer, start_pos, start_line, start_col);
    return;
  }
  if (pos < (ISN)len && src[pos] == 'u' && pos + 1 < (ISN)len) {
    if (src[pos + 1] == '"') {
      lexer->pos = pos;
      CodeLexString(lexer, start_pos, start_line, start_col);
      return;
    }
    if (src[pos + 1] == '8' && pos + 2 < (ISN)len && src[pos + 2] == '"') {
      lexer->pos = pos;
      CodeLexString(lexer, start_pos, start_line, start_col);
      return;
    }
  }
  if (pos < (ISN)len && src[pos] == 'R' && pos + 1 < (ISN)len &&
      src[pos + 1] == '"') {
    lexer->pos = pos;
    CodeLexString(lexer, start_pos, start_line, start_col);
    return;
  }

  /* Consume identifier characters. */
  while (pos < (ISN)len && CodeIsIdentChar(src[pos])) ++pos;
  ISN ident_len = pos - start_pos;
  lexer->pos = pos;
  lexer->column = start_col + ident_len;

  IUB kind = CODE_TOK_IDENT;
  if (CodeIsKeyword(src + start_pos, ident_len)) kind = CODE_TOK_KEYWORD;

  /* User-defined string literal: "hello"_suffix */
  CodeRecordToken(lexer, kind, start_pos, start_line, start_col);

  /* Check for user-defined literal suffix on the NEXT token.
  The Fitter will handle "str"_lit as STRING + IDENT. */
}

/* --- Main lexer step --- */

IUB CodeLexerNext(TCodeLexer* lexer) {
  if (lexer->full) return CODE_TOK_EOF;

  /* Skip whitespace and comments. */
  while (lexer->pos < (ISN)lexer->source_len) {
    CHA c = lexer->source[lexer->pos];
    if (CodeIsSpace(c) || c == '\n' || c == '\r') {
      CodeSkipWhitespace(lexer);
      continue;
    }
    break;
  }

  if (lexer->pos >= (ISN)lexer->source_len) {
    CodeRecordToken(lexer, CODE_TOK_EOF, lexer->pos, lexer->line,
                    lexer->column);
    return CODE_TOK_EOF;
  }

  ISN start = lexer->pos;
  ISN start_line = lexer->line;
  ISN start_col = lexer->column;
  CHA c = lexer->source[lexer->pos];

  /* Comments. */
  if (c == '/' && lexer->pos + 1 < (ISN)lexer->source_len) {
    CHA next = lexer->source[lexer->pos + 1];
    if (next == '/') {
      /* Line comment: // ... \n */
      lexer->pos += 2;
      while (lexer->pos < (ISN)lexer->source_len &&
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
      while (lexer->pos + 1 < (ISN)lexer->source_len) {
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

  /* String / char literal (with optional prefix already consumed as ident). */
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

  /* Multi-char operators (longest match first). */
  const CHA* src = lexer->source;
  IUD len = lexer->source_len;

  /* Three-char: <<<, >>>, <<=, >>=, <=>, ... */
  if (lexer->pos + 2 < (ISN)len) {
    CHA c2 = src[lexer->pos + 1], c3 = src[lexer->pos + 2];
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

  /* Two-char operators. */
  if (lexer->pos + 1 < (ISN)len) {
    CHA c2 = src[lexer->pos + 1];
    IUB two_char = CODE_TOK_EOF;
    if (c == ':' && c2 == ':') two_char = CODE_TOK_DBL_COLON;
    else if (c == '.' && c2 == '*') two_char = CODE_TOK_DOT_STAR;
    else if (c == '-' && c2 == '>') {
      if (lexer->pos + 2 < (ISN)len && src[lexer->pos + 2] == '*') {
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
      if (lexer->pos + 2 < (ISN)len && src[lexer->pos + 2] == '=') {
        lexer->pos += 3; lexer->column += 3;
        CodeRecordToken(lexer, CODE_TOK_LSHIFT_ASSIGN, start, start_line, start_col);
        return CODE_TOK_LSHIFT_ASSIGN;
      }
      two_char = CODE_TOK_LSHIFT;
    }
    else if (c == '>' && c2 == '>') {
      if (lexer->pos + 2 < (ISN)len && src[lexer->pos + 2] == '=') {
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

  /* Single-char tokens. */
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

  /* Unknown character: record as IDENT (single char) to avoid
  crashing the pipeline. The Fitter will flag it. */
  ++lexer->pos;
  ++lexer->column;
  CodeRecordToken(lexer, CODE_TOK_IDENT, start, start_line, start_col);
  return CODE_TOK_IDENT;
}

}  //< namespace _
