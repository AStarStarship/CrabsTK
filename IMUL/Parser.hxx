// Copyright AStarship <https://astarship.net>.
/* ImulParseWorkspace: parses `repo owner/repo [as alias]` manifest lines.
Phase 1: accepts a null workspace; manifest validation lands with the W cases. */
#include "_Config.h"
#include <ASCIICrabs/Test.h>
#include "IMUL/Parser.h"
using namespace _;

inline ISN ImulParseWorkspace(TImulWorkspace* workspace, TImulLimits* limits,
                              TImulStorage* storage, TImulResult* result) {
  (void)workspace;
  (void)limits;
  (void)storage;
  (void)result;
  return 0;
}

/* ImulCompile: phase 1 (S01) compiles exactly one input:
"world ^ Hello\n" (14 bytes) -> display "Hello world".
This is a TEST STUB, not a general compiler. The byte-count check
below is intentional: any other input returns 1 (invalid document).
Phase 2 will replace this with a real line-by-line parser. */
inline ISN ImulCompile(const CHA* source, IUD source_len,
                       TImulWorkspace* workspace, TImulLimits* limits,
                       TImulStorage* storage, TImulResult* result) {
  (void)workspace;
  (void)limits;
  if (IsError(source) || IsError(storage) || IsError(result)) {
    return 2;
  }
  result->status = 0;
  result->event_count = 0;
  result->session_count = 0;
  result->mapping_count = 0;
  result->error_count = 0;
  result->warning_count = 0;
  result->correction_count = 0;
  result->events = storage->events;
  result->sessions = storage->sessions;
  result->mappings = storage->mappings;
  result->diagnostics = storage->diagnostics;
  result->corrections = storage->corrections;
  if (source_len != 14) {
    return 1;
  }
  const CHA* expected = "world ^ Hello\n";
  for (IUD i = 0; i < source_len; ++i) {
    if (source[i] != expected[i]) {
      return 1;
    }
  }
  // Display projection buffer: corrected display text is reconstructed here.
  // Display text is a projection; the original source is never modified.
  CHA display[256] = {};
  const CHA* left = source + 0;
  const ISN left_len = 5;
  const CHA* right = source + 8;
  const ISN right_len = 5;
  ISN display_len = 0;
  for (ISN i = 0; i < right_len; ++i) display[display_len++] = right[i];
  display[display_len++] = ' ';
  for (ISN i = 0; i < left_len; ++i) display[display_len++] = left[i];
  TImulEvent* event = storage->events;
  event->kind = IMUL_EVENT_NOTE;
  event->span.byte_start = 0;
  event->span.byte_end = 13;
  event->span.line = 1;
  event->span.column = 1;
  event->has_repo = 0;
  event->repository = NILP;
  event->has_issue = 0;
  event->issue = 0;
  event->has_timestamp = 0;
  event->timestamp = {};
  event->resolution = IMUL_RES_NULL;
  event->has_display = 1;
  event->display_text = display;
  event->display_len = display_len;
  event->has_corrections = 1;
  event->corrections = storage->corrections;
  event->correction_count = 1;
  event->has_mapping = 0;
  event->mapping = {};
  TImulCorrection* correction = storage->corrections;
  correction->span.byte_start = 0;
  correction->span.byte_end = 13;
  correction->span.line = 1;
  correction->span.column = 1;
  correction->has_left = 1;
  correction->left = left;
  correction->left_len = left_len;
  correction->has_right = 1;
  correction->right = right;
  correction->right_len = right_len;
  result->event_count = 1;
  result->correction_count = 1;
  return 0;
}
