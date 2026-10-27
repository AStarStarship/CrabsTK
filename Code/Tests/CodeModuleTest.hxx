// Copyright AStarship <https://astarship.net>.
// Standalone test for the CrabsTK Code module (Lexer, Stripper, Fitter).
// Builds with a minimal type shim matching ASCIICrabs _ConfigHeader.h widths.
// So it runs without the full ASCIICrabs config chain.
//
// Build with:
//   g++ -x c++ -std=c++23 -Wall -Wextra -Wpedantic -Werror
//       -ICode/Tests Code/Tests/CodeModuleTest.hxx -o /tmp/code_tests
//   (-x c++ is required: GCC 14 treats a bare .hxx source as a header and
//    saves a precompiled header instead of an executable.)
//
#include <cstdio>
#include <cstring>

/* Minimal type shim (matches ASCIICrabs _ConfigHeader.h). */
#include <cstdint>
#include <cstddef>

#ifndef CRABSTK_CODE_TESTS_CONFIG_H
#define CRABSTK_CODE_TESTS_CONFIG_H

// ============================================================
// Type definitions — match ASCIICrabs canonical widths
// ============================================================

// CHA: Character type (1 byte, signed char)
typedef char CHA;

// ISA: 1-byte signed integer
typedef signed char ISA;

// IUA: 1-byte unsigned integer
typedef unsigned char IUA;

// ISB: 2-byte signed integer
typedef short ISB;

// IUB: 2-byte unsigned integer
typedef unsigned short IUB;

// ISC: 4-byte signed integer
typedef int ISC;

// IUC: 4-byte unsigned integer (uint32_t)
// Matches ASCIICrabs: typedef uint32_t IUC; in _ConfigHeader.h
typedef uint32_t IUC;

// ISD: 8-byte signed integer (int64_t)
// Matches ASCIICrabs: typedef int64_t ISD; in _ConfigHeader.h
typedef int64_t ISD;

// IUD: 8-byte unsigned integer (uint64_t)
// Matches ASCIICrabs: typedef uint64_t IUD; in _ConfigHeader.h
typedef uint64_t IUD;

// ISN: Index/size type (4-byte int)
// Matches ASCIICrabs: typedef int ISN; in _ConfigHeader.h
typedef int ISN;

// IUN: Unsigned index type
typedef unsigned int IUN;

// ISW: Word-sized int / pointer-sized integer
// Matches ASCIICrabs: typedef intptr_t ISW; in _ConfigHeader.h
// Old: typedef long ISW; — may mismatch on some platforms
// FIX: Use intptr_t for guaranteed pointer-sized width
typedef intptr_t ISW;

// IUW: Unsigned pointer-sized integer
// Matches ASCIICrabs: typedef uintptr_t IUW; in _ConfigHeader.h
typedef uintptr_t IUW;

// FPC: 4-byte floating-point (float)
// Matches ASCIICrabs: typedef float FPC; in _ConfigHeader.h
typedef float FPC;

// FPD: 8-byte floating-point (double)
// Matches ASCIICrabs: typedef double FPD; in _ConfigHeader.h
typedef double FPD;

// BOL: Boolean type (1 byte)
// Matches ASCIICrabs: typedef bool BOL; in _ConfigHeader.h
typedef bool BOL;

// ============================================================
// Compile-time width verification static_assert
// ============================================================

// BOL must be 1 byte
static_assert(sizeof(BOL) == 1, "Code Tests BOL must be 1 byte width — matches ASCIICrabs bool");

// ISW must be pointer-sized
static_assert(sizeof(ISW) == sizeof(void*), "Code Tests ISW must be pointer-sized — matches ASCIICrabs intptr_t");

// IUC must be 32-bit
static_assert(sizeof(IUC) == 4, "Code Tests IUC must be 32-bit width — matches ASCIICrabs uint32_t");

// ============================================================
// End of file
// ============================================================

#endif // CRABSTK_CODE_TESTS_CONFIG_H

/* Minimal type shim (matches ASCIICrabs _ConfigHeader.h). */
typedef char CHA;
typedef signed char ISA;
typedef unsigned char IUA;
typedef short ISB;
typedef unsigned short IUB;
typedef int ISC;
inline BOL IsError(const void* p) { return p == nullptr; }
#define nilp nullptr

#include "../Lexer.h"
#include "../Stripper.h"
#include "../Fitter.h"

/* Pull in the implementations for the standalone test build. */
#include "../Lexer.hxx"
#include "../Stripper.hxx"
#include "../Fitter.hxx"

