// Copyright AStarship <https://astarship.net>.
#include <_Config.h>
#include "Stripper.h"
namespace CTK {
namespace Code {

ISC CodeCountNonComments(const TCodeToken* tokens, ISC token_count) {
  if (IsError(tokens) || token_count <= 0) return 0;
  ISC count = 0;
  for (ISC i = 0; i < token_count; ++i) {
    if (tokens[i].kind != CODE_TOK_COMMENT_LINE &&
        tokens[i].kind != CODE_TOK_COMMENT_BLOCK) {
      ++count;
    }
  }
  return count;
}

/* Merge adjacent whitespace: if two consecutive SPACE tokens appear
in the output, collapse them into one. Also drop a SPACE token that
immediately follows a NEWLINE (the newline already separates the
lines). This keeps the output clean without reformatting. */
static void CodeMergeWhitespace(TCodeToken* tokens, ISC* count) {
  ISC write = 0;
  for (ISC i = 0; i < *count; ++i) {
    TCodeToken* token = &tokens[i];

    /* Drop SPACE after NEWLINE. */
    if (token->kind == CODE_TOK_SPACE && write > 0 &&
        tokens[write - 1].kind == CODE_TOK_NEWLINE) {
      continue;
    }

    /* Merge consecutive SPACE tokens. */
    if (token->kind == CODE_TOK_SPACE && write > 0 &&
        tokens[write - 1].kind == CODE_TOK_SPACE) {
      continue;
    }

    if (write != i) {
      tokens[write] = *token;
    }
    ++write;
  }
  *count = write;
}

ISC CodeStripComments(const TCodeToken* tokens, ISC token_count,
                      TCodeToken* out_tokens, ISC* out_count) {
  if (IsError(tokens) || IsError(out_tokens) || IsError(out_count) ||
      token_count <= 0) {
    return -1;
  }

  ISC write = 0;
  for (ISC i = 0; i < token_count; ++i) {
    const TCodeToken* token = &tokens[i];
    if (token->kind == CODE_TOK_COMMENT_LINE ||
        token->kind == CODE_TOK_COMMENT_BLOCK) {
      continue;
    }
    out_tokens[write] = *token;
    ++write;
  }

  CodeMergeWhitespace(out_tokens, &write);
  *out_count = write;
  return 0;
}

}  //< namespace Code
}  //< namespace CTK
