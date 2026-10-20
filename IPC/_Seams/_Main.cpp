// Copyright AStarship <https://astarship.net>.
// IPC seams entry point: run the Core, Ring, Legacy, TwoProc groups.
// Result convention: null error + summary line on success; first error
// string on failure. Exit code 0/1.

#include "_Config.h"
#include "01.Ring.h"
#include "02.LegacyRed.h"
#include "03.TwoProc.h"
#include <unistd.h>
#include <cstdio>

namespace _ {
namespace IpcTests {

ISC Report(const CHA* error) {
  std::printf("IPC seams: checks=%d failures=%d\n", (int)g_checks,
              (int)g_failures);
  const CHA* text = error ? error
                          : "PASS: IPC core/ring/twoproc seams";
  std::printf("%s\n", text);
  return error || g_failures ? 1 : 0;
}

}  //< namespace IpcTests
}  //< namespace _

int main() {
  const CHA* error = _::IpcTests::Core(nullptr);
  if (!error) error = _::IpcTests::Ring(nullptr);
  if (!error) error = _::IpcTests::Legacy(nullptr);
  if (!error) error = _::IpcTests::TwoProc(nullptr);
  return _::IpcTests::Report(error);
}