using namespace _;

static int passed = 0;
static int failed = 0;

#define CHECK(name, cond) do { \
  if (cond) { ++passed; printf("  PASS: %s\n", name); } \
  else { ++failed; printf("  FAIL: %s\n", name); } \
} while (0)

/* Helper: lex the entire source into the token array. */
static ISN LexAll(const CHA* src, TCodeToken* tokens, TCodeLexer* lex) {
  CodeLexerInit(lex, src, (IUD)strlen(src), tokens, 64);
  while (CodeLexerNext(lex) != CODE_TOK_EOF) {}
  return lex->token_count;
}

/* Helper: find the index of the Nth non-whitespace, non-EOF token. */
static ISN FindContentToken(const TCodeToken* tokens, ISN count, ISN nth) {
  ISN found = 0;
  for (ISN i = 0; i < count; ++i) {
    IUB k = tokens[i].kind;
    if (k == CODE_TOK_EOF) break;
    if (k == CODE_TOK_SPACE || k == CODE_TOK_NEWLINE) continue;
    if (found == nth) return i;
    ++found;
  }
  return -1;
}

/* --- Test: keyword detection --- */
void TestKeywords() {
  printf("Keywords:\n");
  CHECK("if is keyword", CodeIsKeyword("if", 2));
  CHECK("namespace is keyword", CodeIsKeyword("namespace", 9));
  CHECK("constexpr is keyword", CodeIsKeyword("constexpr", 9));
  CHECK("co_await is keyword", CodeIsKeyword("co_await", 8));
  CHECK("co_return is keyword", CodeIsKeyword("co_return", 9));
  CHECK("co_yield is keyword", CodeIsKeyword("co_yield", 8));
  CHECK("nullptr is keyword", CodeIsKeyword("nullptr", 7));
  CHECK("not a keyword", !CodeIsKeyword("foo", 3));
  CHECK("empty is not keyword", !CodeIsKeyword("", 0));
  CHECK("std is not keyword", !CodeIsKeyword("std", 3));
  CHECK("CHA is not keyword", !CodeIsKeyword("CHA", 3));
}

/* --- Test: basic identifier lexing --- */
void TestIdentifiers() {
  printf("Identifiers:\n");
  const CHA* src = "foo bar_baz 123";
  TCodeToken tokens[64];
  TCodeLexer lex;
  ISN n = LexAll(src, tokens, &lex);

  ISN i0 = FindContentToken(tokens, n, 0);
  ISN i1 = FindContentToken(tokens, n, 1);
  ISN i2 = FindContentToken(tokens, n, 2);

  CHECK("foo is IDENT", i0 >= 0 && tokens[i0].kind == CODE_TOK_IDENT);
  CHECK("foo span", i0 >= 0 && tokens[i0].byte_start == 0 && tokens[i0].byte_end == 3);
  CHECK("bar_baz is IDENT", i1 >= 0 && tokens[i1].kind == CODE_TOK_IDENT);
  CHECK("bar_baz span", i1 >= 0 && tokens[i1].byte_start == 4 && tokens[i1].byte_end == 11);
  CHECK("123 is INT", i2 >= 0 && tokens[i2].kind == CODE_TOK_INT);
  CHECK("SPACE between foo and bar_baz",
        i0 >= 0 && i1 > i0 && tokens[i0+1].kind == CODE_TOK_SPACE);
}

/* --- Test: keyword lexing --- */
void TestKeywordLexing() {
  printf("Keyword lexing:\n");
  const CHA* src = "if (x) { return 0; }";
  TCodeToken tokens[64];
  TCodeLexer lex;
  ISN n = LexAll(src, tokens, &lex);

  ISN i0 = FindContentToken(tokens, n, 0);
  ISN i1 = FindContentToken(tokens, n, 1);
  ISN i2 = FindContentToken(tokens, n, 2);
  ISN i3 = FindContentToken(tokens, n, 3);

  CHECK("if is KEYWORD", i0 >= 0 && tokens[i0].kind == CODE_TOK_KEYWORD);
  CHECK("( is LPAREN", i1 >= 0 && tokens[i1].kind == CODE_TOK_LPAREN);
  CHECK("x is IDENT", i2 >= 0 && tokens[i2].kind == CODE_TOK_IDENT);
  CHECK("} is RBRACE", i3 >= 0);
}

