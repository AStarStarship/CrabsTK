// Copyright AStarship <https://astarship.net>.
#include "Plot.h"
#include <math.h>

namespace _ {
namespace Plot {

/* ============================ TAxis ============================ */
TAxis::TAxis() = default;

void TAxis::SetRect(ISN x0, ISN y0, ISN x1, ISN y1) {
  x0_ = x0;
  y0_ = y0;
  x1_ = x1;
  y1_ = y1;
}
void TAxis::SetXRange(FPD lo, FPD hi) {
  range_.xmin = lo;
  range_.xmax = hi;
}
void TAxis::SetYRange(FPD lo, FPD hi) {
  range_.ymin = lo;
  range_.ymax = hi;
}
ISN TAxis::DataToPixX(FPD fx) const {
  if (range_.xmax == range_.xmin) return (x0_ + x1_) / 2;
  FPD t = (fx - range_.xmin) / (range_.xmax - range_.xmin);
  return x0_ + (ISN)(t * (x1_ - x0_));
}
ISN TAxis::DataToPixY(FPD fy) const {
  if (range_.ymax == range_.ymin) return (y0_ + y1_) / 2;
  FPD t = (fy - range_.ymin) / (range_.ymax - range_.ymin);
  // y is flipped: data max is at the top (small pixel y).
  return y1_ - (ISN)(t * (y1_ - y0_));
}

void AutoFit(TAxis& ax, const TSeries2D& s) {
  if (s.x == nullptr || s.y == nullptr || s.count == 0) return;
  FPD xlo = s.x[0], xhi = s.x[0], ylo = s.y[0], yhi = s.y[0];
  for (ISN i = 1; i < s.count; ++i) {
    if (s.x[i] < xlo) xlo = s.x[i];
    if (s.x[i] > xhi) xhi = s.x[i];
    if (s.y[i] < ylo) ylo = s.y[i];
    if (s.y[i] > yhi) yhi = s.y[i];
  }
  FPD xpad = (xhi - xlo) * 0.05f;
  FPD ypad = (yhi - ylo) * 0.05f;
  if (xpad == 0) xpad = 1.0f;
  if (ypad == 0) ypad = 1.0f;
  ax.SetXRange(xlo - xpad, xhi + xpad);
  ax.SetYRange(ylo - ypad, yhi + ypad);
}

/* ============================ TFigure ============================ */
TFigure::TFigure(ISN width, ISN height, TColor bg)
    : Width_(width), Height_(height),
      canvas_{nullptr, width, height, 4} {
  canvas_.data = new IUA[(IUN)width * (IUN)height * 4]();
  ImageDraw::Fill(canvas_, bg);
}

ISN TFigure::AddAxis(FPD fx0, FPD fy0, FPD fx1, FPD fy1) {
  if (axis_count_ >= kMaxAxes) return -1;
  ISN x0 = (ISN)(fx0 * Width_);
  ISN y0 = (ISN)(fy0 * Height_);
  ISN x1 = (ISN)(fx1 * Width_);
  ISN y1 = (ISN)(fy1 * Height_);
  if (x0 < 0) x0 = 0;
  if (y0 < 0) y0 = 0;
  if (x1 > Width_) x1 = Width_ - 1;
  if (y1 > Height_) y1 = Height_ - 1;
  axes_[axis_count_].SetRect(x0, y0, x1, y1);
  if (current_ < 0) current_ = axis_count_;
  return axis_count_++;
}

/* ===================== 5x7 bitmap mini-font ===================== */
/* Glyphs: A-Z, 0-9, space, '-', '.', '+', '/', ':', '(', ')', 'a'.
   Each glyph is 5 columns x 7 rows, stored as 7 bytes (one per row, low
   5 bits). This is a fixed table — no heap, no std. */
static const IUB kGlyphs[96][7] = {
  /* 0x00 ' ' */ {0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  /* ! */ {0x04,0x04,0x04,0x04,0x04,0x00,0x04},
  /* " */ {0x0A,0x0A,0x00,0x00,0x00,0x00,0x00},
  /* # */ {0x0A,0x0A,0x1F,0x0A,0x1F,0x0A,0x0A},
  /* $ */ {0x04,0x0F,0x14,0x0E,0x05,0x1E,0x04},
  /* % */ {0x18,0x18,0x04,0x02,0x01,0x19,0x19},
  /* & */ {0x04,0x0A,0x05,0x04,0x0D,0x0A,0x0B},
  /* ' */ {0x04,0x04,0x00,0x00,0x00,0x00,0x00},
  /* ( */ {0x02,0x04,0x08,0x08,0x08,0x04,0x02},
  /* ) */ {0x08,0x04,0x02,0x02,0x02,0x04,0x08},
  /* * */ {0x00,0x04,0x0A,0x0F,0x0A,0x04,0x00},
  /* + */ {0x00,0x04,0x04,0x1F,0x04,0x04,0x00},
  /* , */ {0x00,0x00,0x00,0x00,0x04,0x04,0x08},
  /* - */ {0x00,0x00,0x00,0x1F,0x00,0x00,0x00},
  /* . */ {0x00,0x00,0x00,0x00,0x00,0x04,0x04},
  /* / */ {0x01,0x01,0x02,0x04,0x08,0x10,0x10},
  /* 0 */ {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E},
  /* 1 */ {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E},
  /* 2 */ {0x0E,0x11,0x01,0x02,0x04,0x08,0x1F},
  /* 3 */ {0x1F,0x02,0x04,0x02,0x01,0x11,0x0E},
  /* 4 */ {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02},
  /* 5 */ {0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E},
  /* 6 */ {0x06,0x08,0x10,0x1E,0x11,0x11,0x0E},
  /* 7 */ {0x1F,0x01,0x02,0x04,0x08,0x08,0x08},
  /* 8 */ {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E},
  /* 9 */ {0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C},
  /* : */ {0x00,0x04,0x04,0x00,0x04,0x04,0x00},
  /* ; */ {0x00,0x04,0x04,0x00,0x04,0x04,0x08},
  /* < */ {0x01,0x02,0x04,0x08,0x04,0x02,0x01},
  /* = */ {0x00,0x00,0x1F,0x00,0x1F,0x00,0x00},
  /* > */ {0x10,0x08,0x04,0x02,0x04,0x08,0x10},
  /* ? */ {0x0E,0x11,0x01,0x02,0x04,0x00,0x04},
  /* @ */ {0x0E,0x11,0x1F,0x15,0x17,0x10,0x0E},
  /* A */ {0x04,0x0A,0x11,0x1F,0x11,0x11,0x11},
  /* B */ {0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E},
  /* C */ {0x0E,0x11,0x10,0x10,0x10,0x11,0x0E},
  /* D */ {0x1C,0x12,0x11,0x11,0x11,0x12,0x1C},
  /* E */ {0x1F,0x10,0x10,0x1E,0x10,0x10,0x1F},
  /* F */ {0x1F,0x10,0x10,0x1E,0x10,0x10,0x10},
  /* G */ {0x0E,0x11,0x10,0x17,0x11,0x11,0x0F},
  /* H */ {0x11,0x11,0x11,0x1F,0x11,0x11,0x11},
  /* I */ {0x0E,0x04,0x04,0x04,0x04,0x04,0x0E},
  /* J */ {0x07,0x02,0x02,0x02,0x02,0x12,0x0C},
  /* K */ {0x11,0x12,0x14,0x18,0x14,0x12,0x11},
  /* L */ {0x10,0x10,0x10,0x10,0x10,0x10,0x1F},
  /* M */ {0x11,0x1B,0x15,0x15,0x11,0x11,0x11},
  /* N */ {0x11,0x19,0x15,0x13,0x11,0x11,0x11},
  /* O */ {0x0E,0x11,0x11,0x11,0x11,0x11,0x0E},
  /* P */ {0x1E,0x11,0x11,0x1E,0x10,0x10,0x10},
  /* Q */ {0x0E,0x11,0x11,0x11,0x15,0x12,0x0D},
  /* R */ {0x1E,0x11,0x11,0x1E,0x14,0x12,0x11},
  /* S */ {0x0F,0x10,0x10,0x0E,0x01,0x01,0x1E},
  /* T */ {0x1F,0x04,0x04,0x04,0x04,0x04,0x04},
  /* U */ {0x11,0x11,0x11,0x11,0x11,0x11,0x0E},
  /* V */ {0x11,0x11,0x11,0x11,0x11,0x0A,0x04},
  /* W */ {0x11,0x11,0x11,0x15,0x15,0x1B,0x11},
  /* X */ {0x11,0x11,0x0A,0x04,0x0A,0x11,0x11},
  /* Y */ {0x11,0x11,0x11,0x0A,0x04,0x04,0x04},
  /* Z */ {0x1F,0x01,0x02,0x04,0x08,0x10,0x1F},
  /* [ */ {0x0E,0x08,0x08,0x08,0x08,0x08,0x0E},
  /* \ */ {0x10,0x10,0x08,0x04,0x02,0x01,0x01},
  /* ] */ {0x0E,0x02,0x02,0x02,0x02,0x02,0x0E},
  /* ^ */ {0x04,0x0A,0x11,0x00,0x00,0x00,0x00},
  /* _ */ {0x00,0x00,0x00,0x00,0x00,0x00,0x1F},
  /* ` */ {0x08,0x04,0x00,0x00,0x00,0x00,0x00},
  /* a */ {0x00,0x00,0x0E,0x01,0x0F,0x11,0x0F},
  /* b */ {0x10,0x10,0x1E,0x11,0x11,0x11,0x1E},
  /* c */ {0x00,0x00,0x0E,0x11,0x10,0x11,0x0E},
  /* d */ {0x01,0x01,0x0F,0x11,0x11,0x11,0x0F},
  /* e */ {0x00,0x00,0x0E,0x11,0x1F,0x10,0x0E},
  /* f */ {0x06,0x09,0x08,0x1E,0x08,0x08,0x08},
  /* g */ {0x00,0x0F,0x11,0x11,0x0F,0x01,0x0E},
  /* h */ {0x10,0x10,0x1E,0x11,0x11,0x11,0x11},
  /* i */ {0x04,0x00,0x0C,0x04,0x04,0x04,0x0E},
  /* j */ {0x02,0x00,0x06,0x02,0x02,0x12,0x0C},
  /* k */ {0x08,0x08,0x14,0x18,0x14,0x12,0x11},
  /* l */ {0x0C,0x04,0x04,0x04,0x04,0x04,0x0E},
  /* m */ {0x00,0x00,0x1A,0x15,0x15,0x15,0x15},
  /* n */ {0x00,0x00,0x1E,0x11,0x11,0x11,0x11},
  /* o */ {0x00,0x00,0x0E,0x11,0x11,0x11,0x0E},
  /* p */ {0x00,0x00,0x1E,0x11,0x1E,0x10,0x10},
  /* q */ {0x00,0x00,0x0F,0x11,0x0F,0x01,0x01},
  /* r */ {0x00,0x00,0x16,0x19,0x10,0x10,0x10},
  /* s */ {0x00,0x00,0x0F,0x10,0x0E,0x01,0x1E},
  /* t */ {0x08,0x08,0x1E,0x08,0x08,0x09,0x06},
  /* u */ {0x00,0x00,0x11,0x11,0x11,0x13,0x0D},
  /* v */ {0x00,0x00,0x11,0x11,0x11,0x0A,0x04},
  /* w */ {0x00,0x00,0x11,0x11,0x15,0x15,0x0A},
  /* x */ {0x00,0x00,0x11,0x0A,0x04,0x0A,0x11},
  /* y */ {0x00,0x00,0x11,0x11,0x0F,0x01,0x0E},
  /* z */ {0x00,0x00,0x1F,0x02,0x04,0x08,0x1F},
  /* { */ {0x02,0x04,0x04,0x08,0x04,0x04,0x02},
  /* | */ {0x04,0x04,0x04,0x04,0x04,0x04,0x04},
  /* } */ {0x08,0x04,0x04,0x02,0x04,0x04,0x08},
  /* ~ */ {0x00,0x00,0x08,0x15,0x02,0x00,0x00},
};

static IUB GlyphIndex(CHA c) {
  IUA b = (IUA)c;
  if (b >= 0x20 && b <= 0x7E) return b - 0x20;
  return 0;  // space for unknown
}

ISN DrawText(TFigure& fig, TAxis& ax, const CHA* text, ISN px, ISN py,
             TColor color) {
  (void)ax;
  if (text == nullptr) return 0;
  ISN written = 0;
  CImage& cv = fig.Canvas();
  for (const CHA* p = text; *p != 0; ++p) {
    IUB g = GlyphIndex(*p);
    for (ISN row = 0; row < 7; ++row) {
      IUB bits = kGlyphs[g][row];
      for (ISN col = 0; col < 5; ++col) {
        if (bits & (IUB)(1 << col)) {
          ImageDraw::PutPixel(cv, px + col, py + row, color);
          ++written;
        }
      }
    }
    px += 6;  // 5 glyph + 1 spacing
  }
  return written;
}

/* ============================ RenderFrame ============================ */
static void DrawTicks(TFigure& fig, TAxis& ax) {
  CImage& cv = fig.Canvas();
  TColor frame_c = ax.FrameColor();
  for (ISN i = 0; i <= ax.XTicks(); ++i) {
    FPD fx = ax.XMin() + (ax.XMax() - ax.XMin()) * (FPD)i / (FPD)ax.XTicks();
    ISN px = ax.DataToPixX(fx);
    ImageDraw::Line(cv, px, ax.Y1(), px, ax.Y1() + 4, frame_c, 1);
  }
  for (ISN i = 0; i <= ax.YTicks(); ++i) {
    FPD fy = ax.YMin() + (ax.YMax() - ax.YMin()) * (FPD)i / (FPD)ax.YTicks();
    ISN py = ax.DataToPixY(fy);
    ImageDraw::Line(cv, ax.X0() - 4, py, ax.X0(), py, frame_c, 1);
  }
}

void RenderFrame(TFigure& fig) {
  CImage& cv = fig.Canvas();
  for (ISN a = 0; a < fig.AxisCount(); ++a) {
    TAxis& ax = fig.Axis(a);
    ISN x0 = ax.X0(), y0 = ax.Y0(), x1 = ax.X1(), y1 = ax.Y1();
    TColor frame_c = ax.FrameColor();
    TColor grid_c = ax.GridColor();
    TColor bg_c = ax.BgColor();
    ISN xt = ax.XTicks();
    ISN yt = ax.YTicks();
    // Background
    ImageDraw::FillRect(cv, x0, y0, x1, y1, bg_c);
    if (ax.Grid()) {
      for (ISN i = 1; i < xt; ++i) {
        FPD fx = ax.XMin() + (ax.XMax() - ax.XMin()) * (FPD)i / (FPD)xt;
        ISN px = ax.DataToPixX(fx);
        ImageDraw::Line(cv, px, y0, px, y1, grid_c, 1);
      }
      for (ISN i = 1; i < yt; ++i) {
        FPD fy = ax.YMin() + (ax.YMax() - ax.YMin()) * (FPD)i / (FPD)yt;
        ISN py = ax.DataToPixY(fy);
        ImageDraw::Line(cv, x0, py, x1, py, grid_c, 1);
      }
    }
    if (ax.Frame()) {
      ImageDraw::Line(cv, x0, y0, x1, y0, frame_c, 1);
      ImageDraw::Line(cv, x0, y1, x1, y1, frame_c, 1);
      ImageDraw::Line(cv, x0, y0, x0, y1, frame_c, 1);
      ImageDraw::Line(cv, x1, y0, x1, y1, frame_c, 1);
    }
    DrawTicks(fig, ax);
    // Title centered on top.
    if (ax.Title() != nullptr) {
      ISN len = 0;
      while (ax.Title()[len] != 0) ++len;
      ISN tx = x0 + (x1 - x0) / 2 - (len * 6) / 2;
      if (tx < x0) tx = x0;
      DrawText(fig, ax, ax.Title(), tx, y0 - 9, frame_c);
    }
    // X label centered below.
    if (ax.XLabel() != nullptr) {
      ISN len = 0;
      while (ax.XLabel()[len] != 0) ++len;
      ISN tx = x0 + (x1 - x0) / 2 - (len * 6) / 2;
      if (tx < x0) tx = x0;
      DrawText(fig, ax, ax.XLabel(), tx, y1 + 6, frame_c);
    }
  }
}

/* ============================ 2D charts ============================ */
static const TSeries2D* CheckSeries(const TSeries2D& s) {
  if (s.x == nullptr || s.y == nullptr || s.count <= 0) return nullptr;
  return &s;
}

PlotStatus PlotLine(TFigure& fig, TAxis& ax, const TSeries2D& s, ISN thickness) {
  if (!CheckSeries(s)) return PlotStatus::Empty;
  CImage& cv = fig.Canvas();
  for (ISN i = 1; i < s.count; ++i) {
    ISN x0 = ax.DataToPixX(s.x[i - 1]);
    ISN y0 = ax.DataToPixY(s.y[i - 1]);
    ISN x1 = ax.DataToPixX(s.x[i]);
    ISN y1 = ax.DataToPixY(s.y[i]);
    ImageDraw::Line(cv, x0, y0, x1, y1, s.color, thickness);
  }
  return PlotStatus::OK;
}

PlotStatus PlotScatter(TFigure& fig, TAxis& ax, const TSeries2D& s,
                       ISN marker_radius) {
  if (!CheckSeries(s)) return PlotStatus::Empty;
  CImage& cv = fig.Canvas();
  for (ISN i = 0; i < s.count; ++i) {
    ISN px = ax.DataToPixX(s.x[i]);
    ISN py = ax.DataToPixY(s.y[i]);
    ImageDraw::FillCircle(cv, px, py, marker_radius, s.color);
  }
  return PlotStatus::OK;
}

PlotStatus PlotArea(TFigure& fig, TAxis& ax, const TSeries2D& s) {
  if (!CheckSeries(s)) return PlotStatus::Empty;
  CImage& cv = fig.Canvas();
  ISN base = ax.DataToPixY(0);
  for (ISN i = 1; i < s.count; ++i) {
    ISN x0 = ax.DataToPixX(s.x[i - 1]);
    ISN y0 = ax.DataToPixY(s.y[i - 1]);
    ISN x1 = ax.DataToPixX(s.x[i]);
    ISN y1 = ax.DataToPixY(s.y[i]);
    // Fill the vertical band between the two columns down to the baseline.
    ISN xa = x0 < x1 ? x0 : x1;
    ISN xb = x0 < x1 ? x1 : x0;
    ISN ya = y0 < y1 ? y0 : y1;
    ISN yb = (base < y0 ? base : y0) < (base < y1 ? base : y1)
                 ? (base < y0 ? base : y0)
                 : (base < y1 ? base : y1);
    for (ISN xx = xa; xx <= xb; ++xx)
      ImageDraw::Line(cv, xx, ya, xx, yb, s.color, 1);
  }
  return PlotStatus::OK;
}

PlotStatus PlotStairs(TFigure& fig, TAxis& ax, const TSeries2D& s,
                      ISN thickness) {
  if (!CheckSeries(s)) return PlotStatus::Empty;
  CImage& cv = fig.Canvas();
  for (ISN i = 1; i < s.count; ++i) {
    ISN x0 = ax.DataToPixX(s.x[i - 1]);
    ISN y0 = ax.DataToPixY(s.y[i - 1]);
    ISN x1 = ax.DataToPixX(s.x[i]);
    ISN y1 = ax.DataToPixY(s.y[i]);
    // Horizontal then vertical (post).
    ImageDraw::Line(cv, x0, y0, x1, y0, s.color, thickness);
    ImageDraw::Line(cv, x1, y0, x1, y1, s.color, thickness);
  }
  return PlotStatus::OK;
}

PlotStatus PlotStem(TFigure& fig, TAxis& ax, const TSeries2D& s) {
  if (!CheckSeries(s)) return PlotStatus::Empty;
  CImage& cv = fig.Canvas();
  ISN base = ax.DataToPixY(0);
  for (ISN i = 0; i < s.count; ++i) {
    ISN px = ax.DataToPixX(s.x[i]);
    ISN py = ax.DataToPixY(s.y[i]);
    ImageDraw::Line(cv, px, base, px, py, s.color, 1);
    ImageDraw::FillCircle(cv, px, py, 2, s.color);
  }
  return PlotStatus::OK;
}

PlotStatus PlotErrorbar(TFigure& fig, TAxis& ax, const TSeries2D& s,
                        const FPD* yerr, ISN thickness) {
  if (!CheckSeries(s) || yerr == nullptr) return PlotStatus::Empty;
  CImage& cv = fig.Canvas();
  for (ISN i = 0; i < s.count; ++i) {
    ISN px = ax.DataToPixX(s.x[i]);
    ISN py = ax.DataToPixY(s.y[i]);
    ISN pyh = ax.DataToPixY(s.y[i] + yerr[i]);
    ISN pyb = ax.DataToPixY(s.y[i] - yerr[i]);
    ImageDraw::Line(cv, px, pyh, px, pyb, s.color, 1);
    ImageDraw::Line(cv, px - 3, pyh, px + 3, pyh, s.color, 1);
    ImageDraw::Line(cv, px - 3, pyb, px + 3, pyb, s.color, 1);
  }
  PlotLine(fig, ax, s, thickness);
  return PlotStatus::OK;
}

PlotStatus PlotBar(TFigure& fig, TAxis& ax, const TSeries2D& s, FPD bar_width) {
  if (!CheckSeries(s)) return PlotStatus::Empty;
  CImage& cv = fig.Canvas();
  ISN base = ax.DataToPixY(0);
  for (ISN i = 0; i < s.count; ++i) {
    ISN pc = ax.DataToPixX(s.x[i]);
    ISN py = ax.DataToPixY(s.y[i]);
    ISN half = (ISN)((bar_width / (ax.XMax() - ax.XMin())) * (ax.X1() - ax.X0()) / 2);
    if (half < 1) half = 1;
    ImageDraw::FillRect(cv, pc - half, py < base ? py : base,
                        pc + half, py < base ? base : py, s.color);
  }
  return PlotStatus::OK;
}

PlotStatus PlotHist(TFigure& fig, TAxis& ax, const FPD* values, ISN count,
                    ISN bins, TColor color) {
  if (values == nullptr || count <= 0 || bins <= 0) return PlotStatus::Empty;
  FPD lo = values[0], hi = values[0];
  for (ISN i = 1; i < count; ++i) {
    if (values[i] < lo) lo = values[i];
    if (values[i] > hi) hi = values[i];
  }
  if (hi == lo) hi = lo + 1;
  FPD w = (hi - lo) / (FPD)bins;
  ISN hist[128];
  for (ISN b = 0; b < bins && b < 128; ++b) hist[b] = 0;
  for (ISN i = 0; i < count; ++i) {
    ISN b = (ISN)((values[i] - lo) / w);
    if (b < 0) b = 0;
    if (b >= bins) b = bins - 1;
    if (b < 128) ++hist[b];
  }
  // Build a bar series in bin-center coordinates.
  FPD xs[128], ys[128];
  for (ISN b = 0; b < bins && b < 128; ++b) {
    xs[b] = lo + (b + 0.5f) * w;
    ys[b] = (FPD)hist[b];
  }
  ISN nb = bins < 128 ? bins : 128;
  FPD bw = w * 0.9f;
  TSeries2D s{xs, ys, nb, color};
  return PlotBar(fig, ax, s, bw);
}

PlotStatus PlotBoxplot(TFigure& fig, TAxis& ax, const TSeries2D& groups) {
  if (!CheckSeries(groups)) return PlotStatus::Empty;
  // groups.x = group index (0..n-1), groups.y = the single summary value is
  // not enough for a real boxplot; this renders a simplified whisker/box per
  // group using the provided y as the median and a fixed 10% spread.
  CImage& cv = fig.Canvas();
  for (ISN i = 0; i < groups.count; ++i) {
    ISN px = ax.DataToPixX(groups.x[i]);
    ISN py = ax.DataToPixY(groups.y[i]);
    FPD spread = (ax.YMax() - ax.YMin()) * 0.1f;
    ISN pyhi = ax.DataToPixY(groups.y[i] + spread);
    ISN pylo = ax.DataToPixY(groups.y[i] - spread);
    ISN hw = 6;
    ImageDraw::Line(cv, px, pyhi, px, py, groups.color, 1);
    ImageDraw::Line(cv, px, py, px, pylo, groups.color, 1);
    ImageDraw::Line(cv, px - hw, pyhi, px + hw, pyhi, groups.color, 1);
    ImageDraw::Line(cv, px - hw, pylo, px + hw, pylo, groups.color, 1);
    ImageDraw::FillRect(cv, px - hw, py - 4, px + hw, py + 4, groups.color);
  }
  return PlotStatus::OK;
}

/* ============================ 3D charts ============================ */
/* A fixed isometric-ish projection: rotate around vertical axis by 45deg,
   then tilt. Pure math, no allocation. */
static void Project3(TAxis& ax, FPD x, FPD y, FPD z,
                     FPD* px, FPD* py, FPD* depth) {
  // Normalize x,y into 0..1 over the axis range, z into 0..1 over its range.
  FPD xn = (x - ax.XMin()) / (ax.XMax() == ax.XMin() ? 1 : (ax.XMax() - ax.XMin()));
  FPD yn = (y - ax.YMin()) / (ax.YMax() == ax.YMin() ? 1 : (ax.YMax() - ax.YMin()));
  FPD zn = z;
  // Isometric: screen_x = (xn - yn) * cos30, screen_y = (xn + yn) * sin30 - zn
  const FPD kcx = 0.8660254f;  // cos(30)
  const FPD ksy = 0.5f;        // sin(30)
  *px = (xn - yn) * kcx;
  *py = (xn + yn) * ksy - zn;
  *depth = (xn + yn) * 0.5f;  // for painter's order (approx)
}

static void Plot3Pt(TFigure& fig, TAxis& ax, FPD x, FPD y, FPD z, TColor color) {
  FPD px, py, depth;
  (void)depth;
  Project3(ax, x, y, z, &px, &py, &depth);
  // Map normalized (-1..1) screen coords into the axis pixel rect.
  ISN sx = ax.X0() + (ISN)((px * 0.5f + 0.5f) * (ax.X1() - ax.X0()));
  ISN sy = ax.Y0() + (ISN)((py * 0.5f + 0.5f) * (ax.Y1() - ax.Y0()));
  ImageDraw::PutPixel(fig.Canvas(), sx, sy, color);
}

PlotStatus Plot3(TFigure& fig, TAxis& ax, const TPoint3* pts, ISN count,
                 TColor color, ISN thickness) {
  if (pts == nullptr || count <= 0) return PlotStatus::Empty;
  (void)thickness;
  for (ISN i = 0; i < count; ++i) {
    FPD px0, py0, d0, px1, py1, d1;
    (void)d0;
    (void)d1;
    Project3(ax, pts[i].x, pts[i].y, pts[i].z, &px0, &py0, &d0);
    ISN sx0 = ax.X0() + (ISN)((px0 * 0.5f + 0.5f) * (ax.X1() - ax.X0()));
    ISN sy0 = ax.Y0() + (ISN)((py0 * 0.5f + 0.5f) * (ax.Y1() - ax.Y0()));
    if (i + 1 < count) {
      Project3(ax, pts[i + 1].x, pts[i + 1].y, pts[i + 1].z, &px1, &py1, &d1);
      ISN sx1 = ax.X0() + (ISN)((px1 * 0.5f + 0.5f) * (ax.X1() - ax.X0()));
      ISN sy1 = ax.Y0() + (ISN)((py1 * 0.5f + 0.5f) * (ax.Y1() - ax.Y0()));
      ImageDraw::Line(fig.Canvas(), sx0, sy0, sx1, sy1, color, thickness);
    } else {
      ImageDraw::PutPixel(fig.Canvas(), sx0, sy0, color);
    }
  }
  return PlotStatus::OK;
}

PlotStatus PlotMesh(TFigure& fig, TAxis& ax, const FPD* grid, ISN nx, ISN ny,
                    TColor color) {
  // grid is nx*ny of z-values, row-major (y then x).
  if (grid == nullptr || nx <= 0 || ny <= 0) return PlotStatus::Empty;
  CImage& cv = fig.Canvas();
  for (ISN iy = 0; iy < ny; ++iy) {
    for (ISN ix = 0; ix < nx; ++ix) {
      FPD xn = (FPD)ix / (FPD)(nx - 1);
      FPD yn = (FPD)iy / (FPD)(ny - 1);
      FPD z0 = grid[iy * nx + ix];
      if (ix + 1 < nx) {
        FPD xn2 = (FPD)(ix + 1) / (FPD)(nx - 1);
        FPD z1 = grid[iy * nx + ix + 1];
        FPD px0, py0, d0, px1, py1, d1;
        (void)d0; (void)d1;
        Project3(ax, xn, yn, z0, &px0, &py0, &d0);
        Project3(ax, xn2, yn, z1, &px1, &py1, &d1);
        ISN sx0 = ax.X0() + (ISN)((px0 * 0.5f + 0.5f) * (ax.X1() - ax.X0()));
        ISN sy0 = ax.Y0() + (ISN)((py0 * 0.5f + 0.5f) * (ax.Y1() - ax.Y0()));
        ISN sx1 = ax.X0() + (ISN)((px1 * 0.5f + 0.5f) * (ax.X1() - ax.X0()));
        ISN sy1 = ax.Y0() + (ISN)((py1 * 0.5f + 0.5f) * (ax.Y1() - ax.Y0()));
        ImageDraw::Line(cv, sx0, sy0, sx1, sy1, color, 1);
      }
      if (iy + 1 < ny) {
        FPD yn2 = (FPD)(iy + 1) / (FPD)(ny - 1);
        FPD z1 = grid[(iy + 1) * nx + ix];
        FPD px0, py0, d0, px1, py1, d1;
        (void)d0; (void)d1;
        Project3(ax, xn, yn, z0, &px0, &py0, &d0);
        Project3(ax, xn, yn2, z1, &px1, &py1, &d1);
        ISN sx0 = ax.X0() + (ISN)((px0 * 0.5f + 0.5f) * (ax.X1() - ax.X0()));
        ISN sy0 = ax.Y0() + (ISN)((py0 * 0.5f + 0.5f) * (ax.Y1() - ax.Y0()));
        ISN sx1 = ax.X0() + (ISN)((px1 * 0.5f + 0.5f) * (ax.X1() - ax.X0()));
        ISN sy1 = ax.Y0() + (ISN)((py1 * 0.5f + 0.5f) * (ax.Y1() - ax.Y0()));
        ImageDraw::Line(cv, sx0, sy0, sx1, sy1, color, 1);
      }
    }
  }
  return PlotStatus::OK;
}

PlotStatus PlotSurf(TFigure& fig, TAxis& ax, const FPD* grid, ISN nx, ISN ny,
                    TColor color) {
  // Surface = mesh plus a flat fill per quad (approximated by filling a
  // small rect at each projected cell center, shaded by z).
  if (grid == nullptr || nx <= 0 || ny <= 0) return PlotStatus::Empty;
  CImage& cv = fig.Canvas();
  FPD zmin = grid[0], zmax = grid[0];
  for (ISN i = 1; i < nx * ny; ++i) {
    if (grid[i] < zmin) zmin = grid[i];
    if (grid[i] > zmax) zmax = grid[i];
  }
  for (ISN iy = 0; iy < ny; ++iy) {
    for (ISN ix = 0; ix < nx; ++ix) {
      FPD xn = (FPD)ix / (FPD)(nx - 1);
      FPD yn = (FPD)iy / (FPD)(ny - 1);
      FPD z = grid[iy * nx + ix];
      FPD px, py, d;
      (void)d;
      Project3(ax, xn, yn, z, &px, &py, &d);
      ISN sx = ax.X0() + (ISN)((px * 0.5f + 0.5f) * (ax.X1() - ax.X0()));
      ISN sy = ax.Y0() + (ISN)((py * 0.5f + 0.5f) * (ax.Y1() - ax.Y0()));
      IUB shade = (IUB)(64 + (IUN)((z - zmin) / (zmax - zmin) * 128.0f));
      IUB cr = (IUB)((IUN)color.r * (IUN)shade / 255);
      IUB cg = (IUB)((IUN)color.g * (IUN)shade / 255);
      IUB cb = (IUB)((IUN)color.b * (IUN)shade / 255);
      TColor c{cr, cg, cb, 255};
      ImageDraw::FillRect(cv, sx - 1, sy - 1, sx + 1, sy + 1, c);
    }
  }
  return PlotStatus::OK;
}

PlotStatus PlotContour(TFigure& fig, TAxis& ax, const FPD* grid, ISN nx,
                       ISN ny, ISN levels, TColor color) {
  if (grid == nullptr || nx <= 0 || ny <= 0 || levels <= 0)
    return PlotStatus::Empty;
  CImage& cv = fig.Canvas();
  FPD zmin = grid[0], zmax = grid[0];
  for (ISN i = 1; i < nx * ny; ++i) {
    if (grid[i] < zmin) zmin = grid[i];
    if (grid[i] > zmax) zmax = grid[i];
  }
  // For each cell, if the z range of the cell crosses a level, mark the cell
  // boundary (a cheap marching-squares stand-in for the seam test).
  for (ISN lv = 0; lv < levels; ++lv) {
    FPD t = (FPD)lv / (FPD)(levels - 1);
    FPD lvl = zmin + t * (zmax - zmin);
    for (ISN iy = 0; iy < ny; ++iy) {
      for (ISN ix = 0; ix < nx; ++ix) {
        FPD z = grid[iy * nx + ix];
        if ((z - lvl) * (grid[iy * nx + ix] - lvl) > 0) continue;
        FPD xn = (FPD)ix / (FPD)(nx - 1);
        FPD yn = (FPD)iy / (FPD)(ny - 1);
        FPD px, py, d;
        (void)d;
        Project3(ax, xn, yn, z, &px, &py, &d);
        ISN sx = ax.X0() + (ISN)((px * 0.5f + 0.5f) * (ax.X1() - ax.X0()));
        ISN sy = ax.Y0() + (ISN)((py * 0.5f + 0.5f) * (ax.Y1() - ax.Y0()));
        ImageDraw::PutPixel(cv, sx, sy, color);
      }
    }
  }
  return PlotStatus::OK;
}

/* ============================ Quiver ============================ */
PlotStatus PlotQuiver(TFigure& fig, TAxis& ax, const FPD* x, const FPD* y,
                      const FPD* u, const FPD* v, ISN count, FPD scale,
                      TColor color) {
  if (x == nullptr || y == nullptr || u == nullptr || v == nullptr || count <= 0)
    return PlotStatus::Empty;
  CImage& cv = fig.Canvas();
  for (ISN i = 0; i < count; ++i) {
    ISN x0 = ax.DataToPixX(x[i]);
    ISN y0 = ax.DataToPixY(y[i]);
    ISN x1 = ax.DataToPixX(x[i] + u[i] * scale);
    ISN y1 = ax.DataToPixY(y[i] + v[i] * scale);
    ImageDraw::Line(cv, x0, y0, x1, y1, color, 1);
    // Arrow head: two short lines from the tip.
    FPD dx = (FPD)(x1 - x0), dy = (FPD)(y1 - y0);
    FPD len = (FPD)sqrt(dx * dx + dy * dy);
    if (len < 1.0f) len = 1.0f;
    FPD hx = dx / len * 4.0f, hy = dy / len * 4.0f;
    ImageDraw::Line(cv, x1, y1, (ISN)(x1 - hx - hy), (ISN)(y1 - hy + hx),
                    color, 1);
    ImageDraw::Line(cv, x1, y1, (ISN)(x1 - hx + hy), (ISN)(y1 - hy - hx),
                    color, 1);
  }
  return PlotStatus::OK;
}

}  // namespace Plot
}  // namespace _
