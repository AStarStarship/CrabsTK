// Copyright AStarship <https://astarship.net>.
//
// TrueType.hxx — TrueType font decoder. The table layout is Apple's and
// Microsoft's `glyf` flavour; the parser reads big-endian fields straight out
// of the caller's file bytes, so nothing is copied and nothing is cached
// beyond the table offsets.
#pragma once
#ifndef _CrabsTKExoTrueTypeHXX
#define _CrabsTKExoTrueTypeHXX
#include "TrueType.h"
#include <ASCIICrabs/Math.hxx>
namespace CTK {
namespace Exo {

/* The List parameter set a Wall is built on: ISS, ISR, DTB. */
typedef _::TList<ISS, ISR, DTB> FontList;

/* How much glyph a single outline may use. A Latin, Greek, Cyrillic or CJK
glyph stays far under this; a pathological or corrupt font is refused
instead of overrunning the scratch. */
enum : ISC {
  MaxGlyphPoints = 1024,
  MaxShapeVertices = 1024,
  MaxRasterEdges = 1024,
  MaxCrossings = 1024,
  MaxCompositeDepth = 8,
};

/* Type code for the scratch blocks: an 8 byte POD aligns the FPC in Vertex
and the ISC in the crossing list alike. */
enum : DTB { FontScratchType = _::_IUD };

/* sfnt table tags, big-endian, as they sit in the table directory. */
enum : ISC {
  FontTagHead = 0x68656164,
  FontTagHhea = 0x68686561,
  FontTagHmtx = 0x686D7478,
  FontTagMaxp = 0x6D617870,
  FontTagCmap = 0x636D6170,
  FontTagLoca = 0x6C6F6361,
  FontTagGlyf = 0x676C7966,
  FontTagKern = 0x6B65726E,
  FontVersionOne = 0x00010000,
  FontVersionTrue = 0x74727565,
  FontTagTtcf = 0x74746366,
};

/* ── Big-endian field readers ────────────────────────────────────────
The font file is big-endian; the host is not. These read straight from the
file bytes without alignment assumptions. */
static inline ISC U8At(const IUA* p) { return ISC(p[0]); }

static inline ISC U16At(const IUA* p) {
  return (ISC(p[0]) << 8) | ISC(p[1]);
}

static inline ISC S16At(const IUA* p) { return ISC(ISB(U16At(p))); }

static inline IUC U32At(const IUA* p) {
  return (IUC(U16At(p)) << 16) | IUC(U16At(p + 2));
}

/* A signed 16.16 fixed point value, used by the kern table. */
static inline ISC S32At(const IUA* p) {
  return ISC(ISB(U16At(p)) * 65536 + ISB(U16At(p + 2)));
}

/* A signed 2.14 fixed point value, used by composite transforms. */
static inline FPC F2Dot14At(const IUA* p) {
  return FPC(S16At(p)) / FPC(16384);
}

TrueTypeFont::TrueTypeFont()
  : loca_(-1),
    head_(-1),
    glyf_(-1),
    hhea_(-1),
    hmtx_(-1),
    kern_(-1),
    cmap_(-1),
    glyph_count_(0),
    units_per_em_(0),
    loca_format_(0),
    hmetric_count_(0),
    file_(_NilP),
    wall_(_NilP),
    vertices_(_NilP),
    vertices_max_(0),
    point_x_(_NilP),
    point_y_(_NilP),
    point_flags_(_NilP),
    flat_x_(_NilP),
    flat_y_(_NilP),
    flat_max_(0),
    edges_(_NilP),
    edges_max_(0),
    edges_used_(0),
    crosses_(_NilP),
    cross_max_(0),
    start_(0) {}

ISC TrueTypeFont::FontOffset(const IUA* file, ISC index) {
  if (file == _NilP || index < 0) return -1;
  if (ISC(U32At(file)) != FontTagTtcf) return index == 0 ? 0 : -1;
  ISC count = ISC(U32At(file + 8));
  if (index >= count) return -1;
  return ISC(U32At(file + 12 + index * 4));
}

ISC TrueTypeFont::Tag_(ISC tag) const {
  ISC count = U16At(file_ + start_ + 4);
  for (ISC i = 0; i < count; ++i) {
    const IUA* record = file_ + start_ + 12 + i * 16;
    if (ISC(U32At(record)) == tag) return ISC(U32At(record + 8));
  }
  return -1;
}

BOL TrueTypeFont::Start(_::TWall<>* wall, const IUA* file, ISC offset) {
  if (wall == _NilP || file == _NilP || offset < 0) return false;
  ISC version = ISC(U32At(file + offset));
  if (version != FontVersionOne && version != FontVersionTrue) return false;

  file_ = file;
  wall_ = wall;
  start_ = offset;

  ISC maxp = Tag_(FontTagMaxp);
  head_ = Tag_(FontTagHead);
  hhea_ = Tag_(FontTagHhea);
  hmtx_ = Tag_(FontTagHmtx);
  cmap_ = Tag_(FontTagCmap);
  loca_ = Tag_(FontTagLoca);
  glyf_ = Tag_(FontTagGlyf);
  kern_ = Tag_(FontTagKern);

  /* A CFF flavoured OpenType file has no glyf and no loca: refuse it rather
  than answer with an empty font. */
  if (maxp < 0 || head_ < 0 || hhea_ < 0 || hmtx_ < 0 || loca_ < 0 ||
      glyf_ < 0)
    return false;

  units_per_em_ = U16At(file_ + head_ + 18);
  if (units_per_em_ <= 0) return false;
  loca_format_ = S16At(file_ + head_ + 50);
  glyph_count_ = U16At(file_ + maxp + 4);
  hmetric_count_ = U16At(file_ + hhea_ + 34);
  if (glyph_count_ <= 0 || hmetric_count_ <= 0) return false;

  /* One scratch block per kind of data: the outline, the decoded points, and
  the scanline crossings. They live until the font does. */
  FontList* root = _::TWallRoot<>(wall);
  if (root == _NilP) return false;
  ISR index = _::TListAlloc<ISS, ISR, DTB>(
    root, DTB(FontScratchType), ISS(MaxShapeVertices * ISC(sizeof(Vertex))));
  if (index < 0) return false;
  vertices_ = _::TListValue<ISS, ISR, DTB, Vertex>(root, index);
  vertices_max_ = MaxShapeVertices;

  index = _::TListAlloc<ISS, ISR, DTB>(
    root, DTB(FontScratchType), ISS(MaxGlyphPoints * ISC(sizeof(ISC))));
  if (index < 0) return false;
  point_x_ = _::TListValue<ISS, ISR, DTB, ISC>(root, index);

  index = _::TListAlloc<ISS, ISR, DTB>(
    root, DTB(FontScratchType), ISS(MaxGlyphPoints * ISC(sizeof(ISC))));
  if (index < 0) return false;
  point_y_ = _::TListValue<ISS, ISR, DTB, ISC>(root, index);

  index = _::TListAlloc<ISS, ISR, DTB>(
    root, DTB(FontScratchType), ISS(MaxGlyphPoints * ISC(sizeof(IUA))));
  if (index < 0) return false;
  point_flags_ = _::TListValue<ISS, ISR, DTB, IUA>(root, index);

  index = _::TListAlloc<ISS, ISR, DTB>(
    root, DTB(FontScratchType), ISS(MaxRasterEdges * ISC(sizeof(FPC))));
  if (index < 0) return false;
  flat_x_ = _::TListValue<ISS, ISR, DTB, FPC>(root, index);

  index = _::TListAlloc<ISS, ISR, DTB>(
    root, DTB(FontScratchType), ISS(MaxRasterEdges * ISC(sizeof(FPC))));
  if (index < 0) return false;
  flat_y_ = _::TListValue<ISS, ISR, DTB, FPC>(root, index);
  flat_max_ = MaxRasterEdges;

  index = _::TListAlloc<ISS, ISR, DTB>(
    root, DTB(FontScratchType), ISS(MaxRasterEdges * ISC(sizeof(RasterEdge))));
  if (index < 0) return false;
  edges_ = _::TListValue<ISS, ISR, DTB, RasterEdge>(root, index);
  edges_max_ = MaxRasterEdges;
  edges_used_ = 0;

  index = _::TListAlloc<ISS, ISR, DTB>(
    root, DTB(FontScratchType), ISS(MaxCrossings * ISC(sizeof(RasterCross))));
  if (index < 0) return false;
  crosses_ = _::TListValue<ISS, ISR, DTB, RasterCross>(root, index);
  cross_max_ = MaxCrossings;

  return vertices_ != _NilP && point_x_ != _NilP && point_y_ != _NilP &&
         point_flags_ != _NilP && flat_x_ != _NilP && flat_y_ != _NilP &&
         edges_ != _NilP && crosses_ != _NilP;
}

/* ── cmap: code point to glyph ───────────────────────────────────────
Four subtable formats cover every font in the wild: 0 and 6 for 8 bit
legacy fonts, 4 for the BMP, 12 for the full repertoire. */
BOL TrueTypeFont::ReadCmapFormat0_(ISC at, ISC code_point, ISC* glyph) const {
  if (code_point < 0 || code_point > 255) return false;
  *glyph = U8At(file_ + at + 6 + code_point);
  return *glyph != 0;
}

BOL TrueTypeFont::ReadCmapFormat6_(ISC at, ISC code_point, ISC* glyph) const {
  const IUA* p = file_ + at;
  ISC first = U16At(p + 6);
  ISC count = U16At(p + 8);
  if (code_point < first || code_point >= first + count) return false;
  *glyph = U16At(p + 10 + (code_point - first) * 2);
  return *glyph != 0;
}

BOL TrueTypeFont::ReadCmapFormat4_(ISC at, ISC code_point, ISC* glyph) const {
  const IUA* p = file_ + at;
  ISC segments = U16At(p + 6) / 2;
  const IUA* ends = p + 14;
  const IUA* starts = ends + segments * 2 + 2;
  const IUA* deltas = starts + segments * 2;
  const IUA* offsets = deltas + segments * 2;

  for (ISC i = 0; i < segments; ++i) {
    ISC last = U16At(ends + i * 2);
    if (code_point > last) continue;
    ISC first = U16At(starts + i * 2);
    if (code_point < first) return false;
    ISC delta = S16At(deltas + i * 2);
    ISC range = U16At(offsets + i * 2);
    if (range == 0) {
      *glyph = (code_point + delta) & 0xFFFF;
      return *glyph != 0;
    }
    /* The offset is measured from its own slot, in bytes. */
    const IUA* slot = offsets + i * 2 + range + (code_point - first) * 2;
    ISC found = U16At(slot);
    if (found == 0) return false;
    *glyph = (found + delta) & 0xFFFF;
    return *glyph != 0;
  }
  return false;
}

BOL TrueTypeFont::ReadCmapFormat12_(ISC at, ISC code_point, ISC* glyph) const {
  const IUA* p = file_ + at;
  ISC groups = ISC(U32At(p + 12));
  for (ISC i = 0; i < groups; ++i) {
    const IUA* group = p + 16 + i * 12;
    IUC first = U32At(group);
    IUC last = U32At(group + 4);
    if (IUC(code_point) < first) return false;
    if (IUC(code_point) > last) continue;
    *glyph = ISC(U32At(group + 8) + IUC(code_point) - first);
    return *glyph != 0;
  }
  return false;
}

BOL TrueTypeFont::ReadCmap_(ISC code_point, ISC* glyph) const {
  *glyph = 0;
  if (cmap_ < 0 || code_point < 0) return false;
  const IUA* c = file_ + cmap_;
  ISC count = U16At(c + 2);
  ISC chosen = -1;
  ISC chosen_format = 0;

  for (ISC i = 0; i < count; ++i) {
    const IUA* record = c + 4 + i * 8;
    ISC platform = U16At(record);
    ISC encoding = U16At(record + 2);
    BOL unicode = platform == 0 || (platform == 3 && (encoding == 1 ||
                                                      encoding == 10));
    if (!unicode) continue;
    ISC sub = cmap_ + ISC(U32At(record + 4));
    ISC format = U16At(file_ + sub);
    if (format != 0 && format != 4 && format != 6 && format != 12) continue;
    /* The full repertoire beats the BMP table, and either beats legacy. */
    if (format == 12) {
      chosen = sub;
      chosen_format = 12;
      break;
    }
    if (chosen < 0 || (chosen_format != 4 && format == 4)) {
      chosen = sub;
      chosen_format = format;
    }
  }
  if (chosen < 0) return false;

  switch (chosen_format) {
    case 0: return ReadCmapFormat0_(chosen, code_point, glyph);
    case 4: return ReadCmapFormat4_(chosen, code_point, glyph);
    case 6: return ReadCmapFormat6_(chosen, code_point, glyph);
    case 12: return ReadCmapFormat12_(chosen, code_point, glyph);
  }
  return false;
}

ISC TrueTypeFont::FindGlyph(ISC code_point) const {
  ISC glyph = 0;
  ReadCmap_(code_point, &glyph);
  return glyph;
}

/* ── Metrics ─────────────────────────────────────────────────────── */
void TrueTypeFont::GetVerticalMetrics(VMetrics& metrics) const {
  metrics.ascent = hhea_ < 0 ? 0 : S16At(file_ + hhea_ + 4);
  metrics.descent = hhea_ < 0 ? 0 : S16At(file_ + hhea_ + 6);
  metrics.line_gap = hhea_ < 0 ? 0 : S16At(file_ + hhea_ + 8);
}

void TrueTypeFont::GetHorizontalMetrics(ISC glyph, HMetrics& metrics) const {
  metrics.advance_width = 0;
  metrics.left_side_bearing = 0;
  if (hmtx_ < 0 || glyph < 0) return;
  if (glyph < hmetric_count_) {
    metrics.advance_width = U16At(file_ + hmtx_ + glyph * 4);
    metrics.left_side_bearing = S16At(file_ + hmtx_ + glyph * 4 + 2);
    return;
  }
  /* Glyphs past the last full metric repeat its advance and carry only a
  bearing of their own. */
  metrics.advance_width = U16At(file_ + hmtx_ + (hmetric_count_ - 1) * 4);
  metrics.left_side_bearing =
    S16At(file_ + hmtx_ + hmetric_count_ * 4 + (glyph - hmetric_count_) * 2);
}

ISC TrueTypeFont::GetKernAdvance(ISC first, ISC second) const {
  ISC advance = 0;
  ReadKern_(first, second, &advance);
  return advance;
}

/* The kern table: a version 0 header, then subtables. Only the horizontal
format 0 subtable holds glyph pairs; anything else is skipped by its length
field. */
BOL TrueTypeFont::ReadKern_(ISC first, ISC second, ISC* advance) const {
  *advance = 0;
  if (kern_ < 0) return false;
  ISC tables = U16At(file_ + kern_ + 2);
  ISC at = kern_ + 4;
  for (ISC t = 0; t < tables; ++t) {
    ISC length = U16At(file_ + at + 2);
    ISC coverage = U16At(file_ + at + 4);
    if ((coverage >> 8) == 0) {
      ISC pairs = U16At(file_ + at + 6);
      const IUA* base = file_ + at + 14;
      IUD key = (IUD(IUC(first)) << 16) | IUC(second);
      ISC low = 0;
      ISC high = pairs - 1;
      while (low <= high) {
        ISC mid = (low + high) / 2;
        IUD probe = IUD(U32At(base + mid * 6));
        if (probe < key) {
          low = mid + 1;
        } else if (probe > key) {
          high = mid - 1;
        } else {
          *advance = S16At(base + mid * 6 + 4);
          return true;
        }
      }
    }
    if (length <= 0) break;
    at += length;
  }
  return false;
}

FPC TrueTypeFont::ScaleForPixelHeight(FPC pixels) const {
  VMetrics metrics;
  GetVerticalMetrics(metrics);
  ISC height = metrics.ascent - metrics.descent;
  return height > 0 ? pixels / FPC(height) : FPC(0);
}

FPC TrueTypeFont::ScaleForMappingEmToPixels(FPC pixels) const {
  return units_per_em_ > 0 ? pixels / FPC(units_per_em_) : FPC(0);
}

/* ── Glyph geometry ──────────────────────────────────────────────── */
ISC TrueTypeFont::GlyphOffset_(ISC glyph) const {
  if (loca_ < 0 || glyph < 0 || glyph >= glyph_count_) return -1;
  const IUA* p = file_ + loca_;
  if (loca_format_ == 0) return ISC(U16At(p + glyph * 2)) * 2;
  return ISC(U32At(p + glyph * 4));
}

ISC TrueTypeFont::GlyphSize_(ISC glyph) const {
  ISC start = GlyphOffset_(glyph);
  ISC next = GlyphOffset_(glyph + 1);
  if (start < 0 || next < 0 || next < start) return 0;
  return next - start;
}

void TrueTypeFont::GetBitmapBox(ISC glyph, FPC scale_x, FPC scale_y,
                                BitmapBox& box) const {
  box.x0 = box.y0 = box.x1 = box.y1 = 0;
  ISC off = GlyphOffset_(glyph);
  if (off < 0 || GlyphSize_(glyph) <= 0) return;
  const IUA* g = file_ + glyf_ + off;
  ISC x_min = S16At(g + 2);
  ISC y_min = S16At(g + 4);
  ISC x_max = S16At(g + 6);
  ISC y_max = S16At(g + 8);
  /* The box is in whole pixels and covers the outline, so the left and top
  edges round down and the right and bottom edges round up. y flips: the
  bitmap grows downward from the top of the glyph. */
  box.x0 = ISC(_::Floor(FPC(x_min) * scale_x));
  box.y0 = ISC(_::Floor(FPC(-y_max) * scale_y));
  box.x1 = ISC(_::Ceiling(FPC(x_max) * scale_x));
  box.y1 = ISC(_::Ceiling(FPC(-y_min) * scale_y));
}

/* ── Outlines ────────────────────────────────────────────────────────
A simple glyph is a run of contours of on and off curve points, stored as
delta encoded coordinates behind a flags array that repeats itself with a
run length byte. The decoder replays the deltas, then turns each contour into
corner and quadratic vertices, inventing the implied on-curve midpoint
between two off-curve points. */
ISC TrueTypeFont::BuildSimple_(ISC glyph, const FontXform& xf, Vertex* out,
                               ISC out_max, ISC count) const {
  ISC off = GlyphOffset_(glyph);
  if (off < 0) return -1;
  const IUA* g = file_ + glyf_ + off;
  ISC contours = S16At(g);
  if (contours <= 0) return count;

  const IUA* p = g + 10;
  ISC points = S16At(p + (contours - 1) * 2) + 1;
  if (points <= 0 || points > MaxGlyphPoints) return -1;

  const IUA* instructions = p + contours * 2;
  p = instructions + 2 + U16At(instructions);

  /* Flags, with the repeat byte that keeps a straight run short. */
  for (ISC i = 0; i < points;) {
    ISC flag = U8At(p++);
    point_flags_[i++] = IUA(flag);
    if (flag & 8) {
      ISC repeat = U8At(p++);
      while (repeat-- > 0 && i < points) point_flags_[i++] = IUA(flag);
    }
  }

  /* x, then y: a short delta is one byte with the sign in the flag, a long
  one is two bytes, and an unset bit means the coordinate repeats. */
  ISC run = 0;
  for (ISC i = 0; i < points; ++i) {
    ISC flag = ISC(point_flags_[i]);
    if (flag & 2) {
      ISC delta = U8At(p++);
      run += (flag & 16) ? delta : -delta;
    } else if (!(flag & 16)) {
      run += S16At(p);
      p += 2;
    }
    point_x_[i] = run;
  }
  run = 0;
  for (ISC i = 0; i < points; ++i) {
    ISC flag = ISC(point_flags_[i]);
    if (flag & 4) {
      ISC delta = U8At(p++);
      run += (flag & 32) ? delta : -delta;
    } else if (!(flag & 32)) {
      run += S16At(p);
      p += 2;
    }
    point_y_[i] = run;
  }

  ISC first_point = 0;
  for (ISC c = 0; c < contours; ++c) {
    ISC last_point = S16At(g + 10 + c * 2);
    ISC n = last_point - first_point + 1;
    if (n <= 0) {
      first_point = last_point + 1;
      continue;
    }

    /* Start on an on-curve point when the contour has one. A contour of
    nothing but off-curve points starts at the midpoint of its ends. */
    ISC first_on = -1;
    for (ISC i = 0; i < n; ++i) {
      if (ISC(point_flags_[first_point + i]) & 1) {
        first_on = i;
        break;
      }
    }
    BOL all_off = first_on < 0;
    FPC start_x, start_y;
    if (all_off) {
      ISC a = first_point;
      ISC b = first_point + n - 1;
      start_x = FPC(point_x_[a] + point_x_[b]) / 2;
      start_y = FPC(point_y_[a] + point_y_[b]) / 2;
      first_on = 0;
    } else {
      start_x = FPC(point_x_[first_point + first_on]);
      start_y = FPC(point_y_[first_point + first_on]);
    }

    if (count >= out_max) return -1;
    out[count].x = xf.a * start_x + xf.b * start_y + xf.dx;
    out[count].y = xf.c * start_x + xf.d * start_y + xf.dy;
    out[count].cx = out[count].x;
    out[count].cy = out[count].y;
    out[count].type = VertexMove;
    ++count;

    FPC control_x = 0;
    FPC control_y = 0;
    BOL have_control = false;
    for (ISC k = 1; k <= n; ++k) {
      ISC at = first_point + ((first_on + k) % n);
      FPC x = FPC(point_x_[at]);
      FPC y = FPC(point_y_[at]);
      BOL on_curve = !all_off && (ISC(point_flags_[at]) & 1) != 0;
      if (on_curve) {
        if (count >= out_max) return -1;
        if (have_control) {
          out[count].x = control_x;
          out[count].y = control_y;
          out[count].type = VertexQuad;
          have_control = false;
        } else {
          out[count].x = xf.a * x + xf.b * y + xf.dx;
          out[count].y = xf.c * x + xf.d * y + xf.dy;
          out[count].type = VertexCorner;
        }
        out[count].cx = xf.a * x + xf.b * y + xf.dx;
        out[count].cy = xf.c * x + xf.d * y + xf.dy;
        ++count;
      } else {
        if (have_control) {
          /* Two off-curve points in a row imply the midpoint between them. */
          FPC mid_x = (control_x + x) / 2;
          FPC mid_y = (control_y + y) / 2;
          if (count >= out_max) return -1;
          out[count].x = control_x;
          out[count].y = control_y;
          out[count].cx = xf.a * mid_x + xf.b * mid_y + xf.dx;
          out[count].cy = xf.c * mid_x + xf.d * mid_y + xf.dy;
          out[count].type = VertexQuad;
          ++count;
        }
        control_x = xf.a * x + xf.b * y + xf.dx;
        control_y = xf.c * x + xf.d * y + xf.dy;
        have_control = true;
      }
    }
    if (have_control) {
      /* The contour closes back on the point it started at. */
      if (count >= out_max) return -1;
      out[count].x = control_x;
      out[count].y = control_y;
      out[count].cx = xf.a * start_x + xf.b * start_y + xf.dx;
      out[count].cy = xf.c * start_x + xf.d * start_y + xf.dy;
      out[count].type = VertexQuad;
      ++count;
    }
    first_point = last_point + 1;
  }
  return count;
}

/* A composite glyph is a list of components: each names another glyph plus an
offset, a scale, or a 2 by 2 matrix. The transforms compose, so a component
of a component lands where the outer one puts it. */
ISC TrueTypeFont::BuildComposite_(ISC glyph, const FontXform& xf, Vertex* out,
                                  ISC out_max, ISC count, ISC depth) const {
  if (depth > MaxCompositeDepth) return -1;
  ISC off = GlyphOffset_(glyph);
  if (off < 0) return -1;
  const IUA* g = file_ + glyf_ + off;
  ISC at = 10;
  BOL more = true;

  while (more) {
    ISC flags = U16At(g + at);
    ISC component = U16At(g + at + 2);
    at += 4;

    FPC dx = 0;
    FPC dy = 0;
    if (flags & 1) {
      dx = FPC(S16At(g + at));
      dy = FPC(S16At(g + at + 2));
      at += 4;
    } else {
      dx = FPC(ISB(IUA(g[at])));
      dy = FPC(ISB(IUA(g[at + 1])));
      at += 2;
    }
    /* Component arguments that are point numbers, not offsets, are rare; the
    component still lands, just without the point match. */

    FPC a = 1;
    FPC b = 0;
    FPC c = 0;
    FPC d = 1;
    if (flags & 8) {
      a = d = F2Dot14At(g + at);
      at += 2;
    }
    if (flags & 64) {
      a = F2Dot14At(g + at);
      d = F2Dot14At(g + at + 2);
      at += 4;
    }
    if (flags & 128) {
      a = F2Dot14At(g + at);
      b = F2Dot14At(g + at + 2);
      c = F2Dot14At(g + at + 4);
      d = F2Dot14At(g + at + 6);
      at += 8;
    }

    FontXform composed;
    composed.a = xf.a * a + xf.b * c;
    composed.b = xf.a * b + xf.b * d;
    composed.c = xf.c * a + xf.d * c;
    composed.d = xf.c * b + xf.d * d;
    composed.dx = xf.a * dx + xf.b * dy + xf.dx;
    composed.dy = xf.c * dx + xf.d * dy + xf.dy;

    ISC child = GlyphOffset_(component);
    if (child < 0) return -1;
    ISC contours = S16At(file_ + glyf_ + child);
    if (contours > 0)
      count = BuildSimple_(component, composed, out, out_max, count);
    else if (contours < 0)
      count = BuildComposite_(component, composed, out, out_max, count,
                              depth + 1);
    if (count < 0) return -1;

    more = (flags & 32) != 0;
  }
  return count;
}

BOL TrueTypeFont::BuildShape_(ISC glyph, GlyphShape& shape) {
  shape.vertices = _NilP;
  shape.count = 0;
  if (file_ == _NilP || vertices_ == _NilP) return false;
  ISC off = GlyphOffset_(glyph);
  if (off < 0 || GlyphSize_(glyph) <= 0) return false;

  ISC contours = S16At(file_ + glyf_ + off);
  if (contours == 0) return false;  //< a blank glyph, such as the space.

  FontXform identity;
  identity.a = 1;
  identity.b = 0;
  identity.c = 0;
  identity.d = 1;
  identity.dx = 0;
  identity.dy = 0;

  ISC count = contours > 0
    ? BuildSimple_(glyph, identity, vertices_, vertices_max_, 0)
    : BuildComposite_(glyph, identity, vertices_, vertices_max_, 0, 0);
  if (count <= 0) return false;
  shape.vertices = vertices_;
  shape.count = count;
  return true;
}

BOL TrueTypeFont::GetGlyphShape(ISC glyph, GlyphShape& shape) {
  return BuildShape_(glyph, shape);
}

/* ── Rasterizer ──────────────────────────────────────────────────────
Scanline coverage with the non-zero winding rule. The outline is flattened
into segments in pixel space, every row is sampled SubSamples times, and each
sample line sorts the segments it crosses and walks them with a winding
counter: the spans where the winding is not zero are the inside of the glyph
and get their clipped width added to the pixels they cover. That antialiases
in both directions without keeping an edge table sorted between rows.

Contours may overlap and may run either way: the direction of each segment
decides its contribution to the winding, so a counter and an inner cut-out
both come out right. */
enum : ISC { SubSamples = 4 };

/* Adds one point to the flattened run, refusing to overrun the scratch. */
static ISC AddFlatPoint(FPC* xs, FPC* ys, ISC at, ISC max, FPC x, FPC y) {
  if (at >= max) return at;
  xs[at] = x;
  ys[at] = y;
  return at + 1;
}

/* Flattens a quadratic curve in pixel space, subdividing until the halves
are flat to within a sixteenth of a pixel. (x2, y2) is the end point. */
static ISC FlattenQuad(FPC* xs, FPC* ys, ISC at, ISC max, FPC x0, FPC y0,
                       FPC x1, FPC y1, FPC x2, FPC y2, ISC depth) {
  FPC mid_x = (x0 + 2 * x1 + x2) / 4;
  FPC mid_y = (y0 + 2 * y1 + y2) / 4;
  FPC flat_x = (x0 + x2) / 2 - mid_x;
  FPC flat_y = (y0 + y2) / 2 - mid_y;
  if (depth < 10 && flat_x * flat_x + flat_y * flat_y > FPC(0.00390625)) {
    at = FlattenQuad(xs, ys, at, max, x0, y0, (x0 + x1) / 2, (y0 + y1) / 2,
                     mid_x, mid_y, depth + 1);
    return FlattenQuad(xs, ys, at, max, mid_x, mid_y, (x1 + x2) / 2,
                       (y1 + y2) / 2, x2, y2, depth + 1);
  }
  return AddFlatPoint(xs, ys, at, max, x2, y2);
}

/* Flattens a cubic the same way. */
static ISC FlattenCubic(FPC* xs, FPC* ys, ISC at, ISC max, FPC x0, FPC y0,
                        FPC x1, FPC y1, FPC x2, FPC y2, FPC x3, FPC y3,
                        ISC depth) {
  FPC mid_x = (x0 + 3 * x1 + 3 * x2 + x3) / 8;
  FPC mid_y = (y0 + 3 * y1 + 3 * y2 + y3) / 8;
  FPC flat_x = (x0 + x3) / 2 - mid_x;
  FPC flat_y = (y0 + y3) / 2 - mid_y;
  if (depth < 10 && flat_x * flat_x + flat_y * flat_y > FPC(0.00390625)) {
    FPC x01 = (x0 + x1) / 2, y01 = (y0 + y1) / 2;
    FPC x12 = (x1 + x2) / 2, y12 = (y1 + y2) / 2;
    FPC x23 = (x2 + x3) / 2, y23 = (y2 + y3) / 2;
    FPC xa = (x01 + x12) / 2, ya = (y01 + y12) / 2;
    FPC xb = (x12 + x23) / 2, yb = (y12 + y23) / 2;
    at = FlattenCubic(xs, ys, at, max, x0, y0, x01, y01, xa, ya, mid_x, mid_y,
                      depth + 1);
    return FlattenCubic(xs, ys, at, max, mid_x, mid_y, xb, yb, x23, y23, x3,
                        y3, depth + 1);
  }
  return AddFlatPoint(xs, ys, at, max, x3, y3);
}

/* Turns one finished run of the point array into edges and closes it back
onto its first point. One run is one contour: joining two runs would add a
segment the glyph does not have, which is how a counter would get filled in. */
void TrueTypeFont::EmitContour_(ISC begin, ISC end) {
  if (end - begin < 2) return;
  for (ISC i = begin + 1; i < end; ++i)
    AddEdge_(flat_x_[i - 1], flat_y_[i - 1], flat_x_[i], flat_y_[i]);
  AddEdge_(flat_x_[end - 1], flat_y_[end - 1], flat_x_[begin], flat_y_[begin]);
}

ISC TrueTypeFont::Flatten_(const GlyphShape& shape, FPC scale_x, FPC scale_y,
                           ISC off_x, ISC off_y) {
  ISC points = 0;
  ISC contour_begin = -1;
  FPC last_x = 0;
  FPC last_y = 0;

  for (ISC i = 0; i < shape.count; ++i) {
    const Vertex& v = shape.vertices[i];
    if (v.type == VertexMove) {
      if (contour_begin >= 0) EmitContour_(contour_begin, points);
      contour_begin = points;
      last_x = v.x * scale_x - FPC(off_x);
      last_y = -v.y * scale_y - FPC(off_y);
      points = AddFlatPoint(flat_x_, flat_y_, points, flat_max_, last_x,
                            last_y);
    } else if (v.type == VertexCorner) {
      last_x = v.x * scale_x - FPC(off_x);
      last_y = -v.y * scale_y - FPC(off_y);
      points = AddFlatPoint(flat_x_, flat_y_, points, flat_max_, last_x,
                            last_y);
    } else if (v.type == VertexQuad) {
      FPC end_x = v.cx * scale_x - FPC(off_x);
      FPC end_y = -v.cy * scale_y - FPC(off_y);
      points = FlattenQuad(flat_x_, flat_y_, points, flat_max_, last_x,
                           last_y, v.x * scale_x - FPC(off_x),
                           -v.y * scale_y - FPC(off_y), end_x, end_y, 0);
      last_x = end_x;
      last_y = end_y;
    } else {
      /* A cubic's end point is the next vertex's (x, y), and that vertex is
      consumed here rather than starting a contour of its own. */
      FPC end_x = last_x;
      FPC end_y = last_y;
      if (i + 1 < shape.count) {
        end_x = shape.vertices[i + 1].x * scale_x - FPC(off_x);
        end_y = -shape.vertices[i + 1].y * scale_y - FPC(off_y);
        ++i;
      }
      points = FlattenCubic(flat_x_, flat_y_, points, flat_max_, last_x,
                            last_y, v.x * scale_x - FPC(off_x),
                            -v.y * scale_y - FPC(off_y),
                            v.cx * scale_x - FPC(off_x),
                            -v.cy * scale_y - FPC(off_y), end_x, end_y, 0);
      last_x = end_x;
      last_y = end_y;
    }
  }
  if (contour_begin >= 0) EmitContour_(contour_begin, points);
  return points;
}

void TrueTypeFont::AddEdge_(FPC x0, FPC y0, FPC x1, FPC y1) {
  if (edges_max_ <= 0 || edges_used_ >= edges_max_) return;
  if (y0 == y1) return;
  RasterEdge& edge = edges_[edges_used_++];
  if (y0 < y1) {
    edge.x0 = x0;
    edge.y0 = y0;
    edge.x1 = x1;
    edge.y1 = y1;
    edge.dir = 1;
  } else {
    edge.x0 = x1;
    edge.y0 = y1;
    edge.x1 = x0;
    edge.y1 = y0;
    edge.dir = -1;
  }
}

/* Adds the clipped width of one span to each pixel it covers. The caller
adds SubSamples spans per pixel row, so the coverage divides by that. */
void TrueTypeFont::FillSpan_(IUA* row, ISC width, FPC x0, FPC x1) {
  if (x1 <= x0) return;
  ISC first = ISC(_::Floor(x0));
  ISC last = ISC(_::Ceiling(x1)) - 1;
  if (first < 0) first = 0;
  if (last >= width) last = width - 1;
  for (ISC px = first; px <= last; ++px) {
    FPC left = x0 > FPC(px) ? x0 : FPC(px);
    FPC right = x1 < FPC(px + 1) ? x1 : FPC(px + 1);
    FPC cover = right - left;
    if (cover <= 0) continue;
    FPC add = cover * FPC(255) / FPC(SubSamples);
    ISC sum = ISC(row[px]) + ISC(add + FPC(0.5));
    row[px] = IUA(sum > 255 ? 255 : sum);
  }
}

void TrueTypeFont::Rasterize_(IUA* output, ISC out_w, ISC out_h, ISC stride) {
  for (ISC row = 0; row < out_h; ++row) {
    IUA* pixels = output + row * stride;
    FPC top = FPC(row);
    for (ISC sub = 0; sub < SubSamples; ++sub) {
      FPC y = top + (FPC(sub) + FPC(0.5)) / FPC(SubSamples);
      ISC crossings = 0;

      for (ISC e = 0; e < edges_used_ && crossings < cross_max_; ++e) {
        const RasterEdge& edge = edges_[e];
        if (y < edge.y0 || y >= edge.y1) continue;
        FPC t = (y - edge.y0) / (edge.y1 - edge.y0);
        crosses_[crossings].x = edge.x0 + t * (edge.x1 - edge.x0);
        crosses_[crossings].dir = edge.dir;
        ++crossings;
      }

      /* A sample line meets a handful of edges, so insertion sort is the
      cheapest thing that keeps the walk allocation free. */
      for (ISC i = 1; i < crossings; ++i) {
        RasterCross key = crosses_[i];
        ISC j = i - 1;
        while (j >= 0 && crosses_[j].x > key.x) {
          crosses_[j + 1] = crosses_[j];
          --j;
        }
        crosses_[j + 1] = key;
      }

      ISC winding = 0;
      FPC span_start = 0;
      for (ISC i = 0; i < crossings; ++i) {
        if (winding == 0) span_start = crosses_[i].x;
        winding += crosses_[i].dir;
        if (winding == 0) FillSpan_(pixels, out_w, span_start, crosses_[i].x);
      }
    }
  }
}

BOL TrueTypeFont::MakeGlyphBitmap(ISC glyph, FPC scale_x, FPC scale_y,
                                  IUA* output, ISC out_w, ISC out_h,
                                  ISC stride) {
  if (output == _NilP || out_w <= 0 || out_h <= 0) return false;
  if (flat_x_ == _NilP || edges_ == _NilP || crosses_ == _NilP) return false;

  GlyphShape shape;
  if (!BuildShape_(glyph, shape)) return false;

  BitmapBox box;
  GetBitmapBox(glyph, scale_x, scale_y, box);
  edges_used_ = 0;
  Flatten_(shape, scale_x, scale_y, box.x0, box.y0);

  for (ISC row = 0; row < out_h; ++row)
    _::ArrayFill(output + row * stride, out_w);
  Rasterize_(output, out_w, out_h, stride);
  return true;
}

}  //< namespace Exo
}  //< namespace CTK
#endif  //< _CrabsTKExoTrueTypeHXX