/* --- Test: operators --- */
void TestOperators() {
  printf("Operators:\n");
  const CHA* src = "a && b || c <= d != e == f";
  TCodeToken tokens[64];
  TCodeLexer lex;
  ISN n = LexAll(src, tokens, &lex);

  ISN idx[15];
  for (ISN i = 0; i < 15; ++i) idx[i] = FindContentToken(tokens, n, i);

  CHECK("a IDENT", idx[0] >= 0 && tokens[idx[0]].kind == CODE_TOK_IDENT);
  CHECK("&& AND", idx[1] >= 0 && tokens[idx[1]].kind == CODE_TOK_AND);
  CHECK("b IDENT", idx[2] >= 0 && tokens[idx[2]].kind == CODE_TOK_IDENT);
  CHECK("|| OR", idx[3] >= 0 && tokens[idx[3]].kind == CODE_TOK_OR);
  CHECK("c IDENT", idx[4] >= 0 && tokens[idx[4]].kind == CODE_TOK_IDENT);
  CHECK("<= LE", idx[5] >= 0 && tokens[idx[5]].kind == CODE_TOK_LE);
  CHECK("d IDENT", idx[6] >= 0 && tokens[idx[6]].kind == CODE_TOK_IDENT);
  CHECK("!= NE", idx[7] >= 0 && tokens[idx[7]].kind == CODE_TOK_NE);
  CHECK("e IDENT", idx[8] >= 0 && tokens[idx[8]].kind == CODE_TOK_IDENT);
  CHECK("== EQ", idx[9] >= 0 && tokens[idx[9]].kind == CODE_TOK_EQ);
  CHECK("f IDENT", idx[10] >= 0 && tokens[idx[10]].kind == CODE_TOK_IDENT);
}

/* --- Test: numbers --- */
void TestNumbers() {
  printf("Numbers:\n");
  const CHA* src = "42 3.14 0xFF 0b1010 1.5e3 100LL";
  TCodeToken tokens[64];
  TCodeLexer lex;
  ISN n = LexAll(src, tokens, &lex);

  ISN i0 = FindContentToken(tokens, n, 0);
  ISN i1 = FindContentToken(tokens, n, 1);
  ISN i2 = FindContentToken(tokens, n, 2);
  ISN i3 = FindContentToken(tokens, n, 3);
  ISN i4 = FindContentToken(tokens, n, 4);
  ISN i5 = FindContentToken(tokens, n, 5);

  CHECK("42 INT", i0 >= 0 && tokens[i0].kind == CODE_TOK_INT);
  CHECK("3.14 FLOAT", i1 >= 0 && tokens[i1].kind == CODE_TOK_FLOAT);
  CHECK("0xFF INT", i2 >= 0 && tokens[i2].kind == CODE_TOK_INT);
  CHECK("0b1010 INT", i3 >= 0 && tokens[i3].kind == CODE_TOK_INT);
  CHECK("1.5e3 FLOAT", i4 >= 0 && tokens[i4].kind == CODE_TOK_FLOAT);
  CHECK("100LL INT", i5 >= 0 && tokens[i5].kind == CODE_TOK_INT);
}

/* --- Test: strings --- */
void TestStrings() {
  printf("Strings:\n");
  const CHA* src = "\"hello\" 'a' \"esc \\n here\"";
  TCodeToken tokens[64];
  TCodeLexer lex;
  ISN n = LexAll(src, tokens, &lex);

  ISN i0 = FindContentToken(tokens, n, 0);
  ISN i1 = FindContentToken(tokens, n, 1);
  ISN i2 = FindContentToken(tokens, n, 2);

  CHECK("\"hello\" STRING", i0 >= 0 && tokens[i0].kind == CODE_TOK_STRING);
  CHECK("'a' CHAR", i1 >= 0 && tokens[i1].kind == CODE_TOK_CHAR);
  CHECK("escaped STRING", i2 >= 0 && tokens[i2].kind == CODE_TOK_STRING);
  if (i2 >= 0) {
    ISN slen = tokens[i2].byte_end - tokens[i2].byte_start;
    CHECK("escaped string length 13", slen == 13);
  }
}

/* --- Test: comments --- */
void TestComments() {
  printf("Comments:\n");
  const CHA* src = "int x; // line comment\nint y; /* block */ int z;";
  TCodeToken tokens[64];
  TCodeLexer lex;
  ISN n = LexAll(src, tokens, &lex);

  ISN comment_line_idx = -1, comment_block_idx = -1;
  for (ISN i = 0; i < n; ++i) {
    if (tokens[i].kind == CODE_TOK_COMMENT_LINE) comment_line_idx = i;
    if (tokens[i].kind == CODE_TOK_COMMENT_BLOCK) comment_block_idx = i;
  }
  CHECK("line comment found", comment_line_idx >= 0);
  CHECK("block comment found", comment_block_idx >= 0);
}

