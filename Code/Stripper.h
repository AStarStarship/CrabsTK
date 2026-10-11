// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef CRABSTK_CODE_STRIPPER_H
#define CRABSTK_CODE_STRIPPER_H

/* CrabsTK Code Stripper — comment stripper on the token stream.
Works on the output of the Lexer (Code/Lexer.h). Strips // and
block comments from the token array, producing a clean token stream
that the Fitter can reformat. Round-trip safe: strip, re-lex,
identical non-comment tokens.

No allocation, no std::, no stb. The output token array is
caller-owned. */

namespace _ {

/* Strip comments from a token array.
tokens:       Input token array (from CodeLexer).
token_count:  Number of tokens in the input.
out_tokens:   Output token array (caller-owned, capacity >= token_count).
out_count:    Filled with the number of output tokens.
Returns 0 on success, -1 on bad arguments.
Comment tokens (CODE_TOK_COMMENT_LINE, CODE_TOK_COMMENT_BLOCK) are
removed. All other tokens are preserved in order. Whitespace tokens
adjacent to a removed comment are merged or dropped to avoid
double-spacing. */
ISN CodeStripComments(const TCodeToken* tokens, ISN token_count,
                      TCodeToken* out_tokens, ISN* out_count);

/* Count the number of non-comment tokens in a token array.
Useful for sizing the output buffer before calling CodeStripComments. */
ISN CodeCountNonComments(const TCodeToken* tokens, ISN token_count);

}  //< namespace _
#endif
