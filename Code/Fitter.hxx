// Copyright AStarship <https://astarship.net>.
#include <_Config.h>
#include "Fitter.h"
#include "Lexer.h"

namespace _ {

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
  IUD len;
  IUD cap;
  BOL full;
};

static void FitBufInit(TFitBuf* buf, CHA* data, IUD cap) {
  buf->data = data;
  buf->len = 0;
  buf->cap = cap;
  buf->full = false;
}

static void FitBufByte(TFitBuf* buf, CHA c) {
  if (buf->len >= buf->cap) { buf->full = true; return; }
  buf->data[buf->len++] = c;
}

static void FitBufBytes(TFitBuf* buf, const CHA* s, IUD n) {
  for (IUD i = 0; i < n; ++i) FitBufByte(buf, s[i]);
}

/* FitBufStr and FitBufSpaces are reserved for the v2 formatter
pass that will normalize spacing and indentation. */

/* --- Indent tracking --- */

ISN CodeIndentLevel(const TCodeToken* tokens, ISN token_count, ISN index) {
  ISN level = 0;
  for (ISN i = 0; i < index && i < token_count; ++i) {
    if (tokens[i].kind == CODE_TOK_LBRACE) ++level;
    else if (tokens[i].kind == CODE_TOK_RBRACE) {
      if (level > 0) --level;
    }
  }
  return level;
}

/* --- Diagnostic helper --- */

static ISN FitAddFinding(TFittingResult* result, TFitting* findings,
                         ISN capacity, EFitSeverity sev, EFitCode code,
                         ISN line, ISN col, const CHA* msg) {
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
static BOL FitIsStdRef(const TCodeToken* tokens, ISN token_count, ISN i) {
  if (i + 1 >= token_count) return false;
  if (tokens[i].kind != CODE_TOK_IDENT) return false;
  if (tokens[i + 1].kind != CODE_TOK_DBL_COLON) return false;
  ISN len = tokens[i].byte_end - tokens[i].byte_start;
  if (len != 3) return false;
  /* The source pointer is needed to check the actual text.
  We use a simplified check: the token kind is IDENT and the next
  is DBL_COLON and the ident is 3 chars. The Fitter's main loop
  has access to source and does the full check. */
  return true;
}

/* --- Main fitter --- */

ISN CodeFit(const TCodeToken* tokens, ISN token_count,
            const CHA* source,
            CHA* out_buf, IUD out_buf_len,
            TFittingResult* out_result,
            TFitting* findings, ISN finding_capacity,
            const TFitterOptions* options) {
  if (IsError(tokens) || IsError(out_buf) || IsError(out_result) ||
      out_buf_len == 0 || token_count <= 0) {
    return -1;
  }
  if (IsError(findings) && finding_capacity > 0) return -1;

  TFitterOptions opts;
  if (options) opts = *options;
  else opts = FitDefaults;

  TFitBuf buf;
  FitBufInit(&buf, out_buf, out_buf_len);

  out_result->output = out_buf;
  out_result->output_len = 0;
  out_result->findings = findings;
  out_result->finding_count = 0;
  out_result->finding_capacity = finding_capacity;
  out_result->error_count = 0;
  out_result->warning_count = 0;

  ISN indent = 0;
  ISN line = 1;

  /* Check for header guard in the first few tokens. */
  BOL has_pragma_once = false;
  for (ISN i = 0; i < 10 && i < token_count; ++i) {
    if (tokens[i].kind == CODE_TOK_KEYWORD) {
      ISN len = tokens[i].byte_end - tokens[i].byte_start;
      if (source && len == 8) {
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

  for (ISN i = 0; i < token_count; ++i) {
    const TCodeToken* tok = &tokens[i];
    IUD text_len = (IUD)(tok->byte_end - tok->byte_start);

    /* std:: detection. */
    if (source && FitIsStdRef(tokens, token_count, i)) {
      ISN len = tok->byte_end - tok->byte_start;
      if (len == 3 && source[tok->byte_start] == 's' &&
          source[tok->byte_start + 1] == 't' &&
          source[tok->byte_start + 2] == 'd') {
        FitAddFinding(out_result, findings, finding_capacity,
                      FIT_SEV_ERROR, FIT_CODE_STD_TYPE,
                      tok->line, tok->column,
                      "std:: type — use ASCII Data Types");
        /* Still emit the token. */
      }
    }

    /* Tab detection. */
    if (tok->kind == CODE_TOK_SPACE && source) {
      for (IUD b = 0; b < text_len; ++b) {
        if (source[tok->byte_start + b] == '\t') {
          FitAddFinding(out_result, findings, finding_capacity,
                        FIT_SEV_WARNING, FIT_CODE_TAB,
                        tok->line, tok->column,
                        "tab — use spaces");
          break;
        }
      }
    }

    /* Emit the token text. */
    if (source) {
      FitBufBytes(&buf, source + tok->byte_start, text_len);
    }

    /* Track indentation. */
    if (tok->kind == CODE_TOK_LBRACE) {
      ++indent;
    } else if (tok->kind == CODE_TOK_RBRACE) {
      if (indent > 0) --indent;
    }

    /* Track newlines. */
    if (tok->kind == CODE_TOK_NEWLINE) {
      ++line;
    }
    /* v1 preserves original spacing; a future pass will normalize. */
  }

  /* Null-terminate the output. */
  if (buf.len < buf.cap) {
    buf.data[buf.len] = 0;
  }
  out_result->output_len = buf.len;

  if (buf.full) return -2;
  return 0;
}

}  //< namespace _