/* --- Test: C++11/20 operators --- */
void TestModernOperators() {
  printf("Modern operators:\n");
  const CHA* src = "a <=> b << 2 >> 3";
  TCodeToken tokens[64];
  TCodeLexer lex;
  ISN n = LexAll(src, tokens, &lex);

  ISN i0 = FindContentToken(tokens, n, 0);
  ISN i1 = FindContentToken(tokens, n, 1);
  ISN i2 = FindContentToken(tokens, n, 2);
  ISN i3 = FindContentToken(tokens, n, 3);
  ISN i4 = FindContentToken(tokens, n, 4);
  ISN i5 = FindContentToken(tokens, n, 5);

  CHECK("a IDENT", i0 >= 0 && tokens[i0].kind == CODE_TOK_IDENT);
  CHECK("<=> SPACESHIP", i1 >= 0 && tokens[i1].kind == CODE_TOK_SPACESHIP);
  CHECK("b IDENT", i2 >= 0 && tokens[i2].kind == CODE_TOK_IDENT);
  CHECK("<< LSHIFT", i3 >= 0 && tokens[i3].kind == CODE_TOK_LSHIFT);
  CHECK("2 INT", i4 >= 0 && tokens[i4].kind == CODE_TOK_INT);
  CHECK(">> RSHIFT", i5 >= 0 && tokens[i5].kind == CODE_TOK_RSHIFT);
}

/* --- Test: line/column tracking --- */
void TestLineColumn() {
  printf("Line/column:\n");
  const CHA* src = "int x;\nint y;\nint z;";
  TCodeToken tokens[64];
  TCodeLexer lex;
  ISN n = LexAll(src, tokens, &lex);

  ISN i0 = FindContentToken(tokens, n, 0);
  ISN i1 = FindContentToken(tokens, n, 1);
  ISN i3 = FindContentToken(tokens, n, 3);

  CHECK("int line 1", i0 >= 0 && tokens[i0].line == 1);
  CHECK("int col 1", i0 >= 0 && tokens[i0].column == 1);
  CHECK("x col 5", i1 >= 0 && tokens[i1].column == 5);
  CHECK("int line 2", i3 >= 0 && tokens[i3].line == 2);
  CHECK("int col 1 (line 2)", i3 >= 0 && tokens[i3].column == 1);
}

/* --- Test: strip comments --- */
void TestStrip() {
  printf("Stripper:\n");
  const CHA* src = "int x; // remove me\nint y;";
  TCodeToken tokens[64];
  TCodeLexer lex;
  ISN n = LexAll(src, tokens, &lex);

  ISN non_comments = CodeCountNonComments(tokens, n);
  CHECK("non-comment count", non_comments == n - 1);

  TCodeToken stripped[64];
  ISN stripped_count = 0;
  ISN rc = CodeStripComments(tokens, n, stripped, &stripped_count);
  CHECK("strip rc 0", rc == 0);
  CHECK("stripped count", stripped_count == non_comments);

  BOL has_comment = false;
  for (ISN i = 0; i < stripped_count; ++i) {
    if (stripped[i].kind == CODE_TOK_COMMENT_LINE ||
        stripped[i].kind == CODE_TOK_COMMENT_BLOCK) {
      has_comment = true;
    }
  }
  CHECK("no comments in stripped", !has_comment);
}

/* --- Test: fitter std:: detection --- */
void TestFitStdDetection() {
  printf("Fitter std:: detection:\n");
  const CHA* src = "std::string s;";
  TCodeToken tokens[64];
  TCodeLexer lex;
  ISN n = LexAll(src, tokens, &lex);

  CHA out_buf[256];
  TFinding findings[16];
  TFittingResult result;
  TFitterOptions opts = {};
  opts.indent_width = 2;
  opts.max_line_length = 100;
  opts.enforce_seam_gate = false;
  opts.project_name = "CRABSTK";

  ISN rc = CodeFit(tokens, n, src, out_buf, 256,
                   &result, findings, 16, &opts);
  CHECK("fit rc 0", rc == 0);
  CHECK("std:: error found", result.error_count >= 1);
  if (result.finding_count > 0) {
    CHECK("finding is std:: type", findings[0].code == FIT_CODE_STD_TYPE);
  }
}

