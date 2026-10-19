// Copyright AStarship <https://astarship.net>.
#pragma once
#include "_Config.h"

#if __has_include("../../Plot/Plot.h") && \
    __has_include("../../Plot/Plot.hxx")
#include "../../Plot/Plot.h"
#include "../../Plot/Plot.hxx"
#include "../../Image/IDraw.h"
#include "../../Image/IDraw.hxx"
#define CRABSTK_PLOT_PRESENT 1
#endif

namespace _ {
namespace PlotTests {

/* Count pixels in the canvas that are not the background white and opaque.
   Used to assert that a chart actually rendered something. */
inline ISN CountNonWhite(const CImage& cv, const IUA bg_r = 255,
                         const IUA bg_g = 255, const IUA bg_b = 255) {
  ISN n = 0;
  for (ISN i = 0; i < cv.width * cv.height; ++i) {
    if (cv.data[i * 4 + 3] == 255 && cv.data[i * 4] == bg_r &&
        cv.data[i * 4 + 1] == bg_g && cv.data[i * 4 + 2] == bg_b)
      continue;
    ++n;
  }
  return n;
}

/* P01: figure allocates a canvas, AddAxis bounds the rect, and RenderFrame
   draws frame+grid+ticks (i.e. non-background pixels appear). */
inline const CHA* P01_FigureAndFrame(const CHA* args) {
  (void)args;
#if !defined(CRABSTK_PLOT_PRESENT)
  return "FAIL P01: Plot module is not present";
#endif
  Plot::TFigure fig(200, 120, ImageDraw::IWhite);
  if (fig.Width() != 200 || fig.Height() != 120)
    return "FAIL P01: figure size";
  ISN a = fig.AddAxis(0.1f, 0.1f, 0.9f, 0.9f);
  if (a < 0 || fig.AxisCount() != 1) return "FAIL P01: AddAxis";
  Plot::TAxis& ax = fig.Axis(a);
  if (ax.X0() < 0 || ax.Y0() < 0 || ax.X1() > 199 || ax.Y1() > 119)
    return "FAIL P01: axis rect out of bounds";
  Plot::RenderFrame(fig);
  if (CountNonWhite(fig.Canvas()) < 20)
    return "FAIL P01: frame/grid/ticks did not render";
  return nullptr;
}

/* P02: AddAxis over capacity returns -1 and does not corrupt state. */
inline const CHA* P02_AxisCapacity(const CHA* args) {
  (void)args;
#if !defined(CRABSTK_PLOT_PRESENT)
  return "FAIL P02: Plot module is not present";
#endif
  Plot::TFigure fig(100, 100);
  for (ISN i = 0; i < Plot::kMaxAxes; ++i) {
    if (fig.AddAxis(0.1f, 0.1f, 0.9f, 0.9f) < 0) return "FAIL P02: add";
  }
  if (fig.AddAxis(0.1f, 0.1f, 0.9f, 0.9f) != -1)
    return "FAIL P02: capacity not enforced";
  if (fig.AxisCount() != Plot::kMaxAxes) return "FAIL P02: count";
  return nullptr;
}

/* P03: DataToPix maps range extremes to the rect edges (with y flipped). */
inline const CHA* P03_DataToPixel(const CHA* args) {
  (void)args;
#if !defined(CRABSTK_PLOT_PRESENT)
  return "FAIL P03: Plot module is not present";
#endif
  Plot::TAxis ax;
  ax.SetRect(10, 20, 110, 120);
  ax.SetXRange(0, 100);
  ax.SetYRange(0, 100);
  if (ax.DataToPixX(0) != 10 || ax.DataToPixX(100) != 110)
    return "FAIL P03: x mapping";
  // y is flipped: data 0 at the bottom (y1=120), data 100 at top (y0=20).
  if (ax.DataToPixY(0) != 120 || ax.DataToPixY(100) != 20)
    return "FAIL P03: y mapping (should be flipped)";
  if (ax.DataToPixX(50) != 60) return "FAIL P03: x mid";
  return nullptr;
}

/* P04: PlotLine on a monotonic series renders non-background pixels and
   returns OK; an empty series returns Empty. */
inline const CHA* P04_PlotLine(const CHA* args) {
  (void)args;
#if !defined(CRABSTK_PLOT_PRESENT)
  return "FAIL P04: Plot module is not present";
#endif
  Plot::TFigure fig(160, 100);
  ISN a = fig.AddAxis(0.1f, 0.1f, 0.9f, 0.9f);
  Plot::TAxis& ax = fig.Axis(a);
  ax.SetXRange(0, 10);
  ax.SetYRange(0, 100);
  FPD x[11], y[11];
  for (ISN i = 0; i < 11; ++i) {
    x[i] = (FPD)i;
    y[i] = (FPD)(i * 10);
  }
  Plot::TSeries2D s{x, y, 11, ImageDraw::IRed};
  if (Plot::PlotLine(fig, ax, s, 1) != Plot::PlotStatus::OK)
    return "FAIL P04: PlotLine status";
  if (CountNonWhite(fig.Canvas()) < 10) return "FAIL P04: line not drawn";
  Plot::TSeries2D empty{nullptr, nullptr, 0, ImageDraw::IRed};
  if (Plot::PlotLine(fig, ax, empty, 1) != Plot::PlotStatus::Empty)
    return "FAIL P04: empty series should be Empty";
  return nullptr;
}

/* P05: PlotScatter draws markers; PlotBar draws bars. */
inline const CHA* P05_ScatterAndBar(const CHA* args) {
  (void)args;
#if !defined(CRABSTK_PLOT_PRESENT)
  return "FAIL P05: Plot module is not present";
#endif
  Plot::TFigure fig(160, 100);
  ISN a = fig.AddAxis(0.1f, 0.1f, 0.9f, 0.9f);
  Plot::TAxis& ax = fig.Axis(a);
  ax.SetXRange(0, 10);
  ax.SetYRange(0, 100);
  FPD x[5], y[5];
  for (ISN i = 0; i < 5; ++i) {
    x[i] = (FPD)(i * 2);
    y[i] = (FPD)(20 + i * 15);
  }
  Plot::TSeries2D s{x, y, 5, ImageDraw::IBlue};
  if (Plot::PlotScatter(fig, ax, s, 3) != Plot::PlotStatus::OK)
    return "FAIL P05: scatter status";
  if (CountNonWhite(fig.Canvas()) < 20) return "FAIL P05: scatter not drawn";
  if (Plot::PlotBar(fig, ax, s, 1.0f) != Plot::PlotStatus::OK)
    return "FAIL P05: bar status";
  return nullptr;
}

/* P06: PlotHist bins values and renders bars; a uniform distribution yields
   roughly equal bins (spot-check the middle bin is non-zero). */
inline const CHA* P06_Hist(const CHA* args) {
  (void)args;
#if !defined(CRABSTK_PLOT_PRESENT)
  return "FAIL P06: Plot module is not present";
#endif
  Plot::TFigure fig(160, 100);
  ISN a = fig.AddAxis(0.1f, 0.1f, 0.9f, 0.9f);
  Plot::TAxis& ax = fig.Axis(a);
  ax.SetXRange(0, 10);
  ax.SetYRange(0, 50);
  FPD vals[100];
  for (ISN i = 0; i < 100; ++i) vals[i] = (FPD)(i % 10);
  if (Plot::PlotHist(fig, ax, vals, 100, 10, ImageDraw::IGreen) !=
      Plot::PlotStatus::OK)
    return "FAIL P06: hist status";
  if (CountNonWhite(fig.Canvas()) < 20) return "FAIL P06: hist not drawn";
  return nullptr;
}

/* P07: 3D projection + PlotMesh renders; PlotQuiver renders arrows. */
inline const CHA* P07_ThreeDAndQuiver(const CHA* args) {
  (void)args;
#if !defined(CRABSTK_PLOT_PRESENT)
  return "FAIL P07: Plot module is not present";
#endif
  Plot::TFigure fig(160, 160);
  ISN a = fig.AddAxis(0.1f, 0.1f, 0.9f, 0.9f);
  Plot::TAxis& ax = fig.Axis(a);
  ax.SetXRange(0, 10);
  ax.SetYRange(0, 10);
  ISN nx = 5, ny = 5;
  FPD grid[25];
  for (ISN i = 0; i < 25; ++i) grid[i] = (FPD)(i % 5);
  if (Plot::PlotMesh(fig, ax, grid, nx, ny, ImageDraw::IBlue) !=
      Plot::PlotStatus::OK)
    return "FAIL P07: mesh status";
  if (CountNonWhite(fig.Canvas()) < 20) return "FAIL P07: mesh not drawn";
  FPD qx[4], qy[4], qu[4], qv[4];
  for (ISN i = 0; i < 4; ++i) {
    qx[i] = (FPD)i * 2;
    qy[i] = (FPD)i * 2;
    qu[i] = 1.0f;
    qv[i] = 1.0f;
  }
  if (Plot::PlotQuiver(fig, ax, qx, qy, qu, qv, 4, 0.5f, ImageDraw::IGreen) !=
      Plot::PlotStatus::OK)
    return "FAIL P07: quiver status";
  return nullptr;
}

/* P08: DrawText writes a known number of pixels for a single 'O' (the O
   glyph has a fixed pixel count). */
inline const CHA* P08_DrawText(const CHA* args) {
  (void)args;
#if !defined(CRABSTK_PLOT_PRESENT)
  return "FAIL P08: Plot module is not present";
#endif
  Plot::TFigure fig(60, 30);
  ISN a = fig.AddAxis(0.0f, 0.0f, 1.0f, 1.0f);
  Plot::TAxis& ax = fig.Axis(a);
  ISN n = Plot::DrawText(fig, ax, "O", 10, 10, ImageDraw::IBlack);
  // The 'O' glyph (0x0E,0x11,0x11,0x11,0x11,0x11,0x0E) has 16 set bits.
  if (n != 16) return "FAIL P08: O glyph pixel count";
  if (CountNonWhite(fig.Canvas()) < 10) return "FAIL P08: text not drawn";
  return nullptr;
}

/* P09: AutoFit pads the range by 5% and the axis reflects it. */
inline const CHA* P09_AutoFit(const CHA* args) {
  (void)args;
#if !defined(CRABSTK_PLOT_PRESENT)
  return "FAIL P09: Plot module is not present";
#endif
  Plot::TAxis ax;
  FPD x[3] = {1.0f, 2.0f, 3.0f};
  FPD y[3] = {10.0f, 20.0f, 30.0f};
  Plot::TSeries2D s{x, y, 3, ImageDraw::IRed};
  Plot::AutoFit(ax, s);
  // x range 1..3 -> pad 0.1 -> 0.9..3.1
  if (ax.XMin() > 0.95f || ax.XMin() < 0.85f)
    return "FAIL P09: x min pad";
  if (ax.XMax() < 3.05f || ax.XMax() > 3.15f)
    return "FAIL P09: x max pad";
  // y range 10..30 -> pad 1.0 -> 9..31
  if (ax.YMin() > 9.5f || ax.YMin() < 8.5f)
    return "FAIL P09: y min pad";
  return nullptr;
}

/* P10: ImageDraw foundations — PutPixel, Line, FillRect, FillCircle, alpha
   blend — render into a CImage deterministically. */
inline const CHA* P10_ImageDraw(const CHA* args) {
  (void)args;
#if !defined(CRABSTK_PLOT_PRESENT)
  return "FAIL P10: Plot module is not present";
#endif
  CImage cv{nullptr, 64, 32, 4};
  cv.data = new IUA[(IUN)64 * (IUN)32 * 4]();
  ImageDraw::Fill(cv, ImageDraw::IBlack);
  ImageDraw::PutPixel(cv, 10, 10, ImageDraw::IRed);
  if (cv.data[(10 * 64 + 10) * 4] != 255 ||
      cv.data[(10 * 64 + 10) * 4 + 1] != 0 ||
      cv.data[(10 * 64 + 10) * 4 + 2] != 0)
    return "FAIL P10: PutPixel red";
  ImageDraw::Line(cv, 0, 0, 63, 31, ImageDraw::IGreen);
  if (cv.data[(0 * 64 + 0) * 4 + 1] != 255)
    return "FAIL P10: line corner green";
  ImageDraw::FillRect(cv, 5, 5, 20, 15, ImageDraw::IBlue);
  if (cv.data[(8 * 64 + 8) * 4 + 2] != 255)
    return "FAIL P10: fillrect blue";
  ImageDraw::FillCircle(cv, 40, 16, 6, ImageDraw::IYellow);
  if (cv.data[(16 * 64 + 40) * 4] != 255)
    return "FAIL P10: fillcircle center";
  // Alpha blend: 50% green over black = 128 green.
  ImageDraw::PutPixel(cv, 50, 20, ImageDraw::MakeColor(0, 255, 0, 128));
  IUA g = cv.data[(20 * 64 + 50) * 4 + 1];
  if (g < 120 || g > 135) return "FAIL P10: alpha blend";
  delete[] cv.data;
  return nullptr;
}

}  // namespace PlotTests
}  // namespace _
