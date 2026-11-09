// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKPlotPlotH
#define _CrabsTKPlotPlotH
#include <_Config.h>
#include "../Exo/Image.h"
#include "../Exo/IDraw.h"
namespace CTK {
namespace Plot {

using Exo::VColor;
using Exo::CImage;

/* Bounded data ranges for an axis. */
struct TRange {
  FPD xmin;
  FPD xmax;
  FPD ymin;
  FPD ymax;
};

/* Status codes returned by the plot API. */
enum class PlotStatus : IUB {
  OK = 0,
  Invalid = 1,
  Empty = 2,
  NoSpace = 3,
  Full = 4,
};

/* A single 2D data series. Caller owns the arrays; the axis just reads them. */
struct Series2D {
  const FPD* x;
  const FPD* y;
  ISC count;
  VColor color;
};

/* A 3D point. */
struct Point3 {
  FPD x;
  FPD y;
  FPD z;
};

/* Bounded capacity for the axis data cache (so the axis can own copies when
   the caller's arrays are short-lived). Kept small and POD. */
inline constexpr ISC kMaxPoints = 1024;
inline constexpr ISC kMaxAxes = 4;
inline constexpr ISC kMaxSeries = 16;

/* An axes region inside a figure: a pixel rectangle plus a data->pixel
   transform. Title and axis label text are caller-provided CHA strings. */
class Axis {
 public:
  Axis();

  /* Bind this axis to a pixel rectangle within its parent figure. */
  void SetRect(ISC x0, ISC y0, ISC x1, ISC y1);

  ISC X0() const { return x0_; }
  ISC Y0() const { return y0_; }
  ISC X1() const { return x1_; }
  ISC Y1() const { return y1_; }

  /* Data range (the visible window). Auto-fit from data if not set. */
  void SetXRange(FPD lo, FPD hi);
  void SetYRange(FPD lo, FPD hi);
  FPD XMin() const { return range_.xmin; }
  FPD XMax() const { return range_.xmax; }
  FPD YMin() const { return range_.ymin; }
  FPD YMax() const { return range_.ymax; }

  /* Data (fx) -> pixel (px). The pixel origin is top-left; y is flipped so
     data increases upward. */
  ISC DataToPixX(FPD fx) const;
  ISC DataToPixY(FPD fy) const;

  /* Title / axis label text (rendered by the figure, optional). */
  void SetTitle(const CHA* text) { title_ = text; }
  void SetXLabel(const CHA* text) { xlabel_ = text; }
  void SetYLabel(const CHA* text) { ylabel_ = text; }
  const CHA* Title() const { return title_; }
  const CHA* XLabel() const { return xlabel_; }
  const CHA* YLabel() const { return ylabel_; }

  /* Grid + frame toggles. */
  void SetGrid(BOL on) { grid_ = on; }
  void SetFrame(BOL on) { frame_ = on; }

  /* Accessors for the style state (used by RenderFrame and tests). */
  VColor FrameColor() const { return frame_color_; }
  VColor GridColor() const { return grid_color_; }
  VColor BgColor() const { return bg_color_; }
  BOL Grid() const { return grid_; }
  BOL Frame() const { return frame_; }
  ISC XTicks() const { return x_ticks_; }
  ISC YTicks() const { return y_ticks_; }
  void SetXTicks(ISC n) { x_ticks_ = n; }
  void SetYTicks(ISC n) { y_ticks_ = n; }
  void SetFrameColor(VColor c) { frame_color_ = c; }
  void SetGridColor(VColor c) { grid_color_ = c; }
  void SetBgColor(VColor c) { bg_color_ = c; }

 private:
  ISC x0_ = 0;
  ISC y0_ = 0;
  ISC x1_ = 0;
  ISC y1_ = 0;
  TRange range_ = {0, 1, 0, 1};
  const CHA* title_ = nullptr;
  const CHA* xlabel_ = nullptr;
  const CHA* ylabel_ = nullptr;
  BOL grid_ = true;
  BOL frame_ = true;
  VColor frame_color_ = Exo::IGray;
  VColor grid_color_ = Exo::VColor{230, 230, 230, 255};
  VColor bg_color_ = Exo::IWhite;
  ISC x_ticks_ = 5;
  ISC y_ticks_ = 5;
};

/* A figure owns the CImage canvas and a small set of axes. */
class Figure {
 public:
  Figure(ISC width, ISC height, VColor bg = Exo::IWhite);

