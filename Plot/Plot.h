// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_PLOT_PLOT_DECL
#define CRABSTK_PLOT_PLOT_DECL
#include "_Config.h"
#include "../Image/Image.h"
#include "../Image/IDraw.h"

namespace _ {
namespace Plot {

using ImageDraw::TColor;

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
struct TSeries2D {
  const FPD* x;
  const FPD* y;
  ISN count;
  TColor color;
};

/* A 3D point. */
struct TPoint3 {
  FPD x;
  FPD y;
  FPD z;
};

/* Bounded capacity for the axis data cache (so the axis can own copies when
   the caller's arrays are short-lived). Kept small and POD. */
inline constexpr ISN kMaxPoints = 1024;
inline constexpr ISN kMaxAxes = 4;
inline constexpr ISN kMaxSeries = 16;

/* An axes region inside a figure: a pixel rectangle plus a data->pixel
   transform. Title and axis label text are caller-provided CHA strings. */
class TAxis {
 public:
  TAxis();

  /* Bind this axis to a pixel rectangle within its parent figure. */
  void SetRect(ISN x0, ISN y0, ISN x1, ISN y1);

  ISN X0() const { return x0_; }
  ISN Y0() const { return y0_; }
  ISN X1() const { return x1_; }
  ISN Y1() const { return y1_; }

  /* Data range (the visible window). Auto-fit from data if not set. */
  void SetXRange(FPD lo, FPD hi);
  void SetYRange(FPD lo, FPD hi);
  FPD XMin() const { return range_.xmin; }
  FPD XMax() const { return range_.xmax; }
  FPD YMin() const { return range_.ymin; }
  FPD YMax() const { return range_.ymax; }

  /* Data (fx) -> pixel (px). The pixel origin is top-left; y is flipped so
     data increases upward. */
  ISN DataToPixX(FPD fx) const;
  ISN DataToPixY(FPD fy) const;

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
  TColor FrameColor() const { return frame_color_; }
  TColor GridColor() const { return grid_color_; }
  TColor BgColor() const { return bg_color_; }
  BOL Grid() const { return grid_; }
  BOL Frame() const { return frame_; }
  ISN XTicks() const { return x_ticks_; }
  ISN YTicks() const { return y_ticks_; }
  void SetXTicks(ISN n) { x_ticks_ = n; }
  void SetYTicks(ISN n) { y_ticks_ = n; }
  void SetFrameColor(TColor c) { frame_color_ = c; }
  void SetGridColor(TColor c) { grid_color_ = c; }
  void SetBgColor(TColor c) { bg_color_ = c; }

 private:
  ISN x0_ = 0;
  ISN y0_ = 0;
  ISN x1_ = 0;
  ISN y1_ = 0;
  TRange range_ = {0, 1, 0, 1};
  const CHA* title_ = nullptr;
  const CHA* xlabel_ = nullptr;
  const CHA* ylabel_ = nullptr;
  BOL grid_ = true;
  BOL frame_ = true;
  TColor frame_color_ = ImageDraw::IGray;
  TColor grid_color_ = ImageDraw::TColor{230, 230, 230, 255};
  TColor bg_color_ = ImageDraw::IWhite;
  ISN x_ticks_ = 5;
  ISN y_ticks_ = 5;
};

/* A figure owns the CImage canvas and a small set of axes. */
class TFigure {
 public:
  TFigure(ISN width, ISN height, TColor bg = ImageDraw::IWhite);

  ISN Width() const { return Width_; }
  ISN Height() const { return Height_; }
  CImage& Canvas() { return canvas_; }
  const CImage& Canvas() const { return canvas_; }

  /* Add an axis occupying a normalized sub-rect (0..1 fractions of the
     canvas). Returns the index, or -1 if full. */
  ISN AddAxis(FPD fx0, FPD fy0, FPD fx1, FPD fy1);

  TAxis& Axis(ISN i) { return axes_[i]; }
  const TAxis& Axis(ISN i) const { return axes_[i]; }
  ISN AxisCount() const { return axis_count_; }

  /* Set the current axis (the default target for chart calls). */
  void SetCurrentAxis(ISN i) { current_ = i; }
  TAxis& Current() { return axes_[current_]; }
  const TAxis& Current() const { return axes_[current_]; }

 private:
  ISN Width_;
  ISN Height_;
  CImage canvas_;
  TAxis axes_[kMaxAxes];
  ISN axis_count_ = 0;
  ISN current_ = -1;
};

/* ---- 2D charts (rasterize into the axis's rect on the figure canvas) ---- */
PlotStatus PlotLine(TFigure& fig, TAxis& ax, const TSeries2D& s,
                    ISN thickness = 1);
PlotStatus PlotScatter(TFigure& fig, TAxis& ax, const TSeries2D& s,
                       ISN marker_radius = 2);
PlotStatus PlotArea(TFigure& fig, TAxis& ax, const TSeries2D& s);
PlotStatus PlotStairs(TFigure& fig, TAxis& ax, const TSeries2D& s,
                      ISN thickness = 1);
PlotStatus PlotStem(TFigure& fig, TAxis& ax, const TSeries2D& s);
PlotStatus PlotErrorbar(TFigure& fig, TAxis& ax, const TSeries2D& s,
                        const FPD* yerr, ISN thickness = 1);
PlotStatus PlotBar(TFigure& fig, TAxis& ax, const TSeries2D& s,
                   FPD bar_width);
PlotStatus PlotHist(TFigure& fig, TAxis& ax, const FPD* values, ISN count,
                    ISN bins, TColor color = ImageDraw::IBlue);
PlotStatus PlotBoxplot(TFigure& fig, TAxis& ax, const TSeries2D& groups);

/* ---- 3D charts (project 3D -> 2D with a fixed isometric-ish view) ---- */
PlotStatus Plot3(TFigure& fig, TAxis& ax, const TPoint3* pts, ISN count,
                 TColor color = ImageDraw::IBlue, ISN thickness = 1);
PlotStatus PlotSurf(TFigure& fig, TAxis& ax, const FPD* grid, ISN nx,
                    ISN ny, TColor color = ImageDraw::IBlue);
PlotStatus PlotMesh(TFigure& fig, TAxis& ax, const FPD* grid, ISN nx,
                    ISN ny, TColor color = ImageDraw::IBlue);
PlotStatus PlotContour(TFigure& fig, TAxis& ax, const FPD* grid, ISN nx,
                       ISN ny, ISN levels, TColor color = ImageDraw::IBlue);

/* ---- Vector field ---- */
PlotStatus PlotQuiver(TFigure& fig, TAxis& ax, const FPD* x, const FPD* y,
                      const FPD* u, const FPD* v, ISN count,
                      FPD scale, TColor color = ImageDraw::IGreen);

/* ---- Figure-level rendering helpers ---- */
/* Draw the frame, grid, and ticks for every axis. */
void RenderFrame(TFigure& fig);

/* A tiny 5x7 bitmap font for axis titles/labels (A-Z, 0-9, a few symbols).
   Renders text at (px,py) on the figure. Returns pixels written. */
ISN DrawText(TFigure& fig, TAxis& ax, const CHA* text, ISN px, ISN py,
             TColor color);

/* Auto-fit an axis's x/y range from a series (pads by 5%). */
void AutoFit(TAxis& ax, const TSeries2D& s);

}  // namespace Plot
}  // namespace _
#endif
