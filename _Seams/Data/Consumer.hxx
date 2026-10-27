// Copyright AStarship <https://astarship.net>.
#include "_Config.h"
#include "../../Data/_Package.h"

// Consumer imports declarations/templates only. Implementation.hxx supplies
// the .hxx rollup ONCE, proving the file split works across a TU boundary.
ISC main() {
  _::TRollout<1, 1, 1> record = {}, decoded = {};
  record.count = 1;
  record.obs[0] = 3.25F;
  IUA wire[128] = {};
  IUC bytes = 0, used = 0;
  if (_::TRecordEncode(record, wire, sizeof(wire), &bytes) != _::RecordOK ||
      _::TRecordDecode(wire, bytes, decoded, &used) != _::RecordOK ||
      decoded.obs[0] != 3.25F || used != bytes)
    return 1;
  _::TAsciiPlot<1, 1> plot = {};
  CHA text[_::TAsciiPlot<1, 1>::Bytes] = {};
  if (_::TAsciiPlotPush(plot, 1) != _::PlotOK ||
      _::TAsciiPlotRender(plot, text, sizeof(text), &used) != _::PlotOK ||
      text[5] != '*')
    return 1;
  return 0;
}