  ISC Width() const { return Width_; }
  ISC Height() const { return Height_; }
  CImage& Canvas() { return canvas_; }
  const CImage& Canvas() const { return canvas_; }

  /* Add an axis occupying a normalized sub-rect (0..1 fractions of the
     canvas). Returns the index, or -1 if full. */
  ISC AddAxis(FPD fx0, FPD fy0, FPD fx1, FPD fy1);

  Axis& AxisAt(ISC i) { return axes_[i]; }
  const Axis& AxisAt(ISC i) const { return axes_[i]; }
  ISC AxisCount() const { return axis_count_; }

  /* Set the current axis (the default target for chart calls). */
  void SetCurrentAxis(ISC i) { current_ = i; }
  Axis& Current() { return axes_[current_]; }
  const Axis& Current() const { return axes_[current_]; }

 private:
  ISC Width_;
  ISC Height_;
  CImage canvas_;
  Axis axes_[kMaxAxes];
  ISC axis_count_ = 0;
  ISC current_ = -1;
};

/* ---- 2D charts (rasterize into the axis's rect on the figure canvas) ---- */
PlotStatus PlotLine(Figure& fig, Axis& ax, const Series2D& s,
                    ISC thickness = 1);
PlotStatus PlotScatter(Figure& fig, Axis& ax, const Series2D& s,
                       ISC marker_radius = 2);
PlotStatus PlotArea(Figure& fig, Axis& ax, const Series2D& s);
PlotStatus PlotStairs(Figure& fig, Axis& ax, const Series2D& s,
                      ISC thickness = 1);
PlotStatus PlotStem(Figure& fig, Axis& ax, const Series2D& s);
PlotStatus PlotErrorbar(Figure& fig, Axis& ax, const Series2D& s,
                        const FPD* yerr, ISC thickness = 1);
PlotStatus PlotBar(Figure& fig, Axis& ax, const Series2D& s,
                   FPD bar_width);
PlotStatus PlotHist(Figure& fig, Axis& ax, const FPD* values, ISC count,
                    ISC bins, VColor color = Exo::IBlue);
PlotStatus PlotBoxplot(Figure& fig, Axis& ax, const Series2D& groups);

/* ---- 3D charts (project 3D -> 2D with a fixed isometric-ish view) ---- */
PlotStatus Plot3(Figure& fig, Axis& ax, const Point3* pts, ISC count,
                 VColor color = Exo::IBlue, ISC thickness = 1);
PlotStatus PlotSurf(Figure& fig, Axis& ax, const FPD* grid, ISC nx,
                    ISC ny, VColor color = Exo::IBlue);
PlotStatus PlotMesh(Figure& fig, Axis& ax, const FPD* grid, ISC nx,
                    ISC ny, VColor color = Exo::IBlue);
PlotStatus PlotContour(Figure& fig, Axis& ax, const FPD* grid, ISC nx,
                       ISC ny, ISC levels, VColor color = Exo::IBlue);

/* ---- Vector field ---- */
PlotStatus PlotQuiver(Figure& fig, Axis& ax, const FPD* x, const FPD* y,
                      const FPD* u, const FPD* v, ISC count,
                      FPD scale, VColor color = Exo::IGreen);

/* ---- Figure-level rendering helpers ---- */
/* Draw the frame, grid, and ticks for every axis. */
void RenderFrame(Figure& fig);

/* A tiny 5x7 bitmap font for axis titles/labels (A-Z, 0-9, a few symbols).
   Renders text at (px,py) on the figure. Returns pixels written. */
ISC DrawText(Figure& fig, Axis& ax, const CHA* text, ISC px, ISC py,
             VColor color);

/* Auto-fit an axis's x/y range from a series (pads by 5%). */
void AutoFit(Axis& ax, const Series2D& s);

}  // namespace Plot
}  // namespace CTK
#endif  //< _CrabsTKPlotPlotH
