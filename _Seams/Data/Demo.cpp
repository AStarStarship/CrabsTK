// Copyright AStarship <https://astarship.net>.
#include <_Config.h>
#include <errno.h>
#include <unistd.h>
#include "../../Data/_Package.hxx"

namespace _ {

BOL DemoWrite(const CHA* text, IUC bytes) {
  IUC sent = 0;
  while (sent < bytes) {
    ISW result = write(1, text + sent, bytes - sent);
    if (result < 0 && errno == EINTR) continue;
    if (result <= 0) return false;
    sent += IUC(result);
  }
  return true;
}

BOL DemoLabel(const CHA* text) {
  IUC bytes = 0;
  while (text[bytes]) ++bytes;
  return DemoWrite(text, bytes);
}

}  //< namespace _

ISC main() {
  using namespace _;
  if (!DemoLabel("Synthetic seam inputs - NOT training results.\n")) return 2;
  const CHA* Names[5] = {"reward", "policy_loss", "value_loss",
                        "approx_kl", "entropy"};
  const FPC Values[5][12] = {
    {-2, -1, 0, 1, 1, 2, 1, 2, 3, 2, 3, 4},
    {4, 3, 2, 1, 0, -1, -1, -2, -1, -2, -2, -3},
    {4, 3, 3, 2, 2, 1, 2, 1, 1, 0.5F, 1, 0.25F},
    {0, 0.1F, 0.2F, 0.1F, 0.4F, 0.2F, 0.1F, 0.1F, 0.3F, 0.2F, 0.1F, 0},
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
  };
  for (IUC series = 0; series < 5; ++series) {
    TAsciiPlot<12, 5> plot = {};
    for (FPC value : Values[series])
      if (TAsciiPlotPush(plot, value) != PlotOK) return 1;
    CHA text[TAsciiPlot<12, 5>::Bytes] = {};
    IUC written = 0;
    if (TAsciiPlotRender(plot, text, sizeof(text), &written) != PlotOK) return 1;
    if (!DemoLabel(Names[series]) || !DemoLabel("\n") ||
        !DemoWrite(text, written)) return 2;
  }
  return 0;
}
