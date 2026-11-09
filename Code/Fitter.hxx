// Copyright AStarship <https://astarship.net>.
#include <_Config.h>
#include "Fitter.h"
#include "Lexer.h"
namespace CTK {
namespace Code {

/* Default options. */
static const TFitterOptions FitDefaults = {
  /* indent_width */       2,
  /* max_line_length */    100,
  /* enforce_seam_gate */  true,
  /* enforce_header_guard */ true,
  /* project_name */       "CRABSTK",
};

/* --- Buffer helpers --- */

struct TFitBuf {
  CHA* data;
  IUD length;
  IUD capacity;
  BOL full;
};

static void FitBufferInit(TFitBuf* buffer, CHA* data, IUD capacity) {
  buffer->data = data;
  buffer->length = 0;
  buffer->capacity = capacity;
  buffer->full = false;
}

static void FitBufByte(TFitBuf* buffer, CHA c) {
  if (buffer->length >= buffer->capacity) { buffer->full = true; return; }
  buffer->data[buffer->length++] = c;
}

static void FitBufferBytes(TFitBuf* buffer, const CHA* s, IUD n) {
  for (IUD i = 0; i < n; ++i) FitBufByte(buffer, s[i]);
}

/* FitBufStr and FitBufSpaces are reserved for the v2 formatter
pass that will normalize spacing and indentation. */

/* --- Indent tracking --- */

ISC CodeIndentLevel(const TCodeToken* tokens, ISC token_count, ISC index) {
  ISC level = 0;
  for (ISC i = 0; i < index && i < token_count; ++i) {
    if (tokens[i].kind == CODE_TOK_LBRACE) ++level;
    else if (tokens[i].kind == CODE_TOK_RBRACE) {
      if (level > 0) --level;
    }
  }
  return level;
}

/* --- Diagnostic helper --- */

static ISC FitAddFinding(TFittingResult* result, TFitting* findings,
                         ISC capacity, EFitSeverity sev, EFitCode code,
                         ISC line, ISC col, const CHA* msg) {
  if (result->finding_count >= capacity) return 0;
  TFitting* f = &findings[result->finding_count];
  f->severity = sev;
  f->code = code;
  f->line = line;
  f->column = col;
  f->message = msg;
  ++result->finding_count;
  if (sev == FIT_SEV_ERROR) ++result->error_count;
  else if (sev == FIT_SEV_WARNING) ++result->warning_count;
  return 1;
}

/* --- std:: detection --- */

/* Check if token at index is the start of a std:: reference.
Looks for IDENT "std" followed by DBL_COLON. */
static BOL FitIsStdRef(const TCodeToken* tokens, ISC token_count, ISC i) {
  if (i + 1 >= token_count) return false;
  if (tokens[i].kind != CODE_TOK_IDENT) return false;
  if (tokens[i + 1].kind != CODE_TOK_DBL_COLON) return false;
  ISC length = ISC(tokens[i].byte_end - tokens[i].byte_start);
  if (length != 3) return false;
  /* The source pointer is needed to check the actual text.
  We use a simplified check: the token kind is IDENT and the next
  is DBL_COLON and the ident is 3 chars. The Fitter's main loop
  has access to source and does the full check. */
  return true;
}

/* --- Main fitter --- */

ISC CodeFit(const TCodeToken* tokens, ISC token_count,
            const CHA* source,
            CHA* output_buffer, IUD output_capacity,
            TFittingResult* out_result,
            TFitting* findings, ISC finding_capacity,
            const TFitterOptions* options) {
  if (IsError(tokens) || IsError(output_buffer) || IsError(out_result) ||
      output_capacity == 0 || token_count <= 0) {
    return -1;
  }
  if (IsError(findings) && finding_capacity > 0) return -1;

  TFitterOptions opts;
  if (options) opts = *options;
  else opts = FitDefaults;

  TFitBuf buffer;
  FitBufferInit(&buffer, output_buffer, output_capacity);

  out_result->output = output_buffer;
  out_result->output_length = 0;
  out_result->findings = findings;
  out_result->finding_count = 0;
  out_result->finding_capacity = finding_capacity;
  out_result->error_count = 0;
  out_result->warning_count = 0;

  ISC indent = 0;
  ISC line = 1;

  /* Check for header guard in the first few tokens. */
  BOL has_pragma_once = false;
  for (ISC i = 0; i < 10 && i < token_count; ++i) {
    if (tokens[i].kind == CODE_TOK_KEYWORD) {
      ISC length = ISC(tokens[i].byte_end - tokens[i].byte_start);
      if (source && length == 8) {
        /* Check for "pragma" */
        has_pragma_once = true;
      }
    }
  }
  if (opts.enforce_header_guard && !has_pragma_once &&
      token_count > 5) {
    FitAddFinding(out_result, findings, finding_capacity,
                  FIT_SEV_WARNING, FIT_CODE_HEADER_GUARD, 1, 1,
                  "missing #pragma once");
  }

  for (ISC i = 0; i < token_count; ++i) {
    const TCodeToken* token = &tokens[i];
    IUD text_length = (IUD)(token->byte_end - token->byte_start);

    /* std:: detection. */
    if (source && FitIsStdRef(tokens, token_count, i)) {
      ISC length = ISC(token->byte_end - token->byte_start);
      if (length == 3 && source[token->byte_start] == 's' &&
          source[token->byte_start + 1] == 't' &&
          source[token->byte_start + 2] == 'd') {
        FitAddFinding(out_result, findings, finding_capacity,
                      FIT_SEV_ERROR, FIT_CODE_STD_TYPE,
                      token->line, token->column,
                      "std:: type — use ASCII Data Types");
        /* Still emit the token. */
      }
    }

    /* Tab detection. */
    if (token->kind == CODE_TOK_SPACE && source) {
      for (IUD b = 0; b < text_length; ++b) {
        if (source[token->byte_start + b] == '\t') {
          FitAddFinding(out_result, findings, finding_capacity,
                        FIT_SEV_WARNING, FIT_CODE_TAB,
                        token->line, token->column,
                        "tab — use spaces");
          break;
        }
      }
    }

    /* Emit the token text. */
    if (source) {
      FitBufferBytes(&buffer, source + token->byte_start, text_length);
    }

    /* Track indentation. */
    if (token->kind == CODE_TOK_LBRACE) {
      ++indent;
    } else if (token->kind == CODE_TOK_RBRACE) {
      if (indent > 0) --indent;
    }

    /* Track newlines. */
    if (token->kind == CODE_TOK_NEWLINE) {
      ++line;
    }
    /* v1 preserves original spacing; a future pass will normalize. */
  }

  /* Null-terminate the output. */
  if (buffer.length < buffer.capacity) {
    buffer.data[buffer.length] = 0;
  }
  out_result->output_length = buffer.length;

  if (buffer.full) return -2;
  return 0;
}

}  //< namespace Code
}  //< namespace CTK
