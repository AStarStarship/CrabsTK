// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef CRABSTK_IMUL_PARSER_H
#define CRABSTK_IMUL_PARSER_H

namespace _ {

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
  IUD kind;  //< TImulEventKind code.
  TImulSpan span;
  IUD has_repo;          //< 1 when repository points into the source.
  const CHA* repository; //< Canonical "owner/repo" (caller/manifest owned).
  IUD has_issue;         //< 1 when issue is set.
  IUD issue;             //< 1..2147483647.
  IUD has_timestamp;     //< 1 when timestamp is set.
  TImulTimestamp timestamp;
  IUD resolution;        //< TImulResolution code, 0 when no target.
  IUD has_display;       //< 1 when display_text points into the source.
  const CHA* display_text;
  ISN display_len;
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

/* Compile result: status, committed records, and case outcomes. */
struct TImulResult {
  IUD status;            //< 0 valid, 1 has errors, 2 limit exceeded.
  ISN event_count,
      session_count,
      error_count,
      warning_count;
  TImulCase cases[64];   //< Index by conformance case number (0 = unused).
};

/* Limits (inclusive byte/count caps). */
struct TImulLimits {
  IUD source_max,     //< Max source bytes (default 1 MiB).
      line_max,       //< Max line bytes (default 64 KiB).
      event_max,      //< Max events (default 16384).
      binding_max;    //< Max manifest bindings (default 1024).
};

/* Compiles verified IMUL text into the caller-owned records of result.
The source bytes are preserved untouched; records point into them.
@return 0 success, 1 invalid document (diagnostics recorded),
        2 limit/encoding failure. No allocation, no host clock, no I/O. */
ISN ImulCompile(const CHA* source, IUD source_len, TImulLimits* limits,
                TImulResult* result);

}  //< namespace _
#endif
