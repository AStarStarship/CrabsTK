// Copyright AStarship <https://astarship.net>.
#include <_Config.h>
#include <stdio.h>
#include <stdlib.h>
#include "Lexer.h"
#include "CommentStripper.h"
namespace CTK {
namespace Code {

/* Read a whole file into a NUL-terminated buffer. Caller frees.
@return Buffer, or nilp on error. */
static CHA* CodeReadFile(const CHA* path, ISC* byte_count) {
  FILE* file = fopen(path, "rb");
  if (!file) return _NilP;
  if (fseek(file, 0, SEEK_END) != 0) { fclose(file); return _NilP; }
  const long size = ftell(file);
  if (size < 0) { fclose(file); return _NilP; }
  if (fseek(file, 0, SEEK_SET) != 0) { fclose(file); return _NilP; }
  CHA* buffer = (CHA*)malloc((IUC)size + 1);
  if (!buffer) { fclose(file); return _NilP; }
  if (fread(buffer, 1, (IUC)size, file) != (IUC)size) {
    free(buffer); fclose(file); return _NilP;
  }
  fclose(file);
  buffer[size] = 0;
  *byte_count = ISC(size);
  return buffer;
}

static BOL CodeIsCommentKind(IUB kind) {
  return kind == CODE_TOK_COMMENT_LINE || kind == CODE_TOK_COMMENT_BLOCK;
}

static void CodePutChar(CHA c, ISC tab_space_count) {
  if (c == '\t') {
    for (ISC i = tab_space_count; i > 0; --i) fputc(' ', stdout);
  } else {
    fputc(c, stdout);
  }
}

/* Emit the source with comment bytes removed, line by line. */
static void CodeEmitStripped(const CHA* source, ISC source_bytes,
                             const CHA* comment_mask, ISC tab_space_count) {
  for (ISC line_start = 0; line_start < source_bytes; ) {
    ISC line_end = line_start;
    while (line_end < source_bytes && source[line_end] != '\n') ++line_end;

    BOL has_code = false, has_comment = false;
    ISC last_code_byte = line_start - 1;
    for (ISC b = line_start; b < line_end; ++b) {
      if (comment_mask[b]) { has_comment = true; continue; }
      const CHA c = source[b];
      if (c != ' ' && c != '\t' && c != '\r') {
        has_code = true;
        last_code_byte = b;
      }
    }

    if (!has_code) {
      /* Drop comment-only lines; keep genuinely blank lines. */
      if (!has_comment) {
        for (ISC b = line_start; b < line_end; ++b)
          CodePutChar(source[b], tab_space_count);
        if (line_end < source_bytes) fputc('\n', stdout);
      }
    } else {
      for (ISC b = line_start; b <= last_code_byte; ++b) {
        if (!comment_mask[b]) CodePutChar(source[b], tab_space_count);
      }
      if (line_end < source_bytes) fputc('\n', stdout);
    }
    line_start = (line_end < source_bytes) ? line_end + 1 : source_bytes;
  }
}

/* @copydoc StripComments */
ISC StripComments(const CHA* directory, const CHA* filename,
                  ISC tab_space_count) {
  if (IsError(directory) || IsError(filename)) return -1;

  CHA path[4096];
  snprintf(path, sizeof(path), "%s/%s", directory, filename);

  ISC source_bytes = 0;
  CHA* source = CodeReadFile(path, &source_bytes);
  if (!source) return -1;

  /* One token per byte is the worst case. */
  ISC capacity = source_bytes + 64;
  TCodeToken* tokens =
      (TCodeToken*)malloc(sizeof(TCodeToken) * (IUC)capacity);
  if (!tokens) { free(source); return -1; }

  TCodeLexer lexer;
  if (CodeLexerInit(&lexer, source, (IUD)source_bytes, tokens, capacity) != 0) {
    free(tokens); free(source); return -1;
  }
  while (CodeLexerNext(&lexer) != CODE_TOK_EOF) {
    if (lexer.full) { free(tokens); free(source); return -1; }
  }

  CHA* comment_mask = (CHA*)calloc((IUC)source_bytes + 1, 1);
  if (!comment_mask) { free(tokens); free(source); return -1; }
  for (ISC i = 0; i < ISC(lexer.token_count); ++i) {
    if (!CodeIsCommentKind(tokens[i].kind)) continue;
    for (ISC b = ISC(tokens[i].byte_start); b < ISC(tokens[i].byte_end); ++b)
      comment_mask[b] = 1;
  }

  CodeEmitStripped(source, source_bytes, comment_mask, tab_space_count);

  free(comment_mask); free(tokens); free(source);
  return 0;
}

/* @copydoc StripComments */
ISC StripComments(const CHA* directory) {
  (void)directory;  /* Directory enumeration is supplied by the Fitter driver. */
  return 0;
}

}  //< namespace Code
}  //< namespace CTK
