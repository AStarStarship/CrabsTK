// Copyright AStarship <https://astarship.net>.
#pragma once
#include "../_Config.h"
#if _Seam == _CrabsTKDataCore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
namespace CTKDataTest {
using namespace ::_;
using namespace ::CTK;

IUC FuzzNext(IUC& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}
BOL DemoWrite(co_NilPHA* text, IUC bytes) {
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
IUC FuzzNext(IUC& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKDataCore
  _ATestBegin;
  D_COUT(Headingf("Testing Data Core..."));

  // Consumer: record encode/decode + plot round-trip.
  {
    TRollout<1, 1, 1> record = {}, decoded = {};
    record.count = 1;
    record.obs[0] = 3.25F;
    IUA wire[128] = {};
    IUC bytes = 0, used = 0;
    if (TRecordEncode(record, wire, sizeof(wire), &bytes) != RecordOK ||
        TRecordDecode(wire, bytes, decoded, &used) != RecordOK ||
        decoded.obs[0] != 3.25F || used != bytes)
      return "FAIL C01: record encode/decode round-trip";
    TAsciiPlot<1, 1> plot = {};
    CHA text[TAsciiPlot<1, 1>::Bytes] = {};
    if (TAsciiPlotPush(plot, 1) != PlotOK ||
        TAsciiPlotRender(plot, text, sizeof(text), &used) != PlotOK ||
        text[5] != '*')
      return "FAIL C02: ascii plot push/render";
  }

  // Demo: synthetic seam inputs (NOT training results).
  {
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
      for (IUC j = 0; j < 12; ++j)
        if (TAsciiPlotPush(plot, Values[series][j]) != PlotOK)
          return "FAIL D01: ascii plot push";
      CHA text[TAsciiPlot<12, 5>::Bytes] = {};
      IUC written = 0;
      if (TAsciiPlotRender(plot, text, sizeof(text), &written) != PlotOK)
        return "FAIL D02: ascii plot render";
      if (!DemoLabel(Names[series]) || !DemoLabel("\n") ||
          !DemoWrite(text, written))
        return "FAIL D03: demo output write";
    }
  }

  // Fuzz: 10000 mutated decoder inputs + 10000 plot pushes.
  {
    TRollout<2, 2, 2> source = {}, output = {};
    source.count = 2;
    source.run_id = 44;
    source.terminals[1] = 1;
    source.action_mask[0] = source.action_mask[3] = 1;
    IUA original[256] = {};
    IUC bytes = 0;
    if (TRecordEncode(source, original, sizeof(original), &bytes) != RecordOK)
      return "FAIL F01: fuzz encode";
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
          return "FAIL F02: fuzz decode OK invariant";
      } else {
        if (consumed) return "FAIL F03: fuzz decode err consumed";
        for (IUC i = 0; i < sizeof(output); ++i)
          if (raw[i] != before[i]) return "FAIL F04: fuzz decode err mutated output";
      }
    }
    TAsciiPlot<17, 19> plot = {};
    FPC reference[17] = {};
    IUC count = 0;
    for (IUC iteration = 0; iteration < 10000; ++iteration) {
      FPC value = RecordDetail::Float(FuzzNext(state));
      PlotStatus status = TAsciiPlotPush(plot, value);
      if (!TAsciiPlotFinite(value)) {
        if (status != PlotInvalid) return "FAIL F05: fuzz plot non-finite";
        continue;
      }
      if (status != PlotOK) return "FAIL F06: fuzz plot push";
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
          return "FAIL F07: fuzz plot value mismatch";
      FPC minimum = reference[0], maximum = reference[0];
      for (IUC i = 1; i < count; ++i) {
        if (reference[i] < minimum) minimum = reference[i];
        if (reference[i] > maximum) maximum = reference[i];
      }
      FPC actual_min = 0, actual_max = 0;
      if (TAsciiPlotRange(plot, &actual_min, &actual_max) != PlotOK ||
          actual_min != minimum || actual_max != maximum)
        return "FAIL F08: fuzz plot range";
      CHA raster[TAsciiPlot<17, 19>::Bytes] = {};
      IUC written = 0;
      if (TAsciiPlotRender(plot, raster, sizeof(raster), &written) != PlotOK ||
          written != sizeof(raster) - 1 || raster[written] != 0)
        return "FAIL F09: fuzz plot render";
      for (IUC i = 0; i < written; ++i)
        if (IUA(raster[i]) > 127) return "FAIL F10: fuzz plot non-ASCII";
    }
  }
#endif
  return _NilP;
}

}  //< namespace CTKDataTest
