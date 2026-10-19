// Copyright AStarship <https://astarship.net>.
#include "_Config.h"
#include <unistd.h>
#include "../../Data/_Package.hxx"

namespace _ {

IUC FuzzNext(IUC& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

}  //< namespace _

ISC main() {
  using namespace _;
  TRollout<2, 2, 2> source = {}, output = {};
  source.count = 2;
  source.run_id = 44;
  source.terminals[1] = 1;
  source.action_mask[0] = source.action_mask[3] = 1;
  IUA original[256] = {};
  IUC bytes = 0;
  if (TRecordEncode(source, original, sizeof(original), &bytes) != RecordOK)
    return 1;
  IUC state = 0x5eed1234U;
  for (IUC iteration = 0; iteration < 10000; ++iteration) {
    IUA mutated[256] = {};
    for (IUC i = 0; i < bytes; ++i) mutated[i] = original[i];
    for (IUC i = 0; i < 3; ++i)
      mutated[FuzzNext(state) % bytes] ^= IUA(FuzzNext(state));
    if (iteration % 3 == 0)
      RecordDetail::Store(mutated + 64, RecordDetail::Checksum(mutated, bytes), 4);
    IUC length = FuzzNext(state) % (bytes + 1);
    output = {};
    output.run_id = 777;
    IUA before[sizeof(output)] = {};
    const IUA* raw = reinterpret_cast<const IUA*>(&output);
    for (IUC i = 0; i < sizeof(output); ++i) before[i] = raw[i];
    IUC consumed = 999;
    RecordStatus status = TRecordDecode(mutated, length, output, &consumed);
    if (status == RecordOK) {
      if (!consumed || consumed > length || !output.count || output.count > 2)
        return 1;
    } else {
      if (consumed) return 1;
      for (IUC i = 0; i < sizeof(output); ++i)
        if (raw[i] != before[i]) return 1;
    }
  }

  TAsciiPlot<17, 19> plot = {};
  FPC reference[17] = {};
  IUC count = 0;
  for (IUC iteration = 0; iteration < 10000; ++iteration) {
    FPC value = RecordDetail::Float(FuzzNext(state));
    PlotStatus status = TAsciiPlotPush(plot, value);
    if (!TAsciiPlotFinite(value)) {
      if (status != PlotInvalid) return 1;
      continue;
    }
    if (status != PlotOK) return 1;
    if (count < 17) {
      reference[count++] = value;
    } else {
      for (IUC i = 0; i < 16; ++i) reference[i] = reference[i + 1];
      reference[16] = value;
    }
    IUC first = count == 17 ? plot.next : 0;
    for (IUC i = 0; i < count; ++i)
      if (RecordDetail::FloatBits(reference[i]) !=
          RecordDetail::FloatBits(plot.values[(first + i) % 17]))
        return 1;
    FPC minimum = reference[0], maximum = reference[0];
    for (IUC i = 1; i < count; ++i) {
      if (reference[i] < minimum) minimum = reference[i];
      if (reference[i] > maximum) maximum = reference[i];
    }
    FPC actual_min = 0, actual_max = 0;
    if (TAsciiPlotRange(plot, &actual_min, &actual_max) != PlotOK ||
        actual_min != minimum || actual_max != maximum)
      return 1;
    CHA raster[TAsciiPlot<17, 19>::Bytes] = {};
    IUC written = 0;
    if (TAsciiPlotRender(plot, raster, sizeof(raster), &written) != PlotOK ||
        written != sizeof(raster) - 1 || raster[written] != 0)
      return 1;
    for (IUC i = 0; i < written; ++i)
      if (IUA(raster[i]) > 127) return 1;
  }
  const CHA message[] = "PASS: 10000 mutated decoder inputs + 10000 plot pushes\n";
  return write(1, message, sizeof(message) - 1) == ISW(sizeof(message) - 1) ? 0 : 2;
}
