// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef CRABSTK_IMUL_PARSER_H
#define CRABSTK_IMUL_PARSER_H

namespace _ {

/* Event payload kinds (payload of each compiled line). */
enum {
  IMUL_EVENT_NONE = 0,
  IMUL_EVENT_NOTE = 1,       //< Ordinary or corrected note.
  IMUL_EVENT_LITERAL = 2,    //< `|` literal payload or fenced line.
  IMUL_EVENT_SESSION_START = 3,
  IMUL_EVENT_SESSION_END = 4,
  IMUL_EVENT_MAPPING = 5,
};

/* Target resolution labels. */
enum {
  IMUL_RES_NULL = 0,         //< No target on the line / null context.
  IMUL_RES_SYNTAX_ONLY = 1,  //< Qualified target, no manifest.
  IMUL_RES_WORKSPACE = 2,    //< Resolved against a manifest binding.
};

/* Mapping directions. */
enum {
  IMUL_MAP_ONE_WAY = 1,
  IMUL_MAP_TWO_WAY = 2,
};

/* Mapping operand kinds. */
enum {
  IMUL_NODE_IDENTIFIER = 1,
  IMUL_NODE_LITERAL = 2,
};

/* Diagnostic codes (Compiler.md). */
enum {
  IMUL_DIAG_NONE = 0,
  IMUL_DIAG_TIME_INVALID,
  IMUL_DIAG_TIME_ANCHOR_MISSING,
  IMUL_DIAG_TIME_SUFFIX_INVALID,
  IMUL_DIAG_TIME_OVERFLOW,
  IMUL_DIAG_WORKSPACE_REQUIRED,
  IMUL_DIAG_REPOSITORY_UNKNOWN,
  IMUL_DIAG_REPOSITORY_AMBIGUOUS,
  IMUL_DIAG_ISSUE_INVALID,
  IMUL_DIAG_ISSUE_REPOSITORY_MISSING,
  IMUL_DIAG_REORDER_INVALID,
  IMUL_DIAG_MAPPING_INVALID,
  IMUL_DIAG_SESSION_INVALID,
  IMUL_DIAG_SESSION_UNCLOSED,
  IMUL_DIAG_SESSION_TARGET_CHANGED,
  IMUL_DIAG_FENCE_UNCLOSED,
  IMUL_DIAG_ENCODING_INVALID,
  IMUL_DIAG_LIMIT_EXCEEDED,
};

/* Bounded byte span into the ORIGINAL IMUL source (half-open).
@code
  byte_start .. byte_end - 1
@endcode
line/column are 1-based; column counts bytes from the line start. */
struct TImulSpan {
  ISN byte_start,
      byte_end,
      line,
      column;
};

/* A word-correction pair inside a note (original span + display operands). */
struct TImulCorrection {
  TImulSpan span;         //< The original correction span (both operands).
  IUD has_left;
  const CHA* left;
  ISN left_len;
  IUD has_right;
  const CHA* right;
  ISN right_len;
};

/* A mapping operand (kind, original text, comparison key). */
struct TImulMapNode {
  IUD kind;             //< IMUL_NODE_IDENTIFIER or IMUL_NODE_LITERAL.
  IUD has_text;
  const CHA* text;      //< Original operand text.
  ISN text_len;
  IUD has_key;
  const CHA* key;       //< Identifier comparison key (identifiers only).
  ISN key_len;
};

/* One mapping record (source-backed edge). */
struct TImulMapping {
  TImulSpan span;
  IUD direction;        //< IMUL_MAP_ONE_WAY or IMUL_MAP_TWO_WAY.
  TImulMapNode left,
              right;
};

/* A diagnostic with code, severity, span, and message. */
struct TImulDiagnostic {
  IUD code;
  IUD severity;         //< 1 = error, 2 = warning.
  TImulSpan span;
  const CHA* message;   //< Explanatory; tests assert code/span, not wording.
};

/* A resolved IMUL timestamp. Civil fields are the clock as written/advanced;
offset_minutes is null (ISD(0) with has_offset false) for floating clocks. */
struct TImulTimestamp {
  IUB year,      //< 0001..9999.
      month,     //< 1..12.
      day,       //< 1..31.
      hour,      //< 0..23.
      minute,    //< 0..59.
      second;    //< 0..59.
  IUD has_offset;      //< 1 when offset_minutes is meaningful.
  ISD offset_minutes;  //< -840..+840, null when floating.
  IUD exact;           //< 1 = exact, 0 = approximate.
  IUD second_grain;    //< 1 = second granularity, 0 = minute.
};

/* One compiled IMUL line: its committed context snapshot plus the payload. */
struct TImulEvent {
  IUD kind;  //< IMUL_EVENT_* code.
  TImulSpan span;
  IUD has_repo;          //< 1 when repository points into the source.
  const CHA* repository; //< Canonical "owner/repo" (source/manifest owned).
  IUD has_issue;         //< 1 when issue is set.
  IUD issue;             //< 1..2147483647.
  IUD has_timestamp;     //< 1 when timestamp is set.
  TImulTimestamp timestamp;
  IUD resolution;        //< IMUL_RES_* code.
  IUD has_display;       //< 1 when display_text is set.
  const CHA* display_text;
  ISN display_len;
  // Note payloads:
  IUD has_corrections;
  const TImulCorrection* corrections;
  ISN correction_count;
  // Mapping payloads:
  IUD has_mapping;
  TImulMapping mapping;
};

/* A closed session row (start/stop pairs committed in source order). */
struct TImulSession {
  ISN session_id;
  IUD has_label;
  const CHA* label;
  ISN label_len;
  IUD has_repo;
  const CHA* repository;
  IUD has_issue;
  IUD issue;
  TImulTimestamp start,
                end;
  TImulSpan start_span,
            end_span;
  ISD duration_seconds;  //< -1 when incomparable (should not happen if valid).
  IUD exact;             //< 1 exact duration, 0 nominal/approximate.
  IUD basis;             //< 1 = civil (floating), 0 = utc (known offsets).
};

/* One conformance case result (filled by the seam tracers). */
struct TImulCase {
  IUD passed;  //< 1 when every assertion for the case held.
  IUD checked; //< 1 when the case was actually evaluated.
};

/* Compile result: status, committed records, and case outcomes.
The caller owns all storage the result arrays point at. */
struct TImulResult {
  IUD status;            //< 0 valid, 1 has errors, 2 limit exceeded.
  ISN event_count,
      session_count,
      mapping_count,
      error_count,
      warning_count,
      correction_count;
  const TImulEvent* events;          //< Caller-owned array.
  const TImulSession* sessions;      //< Caller-owned array.
  const TImulMapping* mappings;      //< Caller-owned array (all edges).
  const TImulDiagnostic* diagnostics;//< Caller-owned array.
  const TImulCorrection* corrections;//< Caller-owned array (note payloads).
};

/* Limits (inclusive byte/count caps).
Null selects the CLI defaults: 1 MiB source, 64 KiB per line, 16,384
events, 1,024 manifest bindings. Session and diagnostic capacity is
bounded by the caller-owned arrays below. */
struct TImulLimits {
  IUD source_max,     //< Max source bytes.
      line_max,       //< Max line bytes (including line ending).
      event_max,      //< Max events.
      binding_max;    //< Max manifest bindings.
};

/* Workspace manifest: read-only set of `repo owner/repo [as alias]` lines. */
struct TImulWorkspace {
  const CHA* text;    //< Original manifest bytes (null = no manifest).
  IUD text_len;
  ISN binding_count;  //< Filled by ImulParseWorkspace.
};

/* Caller-owned compile storage. All array pointers may be null only when
the matching capacity is zero. ImulCompile fills result's arrays with
pointers into this storage. */
struct TImulStorage {
  TImulEvent* events;
  ISN event_capacity;
  TImulSession* sessions;
  ISN session_capacity;
  TImulMapping* mappings;
  ISN mapping_capacity;
  TImulDiagnostic* diagnostics;
  ISN diagnostic_capacity;
  TImulCorrection* corrections;
  ISN correction_capacity;
};

/* Parses a workspace manifest into its bindings (bounded by limits).
@return 0 valid, 1 invalid manifest. */
ISN ImulParseWorkspace(TImulWorkspace* workspace, TImulLimits* limits,
                       TImulStorage* storage, TImulResult* result);

/* Compiles verified IMUL text into the caller-owned records of storage.
The source bytes are preserved untouched; records point into them.
Each document starts with empty clock/target/session state.
@return 0 success, 1 invalid document (diagnostics recorded),
        2 limit/encoding failure. No allocation, no host clock, no I/O. */
ISN ImulCompile(const CHA* source, IUD source_len, TImulWorkspace* workspace,
                TImulLimits* limits, TImulStorage* storage,
                TImulResult* result);

}  //< namespace _
#endif
