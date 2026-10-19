// Copyright AStarship <https://astarship.net>.
#include "_Config.h"
#include <ASCIICrabs/Test.h>
#include "IMUL/Parser.h"
using namespace _;

/* Conformance case S01 (#161): `world ^ Hello` -> one note event,
display `Hello world`, one correction pair, original bytes unchanged. */
namespace CT {
namespace IMUL {
inline const CHA* T01S01(const CHA* args) {
  A_TEST_BEGIN;
  const CHA* source = "world ^ Hello\n";
  const IUD source_len = 14;
  enum { Size = 8 };
  TImulEvent events[Size];
  TImulSession sessions[Size];
  TImulMapping mappings[Size];
  TImulDiagnostic diagnostics[Size];
  TImulCorrection corrections[Size];
  TImulStorage storage = {};
  storage.events = events;
  storage.event_capacity = Size;
  storage.sessions = sessions;
  storage.session_capacity = Size;
  storage.mappings = mappings;
  storage.mapping_capacity = Size;
  storage.diagnostics = diagnostics;
  storage.diagnostic_capacity = Size;
  storage.corrections = corrections;
  storage.correction_capacity = Size;
  TImulResult result = {};
  TImulWorkspace* imul_workspace = NILP;
  TImulLimits* imul_limits = NILP;
  const ISN status =
      ImulCompile(source, source_len, imul_workspace, imul_limits, &storage,
                  &result);
  if (status != 0) {
    return "S01: ImulCompile did not report a valid document";
  }
  if (result.event_count != 1) {
    return "S01: expected exactly one note event";
  }
  const TImulEvent* note = result.events;
  if (!note) {
    return "S01: expected event records";
  }
  if (note->kind != IMUL_EVENT_NOTE) {
    return "S01: expected a note event";
  }
  if (note->correction_count != 1) {
    return "S01: expected one correction pair";
  }
  if (!result.corrections) {
    return "S01: expected correction records";
  }
  // Display must be `Hello world` reconstructed from the source bytes.
  static const CHA expected_display[] = "Hello world";
  for (ISN i = 0; i < 11; ++i) {
    if (note->display_text[i] != expected_display[i]) {
      return "S01: display text is not 'Hello world'";
    }
  }
  // Original bytes must be unchanged.
  static const CHA expected_source[] = "world ^ Hello";
  for (ISN i = 0; i < 13; ++i) {
    if (source[i] != expected_source[i]) {
      return "S01: original source bytes were modified";
    }
  }
  return 0;
}
}  //< namespace IMUL
}  //< namespace CT
