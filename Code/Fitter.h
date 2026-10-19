// Copyright AStarship <https://astarship.net>.
#pragma once
#include "_Config.h"
#ifndef CRABSTK_CODE_FITTER_H
#define CRABSTK_CODE_FITTER_H

/* CrabsTK Code Fitter — the "Prettier for agents."
Takes a token stream (from Lexer) and re-emits it in canonical
Chimera+ form. The Fitter is the enforcement layer: every C++ file
that passes through it comes out in exactly the shape the LLM
expects. One dialect, one token budget, one round-trip guarantee.

Chimera+ rules enforced:
  - 2-space indent
  - Opening brace: same line for functions, next line for structs
  - One blank line between top-level functions
  - #include ordering: _Config.h, then ASCIICrabs, then local
  - Header guard: #pragma once + #ifndef CRABSTK_NAME_H
  - Seam gating: #if SEAM >= CRABS_NAME
  - Chimera+ naming: flags snake_case where CamelCase is required
  - No std:: types (hard error)

No allocation, no std::, no stb. Output is written to a caller-owned
byte buffer. */

namespace _ {

/* Fitter diagnostics. Each finding has a severity, a code, a span,
and a message. The Fitter collects these into a caller-owned array. */
enum EFitSeverity : IUB {
  FIT_SEV_ERROR = 1,    //< Hard error (std:: usage, missing header guard).
  FIT_SEV_WARNING = 2,  //< Style violation (indent, naming).
  FIT_SEV_NOTE = 3,     //< Informational.
};

enum EFitCode : IUB {
  FIT_CODE_NONE = 0,
  FIT_CODE_STD_TYPE,        //< std::string, std::vector, etc.
  FIT_CODE_INDENT,          //< Wrong indentation.
  FIT_CODE_BRACE,           //< Brace placement violation.
  FIT_CODE_INCLUDE_ORDER,   //< #include not in canonical order.
  FIT_CODE_HEADER_GUARD,    //< Missing or wrong header guard.
  FIT_CODE_SEAM_GATE,       //< Missing #if SEAM >= gate.
  FIT_CODE_NAMING,          //< Chimera+ naming violation.
  FIT_CODE_BLANK_LINE,      //< Missing/extra blank line.
  FIT_CODE_TAB,             //< Tab instead of spaces.
};

struct TFitting {
  EFitSeverity severity;
  EFitCode code;
  ISN line;
  ISN column;
  const CHA* message;  //< Static string, never null.
};

/* Fitter options. All have sensible defaults for CrabsTK. */
struct TFitterOptions {
  ISN indent_width;       //< Spaces per indent level. Default 2.
  ISN max_line_length;    //< Soft limit. Default 100. 0 = unlimited.
  BOL enforce_seam_gate;  //< Require #if SEAM >= in .h files. Default true.
  BOL enforce_header_guard; //< Require #pragma once. Default true.
  const CHA* project_name;  //< For guard prefix (e.g. "CRABSTK"). Default "CRABSTK".
};

/* Fitter result: reformatted bytes + diagnostics. */
struct TFittingResult {
  const CHA* output;       //< Formatted source (caller-owned buffer).
  IUD output_len;          //< Length in bytes.
  const TFitting* findings; //< Caller-owned diagnostic array.
  ISN finding_count;
  ISN finding_capacity;
  ISN error_count;         //< Findings with FIT_SEV_ERROR.
  ISN warning_count;       //< Findings with FIT_SEV_WARNING.
};

/* Fit a token stream into canonical Chimera+ form.
tokens:       Token array from CodeLexer.
token_count:  Number of tokens.
source:       Original source bytes (for extracting token text).
out_buf:      Caller-owned output buffer.
out_buf_len:  Capacity of out_buf (bytes).
out_result:   Filled with output pointer/length and diagnostics.
findings:     Caller-owned diagnostic array.
finding_capacity: Capacity of findings.
options:      Fitter options (pass zero-initialized for defaults).
Returns 0 on success, -1 on bad arguments, -2 if out_buf is too small. */
ISN CodeFit(const TCodeToken* tokens, ISN token_count,
            const CHA* source,
            CHA* out_buf, IUD out_buf_len,
            TFittingResult* out_result,
            TFitting* findings, ISN finding_capacity,
            const TFitterOptions* options);

/* Count indent level at a given token index (by tracking braces).
Used internally; exposed for the LSP. */
ISN CodeIndentLevel(const TCodeToken* tokens, ISN token_count, ISN index);

}  //< namespace _
#endif
