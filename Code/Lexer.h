// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef CRABSTK_CODE_LEXER_H
#define CRABSTK_CODE_LEXER_H

/* CrabsTK Code Lexer — in-house C++ lexer on the no-stdlib type system.
Token taxonomy is derived from Clang's TokenKinds.def (BSD license,
see LICENSE.Clang). This is the first stage of the Chimera+ tooling
pipeline: bytes in, token stream out. No allocation, no std::, no stb.
The Stripper, Fitter, and (later) LSP consume the token stream. */

namespace _ {

/* Token kinds. Values are stable — once assigned, never renumbered.
The Fitter and LSP depend on these values. New kinds are appended
at the end before CODE_TOK_COUNT. */
enum ECodeToken : IUB {
  CODE_TOK_EOF = 0,

  /* Literals */
  CODE_TOK_IDENT,         //< [A-Za-z_][A-Za-z0-9_]*
  CODE_TOK_KEYWORD,       //< C++ keyword (see CodeKeywordTable).
  CODE_TOK_INT,           //< 42, 0xFF, 0b1010, 123u, 456789012345LL
  CODE_TOK_FLOAT,         //< 3.14, 2.5e-3, 1.f, 0x1.8p3
  CODE_TOK_CHAR,          //< 'a', '\n', '\x41', L'w', u8'b'
  CODE_TOK_STRING,        //< "hello", L"wide", R"raw(raw)"
  CODE_TOK_USER_STRING,   //< "hello"_my_literal

  /* Punctuation */
  CODE_TOK_LPAREN,        //< (
  CODE_TOK_RPAREN,        //< )
  CODE_TOK_LBRACKET,      //< [
  CODE_TOK_RBRACKET,      //< ]
  CODE_TOK_LBRACE,        //< {
  CODE_TOK_RBRACE,        //< }
  CODE_TOK_SEMICOLON,     //< ;
  CODE_TOK_COLON,         //< :
  CODE_TOK_DBL_COLON,     //< ::
  CODE_TOK_COMMA,         //< ,
  CODE_TOK_DOT,           //< .
  CODE_TOK_DOT_STAR,      //< .*
  CODE_TOK_ARROW,         //< ->
  CODE_TOK_ARROW_STAR,    //< ->*
  CODE_TOK_ELLIPSIS,      //< ...
  CODE_TOK_QUESTION,      //< ?
  CODE_TOK_AT,            //< @ (for future attribute syntax)

  /* Operators */
  CODE_TOK_ASSIGN,        //< =
  CODE_TOK_PLUS,          //< +
  CODE_TOK_MINUS,         //< -
  CODE_TOK_STAR,          //< *
  CODE_TOK_SLASH,         //< /
  CODE_TOK_PERCENT,       //< %
  CODE_TOK_AMP,           //< &
  CODE_TOK_PIPE,          //< |
  CODE_TOK_CARET,         //< ^
  CODE_TOK_TILDE,         //< ~
  CODE_TOK_BANG,          //< !
  CODE_TOK_LT,            //< <
  CODE_TOK_GT,            //< >
  CODE_TOK_LE,            //< <=
  CODE_TOK_GE,            //< >=
  CODE_TOK_EQ,            //< ==
  CODE_TOK_NE,            //< !=
  CODE_TOK_AND,           //< &&
  CODE_TOK_OR,            //< ||
  CODE_TOK_INC,           //< ++
  CODE_TOK_DEC,           //< --
  CODE_TOK_LSHIFT,        //< <<
  CODE_TOK_RSHIFT,        //< >>
  CODE_TOK_LSHIFT3,       //< <<< (user-defined)
  CODE_TOK_RSHIFT3,       //< >>> (user-defined)
  CODE_TOK_SPACESHIP,     //< <=> (C++20)
  CODE_TOK_CARET_CARET,   //< ^^ (C++20 bitwise)
  CODE_TOK_PLUS_ASSIGN,   //< +=
  CODE_TOK_MINUS_ASSIGN,  //< -=
  CODE_TOK_STAR_ASSIGN,   //< *=
  CODE_TOK_SLASH_ASSIGN,  //< /=
  CODE_TOK_PERCENT_ASSIGN, //< %=
  CODE_TOK_AMP_ASSIGN,    //< &=
  CODE_TOK_PIPE_ASSIGN,   //< |=
  CODE_TOK_CARET_ASSIGN,  //< ^=
  CODE_TOK_LSHIFT_ASSIGN, //< <<=
  CODE_TOK_RSHIFT_ASSIGN, //< >>=

  /* Preprocessor */
  CODE_TOK_HASH,          //< #
  CODE_TOK_HASH_HASH,     //< ##
  CODE_TOK_HASH_AT,       //< #@

  /* Comments (preserved for round-trip fidelity) */
  CODE_TOK_COMMENT_LINE,  //< // ... \n
  CODE_TOK_COMMENT_BLOCK, //< /* ... */

  /* Whitespace (preserved for round-trip fidelity) */
  CODE_TOK_SPACE,         //< horizontal whitespace
  CODE_TOK_NEWLINE,       //< \n or \r\n

  CODE_TOK_COUNT          //< sentinel: number of token kinds.
};

/* A single token: kind, byte span into the source, and 1-based
line/column for diagnostics. The text is source + byte_start ..
byte_end (caller reads it, no copy). */
struct TCodeToken {
  IUB kind;         //< ECodeToken.
  ISN byte_start;   //< 0-based offset into source.
  ISN byte_end;     //< 0-based, exclusive.
  ISN line;         //< 1-based line number.
  ISN column;       //< 1-based byte column from line start.
};

/* Token span: a half-open range [first, last) into a token array.
Used by the Fitter to select regions for reformatting. */
struct TCodeSpan {
  ISN first;
  ISN last;
};

/* Lexer state. The source pointer is read-only; the token array is
caller-owned. No heap allocation. The lexer tracks line/column so
diagnostics can point at the right position. */
struct TCodeLexer {
  const CHA* source;
  IUD source_len;
  ISN pos;              //< Current byte offset.
  ISN line;             //< Current line (1-based).
  ISN column;           //< Current column (1-based).
  TCodeToken* tokens;   //< Caller-owned array.
  ISN token_count;      //< Tokens produced so far.
  ISN token_capacity;   //< Capacity of the tokens array.
  BOL full;             //< True when tokens array is full (next token dropped).
};

/* Initialize the lexer. The token array must be pre-allocated.
Returns 0 on success, -1 on bad arguments. */
ISN CodeLexerInit(TCodeLexer* lexer, const CHA* source, IUD source_len,
                  TCodeToken* tokens, ISN token_capacity);

/* Advance the lexer by one token. Writes the token into
tokens[token_count] and increments token_count. Returns the token
kind, or CODE_TOK_EOF at end of source. If the token array is full,
sets full=true and returns CODE_TOK_EOF without advancing. */
IUB CodeLexerNext(TCodeLexer* lexer);

/* Check if a string is a C++ keyword. Case-sensitive, exact match.
Used by CodeLexerNext to classify ident vs keyword. */
BOL CodeIsKeyword(const CHA* text, ISN len);

/* Get the spelling of a punctuation token (e.g. "&&" for CODE_TOK_AND).
Returns nilp for non-punctuation tokens. */
const CHA* CodeTokenSpelling(IUB kind);

/* Token kind name for diagnostics (e.g. "keyword", "int_literal").
Returns a static string, never null. */
const CHA* CodeTokenName(IUB kind);

}  //< namespace _
#endif
