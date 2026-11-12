// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKExoTrueTypeH
#define _CrabsTKExoTrueTypeH
#include <_Config.h>
#include <ASCIICrabs/_Config.h>
#include <ASCIICrabs/Room.hpp>
namespace CTK {
namespace Exo {

/* Vertical metrics of a font, in font units. */
struct VMetrics {
  ISC ascent, descent, line_gap;
};

/* Horizontal metrics of one glyph, in font units. */
struct HMetrics {
  ISC advance_width;
  ISC left_side_bearing;
};

/* Integer pixel bounds of a rasterized glyph: [x0, x1) wide, [y0, y1) tall,
with y0 above the baseline negative and growing downward. */
struct BitmapBox {
  ISC x0, y0, x1, y1;
};

/* One step of an outline, in font units, y growing up.
type 3 (Move):   the first point of a contour, on-curve at (x, y).
type 0 (Corner): an on-curve point at (x, y), continuing the contour.
type 1 (Quad):   (x, y) is the quadratic control point, (cx, cy) the on-curve
                 end point.
type 2 (Cubic):  (x, y) and (cx, cy) are the two control points, and the next
                 vertex's (x, y) is the end point. */
enum : ISC {
  VertexCorner = 0,
  VertexQuad = 1,
  VertexCubic = 2,
  VertexMove = 3,
};

struct Vertex {
  FPC x, y;
  FPC cx, cy;
  ISC type;
};

/* An affine transform of an outline: x' = a*x + b*y + dx, y' = c*x + d*y + dy.
Simple glyphs use the identity; composite glyphs compose one per component. */
struct FontXform {
  FPC a, b, c, d, dx, dy;
};

/* One flattened outline segment in pixels, y0 < y1, with the direction of
the original edge (+1 downward in the bitmap, -1 upward). The direction is
what makes the winding rule work where two contours overlap. */
struct RasterEdge {
  FPC x0, y0, x1, y1;
  ISC dir;
};

/* Where one sample line meets one edge. */
struct RasterCross {
  FPC x;
  ISC dir;
};

/* An outline: vertices borrowed from the font's Wall scratch, valid until
the next call that draws on the same font. */
struct GlyphShape {
  Vertex* vertices;
  ISC count;
};

/* A TrueType font decoder: it reads a `glyf` flavoured .ttf that the caller
already has in memory and answers the questions a text renderer asks, then
rasterizes a glyph into a caller-supplied 8 bit coverage bitmap.

The file bytes stay the caller's. Every scratch block the decoder needs (the
outline vertices and the rasterizer edges) comes from a Wall in the Room, so
the decoder never allocates, and each call hands its scratch back before it
returns. `Start` reads the table directory once; nothing else re-parses it.

Fonts without a `glyf` table (CFF flavoured OpenType) are refused rather than
half decoded.

@code
  _::TWall<> wall;
  _::TWallInit<>(&wall, 512 * 1024);
  TrueTypeFont font;
  if (!font.Start(&wall, file_bytes, 0)) return;
  ISC advance, bearing;
  font.GetHorizontalMetrics(font.FindGlyph('A'), advance, bearing);
@endcode
*/
class TrueTypeFont {
 public:
  TrueTypeFont();

  /* Reads the table directory at `offset` inside `file`. Returns false when
  the file is not a TrueType font, when glyf/loca/head/maxp/hhea/hmtx are
  missing, or when the Wall cannot hold the scratch blocks. */
  BOL Start(_::TWall<>* wall, const IUA* file, ISC offset = 0);

  /* The glyph a Unicode code point maps to, or 0 when the font has none. */
  ISC FindGlyph(ISC code_point) const;

  void GetVerticalMetrics(VMetrics& metrics) const;

  /* Advance width and left side bearing of a glyph, in font units. */
  void GetHorizontalMetrics(ISC glyph, HMetrics& metrics) const;

  /* Kerning between two glyphs, in font units; 0 when the font has none. */
  ISC GetKernAdvance(ISC first, ISC second) const;

