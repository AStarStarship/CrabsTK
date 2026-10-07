// Copyright AStarship <https://astarship.net>.
#include <_Config.h>
#include <ASCIICrabs/Test.h>
#include "IMUL/Parser.h"
using namespace _;

/* Single-tracer conformance test.
S01: `world ^ Hello` -> one note, display `Hello world`, one correction.
RED: ImulCompile has no implementation yet, so this fails on purpose. */
namespace CT {
namespace IMUL {
inline const CHA* T01S01(const CHA* args) {
  A_TEST_BEGIN;
  const CHA* source = "world ^ Hello\n";
  TImulResult result = {};
  const ISN status = ImulCompile(source, 14, NILP, &result);
  if (status != 0) {
    return "S01: ImulCompile did not report a valid document";
  }
  if (result.event_count != 1) {
    return "S01: expected exactly one note event";
  }
  return "S01: expected one note event";
}
}  //< namespace IMUL
}  //< namespace CT