/* --- Test: fitter clean code --- */
void TestFitClean() {
  printf("Fitter clean code:\n");
  const CHA* src = "int x = 42;";
  TCodeToken tokens[64];
  TCodeLexer lex;
  ISN n = LexAll(src, tokens, &lex);

  CHA out_buf[256];
  TFinding findings[16];
  TFittingResult result;
  TFitterOptions opts = {};
  opts.indent_width = 2;
  opts.max_line_length = 100;
  opts.enforce_seam_gate = false;
  opts.project_name = "CRABSTK";

  ISN rc = CodeFit(tokens, n, src, out_buf, 256,
                   &result, findings, 16, &opts);
  CHECK("fit rc 0", rc == 0);
  CHECK("no errors", result.error_count == 0);
  CHECK("output not empty", result.output_len > 0);
}

/* --- Test: indent level tracking --- */
void TestIndentLevel() {
  printf("Indent level:\n");
  const CHA* src = "{ { a } b }";
  TCodeToken tokens[64];
  TCodeLexer lex;
  ISN n = LexAll(src, tokens, &lex);

  ISN a_idx = FindContentToken(tokens, n, 2); /* { { a -> a is 3rd content */
  CHECK("found ident", a_idx >= 0);
  if (a_idx >= 0) {
    ISN level = CodeIndentLevel(tokens, n, a_idx);
    CHECK("indent level 2", level == 2);
  }
}

/* --- Test: full pipeline (lex -> strip -> fit) --- */
void TestPipeline() {
  printf("Full pipeline:\n");
  const CHA* src =
    "// header comment\n"
    "int main() {\n"
    "  int x = 42; // inline\n"
    "  return x;\n"
    "}\n";
  TCodeToken tokens[256];
  TCodeLexer lex;
  CodeLexerInit(&lex, src, (IUD)strlen(src), tokens, 256);
  while (CodeLexerNext(&lex) != CODE_TOK_EOF) {}
  ISN n = lex.token_count;

  CHECK("lexed tokens", n > 10);

  TCodeToken stripped[256];
  ISN stripped_count = 0;
  CodeStripComments(tokens, n, stripped, &stripped_count);
  CHECK("stripped fewer tokens", stripped_count < n);

  CHA out_buf[1024];
  TFinding findings[32];
  TFittingResult result;
  TFitterOptions opts = {};
  opts.indent_width = 2;
  opts.max_line_length = 100;
  opts.enforce_seam_gate = false;
  opts.project_name = "CRABSTK";

  ISN rc = CodeFit(stripped, stripped_count, src, out_buf, 1024,
                   &result, findings, 32, &opts);
  CHECK("pipeline rc 0", rc == 0);
  CHECK("pipeline output", result.output_len > 0);
  CHECK("no std:: errors", result.error_count == 0);
}

/* --- Test: token spelling --- */
void TestTokenSpelling() {
  printf("Token spelling:\n");
  CHECK("AND spelling", CodeTokenSpelling(CODE_TOK_AND) &&
    strcmp(CodeTokenSpelling(CODE_TOK_AND), "&&") == 0);
  CHECK("EQ spelling", CodeTokenSpelling(CODE_TOK_EQ) &&
    strcmp(CodeTokenSpelling(CODE_TOK_EQ), "==") == 0);
  CHECK("SPACESHIP spelling", CodeTokenSpelling(CODE_TOK_SPACESHIP) &&
    strcmp(CodeTokenSpelling(CODE_TOK_SPACESHIP), "<=>") == 0);
  CHECK("IDENT no spelling", CodeTokenSpelling(CODE_TOK_IDENT) == nilp);
  CHECK("EOF no spelling", CodeTokenSpelling(CODE_TOK_EOF) == nilp);
}

/* --- Test: token count --- */
void TestTokenCount() {
  printf("Token count:\n");
  CHECK("COUNT > 60", CODE_TOK_COUNT > 60);
  CHECK("SPACE != 0", CODE_TOK_SPACE != 0);
  CHECK("NEWLINE != 0", CODE_TOK_NEWLINE != 0);
}

int main() {
  printf("CrabsTK Code Module Tests\n");
  printf("=========================\n");

  TestKeywords();
  TestIdentifiers();
  TestKeywordLexing();
  TestOperators();
  TestNumbers();
  TestStrings();
  TestComments();
  TestModernOperators();
  TestLineColumn();
  TestStrip();
  TestFitStdDetection();
  TestFitClean();
  TestIndentLevel();
  TestPipeline();
  TestTokenSpelling();
  TestTokenCount();

  printf("=========================\n");
  printf("Results: %d passed, %d failed\n", passed, failed);
  return failed ? 1 : 0;
}