  /* Font units to pixels: what the glyph outline is scaled by to land on a
  font size picked in pixels measured on the em square. */
  FPC ScaleForMappingEmToPixels(FPC pixels) const;

  /* Font units to pixels: the scale that makes a capital H `pixels` tall,
  which is what a line height in pixels usually means. */
  FPC ScaleForPixelHeight(FPC pixels) const;

  /* Bounds the glyph would occupy when rasterized at this scale. */
  void GetBitmapBox(ISC glyph, FPC scale_x, FPC scale_y,
                    BitmapBox& box) const;

  /* Rasterizes the glyph into `output` as 8 bit coverage: 0 is empty, 255
  is fully covered. `stride` is the row pitch of `output`; rows past the
  bottom of the box are left alone. Returns false when the glyph has no
  outline. */
  BOL MakeGlyphBitmap(ISC glyph, FPC scale_x, FPC scale_y, IUA* output,
                      ISC out_w, ISC out_h, ISC stride);

  /* The glyph outline in font units. Vertices are scratch: they stay valid
  until the next GetGlyphShape or MakeGlyphBitmap call on this font. */
  BOL GetGlyphShape(ISC glyph, GlyphShape& shape);

  ISC GetGlyphCount() const { return glyph_count_; }
  ISC GetUnitsPerEm() const { return units_per_em_; }

  /* The offset of font `index` inside a font file, or -1 when there is no
  such font. Font collections hold several. */
  static ISC FontOffset(const IUA* file, ISC index);

 private:
  /* Table directory entry offsets, resolved once by Start. */
  ISC loca_, head_, glyf_, hhea_, hmtx_, kern_, cmap_;
  ISC glyph_count_;
  ISC units_per_em_;
  ISC loca_format_;     //< 0: short offsets, 1: long offsets.
  ISC hmetric_count_;   //< entries in hmtx; the rest repeat the last one.
  const IUA* file_;
  _::TWall<>* wall_;
  Vertex* vertices_;      //< glyph outline scratch.
  ISC vertices_max_;
  ISC* point_x_;          //< decoded point coordinates, font units.
  ISC* point_y_;
  IUA* point_flags_;      //< one raw TrueType flag byte per point.
  FPC* flat_x_;           //< finished outline, in pixels.
  FPC* flat_y_;
  ISC flat_max_;
  RasterEdge* edges_;     //< flattened segments, one per outline step.
  ISC edges_max_;
  ISC edges_used_;
  RasterCross* crosses_;  //< one line's worth of crossings.
  ISC cross_max_;

  ISC start_;
  ISC Tag_(ISC tag) const;
  ISC GlyphOffset_(ISC glyph) const;
  ISC GlyphSize_(ISC glyph) const;

  BOL ReadCmapFormat0_(ISC at, ISC code_point, ISC* glyph) const;
  BOL ReadCmapFormat4_(ISC at, ISC code_point, ISC* glyph) const;
  BOL ReadCmapFormat6_(ISC at, ISC code_point, ISC* glyph) const;
  BOL ReadCmapFormat12_(ISC at, ISC code_point, ISC* glyph) const;
  BOL ReadCmap_(ISC code_point, ISC* glyph) const;
  BOL ReadKern_(ISC first, ISC second, ISC* advance) const;

  BOL BuildShape_(ISC glyph, GlyphShape& shape);
  ISC BuildSimple_(ISC glyph, const FontXform& xf, Vertex* out, ISC out_max,
                   ISC count) const;
  ISC BuildComposite_(ISC glyph, const FontXform& xf, Vertex* out,
                      ISC out_max, ISC count, ISC depth) const;
  ISC Flatten_(const GlyphShape& shape, FPC scale_x, FPC scale_y, ISC off_x,
               ISC off_y);
  void EmitContour_(ISC begin, ISC end);
  void AddEdge_(FPC x0, FPC y0, FPC x1, FPC y1);
  void FillSpan_(IUA* row, ISC width, FPC x0, FPC x1);
  void Rasterize_(IUA* output, ISC out_w, ISC out_h, ISC stride);
};

}  //< namespace Exo
}  //< namespace CTK
#endif  //< _CrabsTKExoTrueTypeH
