// Copyright AStarship <https://astarship.net>.
#include "_Config.h"
#include "Stripper.h"

namespace _ {

ISN CodeCountNonComments(const TCodeToken* tokens, ISN token_count) {
  if (IsError(tokens) || token_count <= 0) return 0;
  ISN count = 0;
  for (ISN i = 0; i < token_count; ++i) {
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
static void CodeMergeWhitespace(TCodeToken* tokens, ISN* count) {
  ISN write = 0;
  for (ISN i = 0; i < *count; ++i) {
    TCodeToken* tok = &tokens[i];

    /* Drop SPACE after NEWLINE. */
    if (tok->kind == CODE_TOK_SPACE && write > 0 &&
        tokens[write - 1].kind == CODE_TOK_NEWLINE) {
      continue;
    }

    /* Merge consecutive SPACE tokens. */
    if (tok->kind == CODE_TOK_SPACE && write > 0 &&
        tokens[write - 1].kind == CODE_TOK_SPACE) {
      continue;
    }

    if (write != i) {
      tokens[write] = *tok;
    }
    ++write;
  }
  *count = write;
}

ISN CodeStripComments(const TCodeToken* tokens, ISN token_count,
                      TCodeToken* out_tokens, ISN* out_count) {
  if (IsError(tokens) || IsError(out_tokens) || IsError(out_count) ||
      token_count <= 0) {
    return -1;
  }

  ISN write = 0;
  for (ISN i = 0; i < token_count; ++i) {
    const TCodeToken* tok = &tokens[i];
    if (tok->kind == CODE_TOK_COMMENT_LINE ||
        tok->kind == CODE_TOK_COMMENT_BLOCK) {
      continue;
    }
    out_tokens[write] = *tok;
    ++write;
  }

  CodeMergeWhitespace(out_tokens, &write);
  *out_count = write;
  return 0;
}

}  //< namespace _
