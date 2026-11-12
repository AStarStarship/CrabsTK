// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKExoFontStashH
#define _CrabsTKExoFontStashH
#include <_Config.h>
#include <ASCIICrabs/_Config.h>
#include <ASCIICrabs/Room.hpp>
#include "RectPack.h"
#include "TrueType.h"
namespace CTK {
namespace Exo {

/* Fixed capacities of a glyph cache. Nothing here grows at run time: the
caller sizes the atlas and the glyph table in Start, so a frame can never
run out of Room in the middle of a draw call. */
enum : ISC {
  StashFontsMax = 8,
  StashGlyphsMin = 256,
  StashStatesMax = 20,
  StashFallbacksMax = 4,
  StashLutSize = 256,
  StashNameLength = 64,
  StashSizeTenths = 10,
  StashBlurMax = 20,
  StashBorder = 2,
  StashScratchMax = 256 * 256,
  /* An atlas is a square of 8 bit coverage; 2048 squared is the largest a
  single channel texture stays comfortably inside on every target we ship. */
  StashAtlasBytesMax = 2048 * 2048,
  /* Blocks a Wall must be able to hold: the atlas, the font table, the shared
  glyph table, the packer nodes, the blur scratch, plus five per font for the
  decoder's own scratch. A Wall holds a fixed number of blocks, so this is
  what the caller passes to TWallInit. */
  StashWallBlocksFixed = 5,
  StashWallBlocksPerFont = 5,
};

/* Blocks a Wall needs to hold a cache of `fonts` fonts. */
constexpr ISC StashWallBlocks(ISC fonts) {
  return StashWallBlocksFixed + fonts * StashWallBlocksPerFont;
}

/* Where a string lands relative to the pen. One horizontal bit and one
vertical bit are meant to be combined, which is how callers already think
about text anchoring. The values match the Alignments enum in VG.h, so a
renderer state passes its alignment straight through. */
enum class TextAlign : ISC {
  Left = 1,
  Center = 2,
  Right = 4,
  Top = 8,
  Middle = 16,
  Bottom = 32,
  Baseline = 64,
};

constexpr TextAlign operator|(TextAlign a, TextAlign b) {
  return TextAlign(ISC(a) | ISC(b));
}

constexpr BOL HasAlign(TextAlign set, TextAlign flag) {
  return (ISC(set) & ISC(flag)) != 0;
}

/* Whether a glyph walk has to materialize pixels. Optional is the measuring
pass: it answers advances and offsets and leaves the atlas untouched, which
is what aligning a string needs before it can be drawn. */
enum class BitmapMode : ISC {
  Optional = 0,
  Required = 1,
};

/* One cached glyph: where it sits in the atlas and how it moves the pen.
Advance and offsets are kept in tenths of a pixel, because a size of 13.4 px
has to survive the cache intact and rounding the advance to whole pixels
drifts a line visibly by the end of a paragraph. */
struct StashGlyph {
  CHC code_point;
  ISC index;              //< glyph index in the font that supplied pixels.
  ISC next;               //< next slot in the hash chain, -1 at the end.
  ISC size;               //< tenths of a pixel the glyph was rasterized at.
  ISC blur;
  ISC x0, y0, x1, y1;     //< atlas rectangle, half open, border included.
  ISC w, h;               //< tile size including the border, 0 when none.
  ISC advance;            //< pen movement, tenths of a pixel.
  ISC x_offset;           //< where the bitmap starts from the pen, pixels.
  ISC y_offset;
};

/* One loaded font: the decoder, the glyph table, and the fallback chain.
The font bytes stay the caller's; the glyph table comes out of the same Room
Wall as the atlas. */
struct StashFont {
  CHR name[StashNameLength];
  const IUA* data;
  ISC data_size;
  FPC ascender;           //< normalized: ascent / (ascent - descent).
  FPC descender;          //< line height is (ascender - descender) * size.
  StashGlyph* glyphs;
  ISC glyph_count;
  ISC glyph_max;
  ISC lut[StashLutSize];
  ISC fallbacks[StashFallbacksMax];
  ISC fallback_count;
  TrueTypeFont font;
};

/* The part of the drawing state a Push/Pop pair has to restore. */
struct StashState {
  ISC font;
  FPC size;
  FPC blur;
  FPC spacing;
  IUC color;
  TextAlign align;
};

/* One glyph ready to draw: a screen rectangle and the atlas coordinates it
samples. The atlas side is already inset by a pixel so bilinear filtering
cannot pull a neighbour into the glyph. */
struct StashQuad {
  FPC x0, y0, s0, t0;
  FPC x1, y1, s1, t1;
};

/* Position in a string: where the pen stands, which codepoint comes next,
and which glyph came before it, for kerning. The caller advances it with
TextIterNext and draws the quad it hands back. */
struct TextIter {
  const CHR* cursor;      //< next unit to decode.
  const CHR* end;
  FPC x, y;               //< pen for the glyph just returned.
  FPC next_x, next_y;     //< pen after it.
  FPC scale;
  FPC spacing;
  ISC size;               //< tenths of a pixel.
  ISC blur;
  ISC prev_glyph;
  CHC code_point;
  StashFont* font;
  BitmapMode mode;
};

/* A glyph atlas and the text layout that fills it.
Takes UTF strings, walks them a codepoint at a time, caches each glyph as an
8 bit coverage tile inside one square texture, and hands the caller quads to
draw with whatever renderer it has. Decoding is the house `SScan`, rasterizing
is `TrueTypeFont`, and packing is `RectPack`, so the whole path is the
ASCIICrabs type system with no C heap, no C string and no stdlib geometry.

The atlas does not grow. The caller decides its size in Start and resets it
with a new size when it fills; a glyph that does not fit leaves its quad out
but still advances the pen, so a missing glyph never shifts a line.

@code
  _::TWall<> wall;
  _::TWallInit<>(&wall, 1 << 21);
  FontStash stash;
  if (!stash.Start(&wall, 1024, 1024, 512)) return;
  ISC index = stash.AddFont("body", font_bytes, font_bytes_size);
  stash.SetFont(index);
  stash.SetSize(24);
  TextIter iter;
  StashQuad quad;
  if (stash.TextIterInit(iter, x, y, text, _NilP, BitmapMode::Required))
    while (stash.TextIterNext(iter, quad)) DrawQuad(quad);
@endcode
*/
class FontStash {
 public:
  FontStash();
  ~FontStash();

  /* Binds the cache to a Wall, carves the atlas, the font table, the glyph
  table and the blur scratch out of it, and clears every state slot. Returns
  false when the Wall cannot hold them. The Wall must be initialized with at
  least StashWallBlocks(fonts) block slots. `top_left` selects the coordinate
  convention: true puts y down from the top left corner, false puts y up from
  the bottom left, and every quad flips accordingly. */
  BOL Start(_::TWall<>* wall, ISC width, ISC height,
            ISC glyphs = StashGlyphsMin, ISC fonts = StashFontsMax,
            BOL top_left = true);

  /* Registers a font the caller already has in memory and returns its index
  in the cache, or -1 when the data is not a font the decoder accepts or the
  font table is full. The bytes are borrowed, not copied: they have to
  outlive the cache. */
  ISC AddFont(const CHR* name, const IUA* data, ISC data_size);

  /* Index of the font with that name, or -1. */
  ISC FindFont(const CHR* name) const;

  /* How many fonts are registered. */
  ISC FontCount() const { return font_count_; }

  /* Makes `fallback` answer codepoints that `base` has no glyph for, which is
  how a Latin font covers CJK or an icon font covers a symbol. Returns the
  number of fallbacks now chained, or -1 when the list is full. */
  ISC AddFallback(ISC base, ISC fallback);

  /* Drops every fallback of `base`. */
  void ResetFallbacks(ISC base);

  void SetFont(ISC index);
  void SetSize(FPC size);
  void SetColor(IUC color);
  void SetSpacing(FPC spacing);
  void SetBlur(FPC blur);
  void SetAlign(TextAlign align);

  /* Saves the current state so a nested draw can change size and colour and
  give the old ones back. Push beyond StashStatesMax is ignored. */
  void PushState();
  void PopState();

  /* Puts the state back to the values Start left it in and empties the
  push stack. */
  void ClearState();

  /* Vertical extent of a line of text drawn with the current size whose
  baseline is at `y`, measured the same way the quads are placed. */
  void LineBounds(FPC y, FPC& min_y, FPC& max_y);

  /* Ascender, descender and line height of the current font at the current
  size, in pixels. Negated descender is the depth below the baseline. */
  void VerticalMetrics(FPC& ascender, FPC& descender, FPC& line_height);

  /* The atlas pixels, tightly packed at `width` by `height` bytes. What the
  caller uploads as a single channel texture. */
  const IUA* Texture(ISC& width, ISC& height) const;

  /* Hands back the part of the atlas that changed since the last call and
  clears it, as {x, y, width, height}. Returns false when nothing changed, in
  which case the rectangle is not written. */
  BOL ValidateTexture(ISC rect[4]);

  /* Throws the atlas and every cached glyph away and starts a fresh one at a
  new size, which is what a caller does when the cache fills up. Fails when
  the request is larger than the Wall had room for in Start. */
  BOL ResetAtlas(ISC width, ISC height);

  /* Starts a walk over `text` at pen (x, y). `end` may be nil for a nil
  terminated string. Alignment and vertical anchoring come from the current
  state, so a centred string needs no measuring pass of its own. */
  BOL TextIterInit(TextIter& iter, FPC x, FPC y, const CHR* text,
                   const CHR* end, BitmapMode mode);

  /* Steps to the next codepoint. Fills `quad` and returns true while there is
  one; a codepoint whose glyph does not fit in the atlas still advances the
  pen and returns true with the quad untouched. */
  BOL TextIterNext(TextIter& iter, StashQuad& quad);

  /* Glyphs the atlas could not place since the last call, cleared by it. A
  renderer that can grow its atlas checks this after a walk and retries the
  string at the larger size; one that cannot knows which glyphs it lost. */
  ISC TakeMissing() {
    ISC missing = missing_;
    missing_ = 0;
    return missing;
  }

  /* Pen advance of `text` and, when `bounds` is not nil, the box it covers as
  {min_x, min_y, max_x, max_y}. */
  FPC TextBounds(FPC x, FPC y, const CHR* text, const CHR* end,
                 FPC bounds[4]);

 private:
  /* Copy and assign are deleted: the cache borrows a Wall and hands out
  pointers into it, so a copy could not own what it points at. */
  FontStash(const FontStash&) = delete;
  FontStash& operator=(const FontStash&) = delete;

  StashState& Current_() { return states_[state_count_ - 1]; }
  const StashState& Current_() const { return states_[state_count_ - 1]; }

  static ISC Hash_(CHC code_point);
  static ISC TextLength_(const CHR* text);

  StashFont* FontAt_(ISC index);
  const StashFont* FontAt_(ISC index) const;

  /* Finds the cached glyph, or creates the entry and rasterizes it when
  `mode` demands pixels. Returns nil when the atlas cannot hold it. */
  StashGlyph* Glyph_(ISC font, CHC code_point, ISC size, ISC blur,
                     BitmapMode mode);

  /* Turns a cached glyph into the quad for the current pen and moves the
  pen past it, kerning against the previous glyph. */
  void Quad_(StashFont& font, ISC prev_glyph, StashGlyph& glyph, FPC spacing,
             FPC& x, FPC& y, StashQuad& quad);

  /* Distance from the pen down to the top of the text, for the vertical half
  of the alignment. */
  FPC VerticalAlign_(const StashFont& font, TextAlign align, ISC size) const;

  /* Box blur, in place, over an atlas rectangle. Two exponential passes,
  which is what makes a shadow look like a shadow rather than a smear. */
  void Blur_(IUA* dst, ISC width, ISC height, ISC stride, ISC blur);

  _::TWall<>* wall_;
  IUA* texture_;
  ISC width_;
  ISC height_;
  ISC max_width_;         //< block the texture came from: the growth ceiling.
  ISC max_height_;
  FPC inverse_width_;
  FPC inverse_height_;
  ISC dirty_[4];          //< left, top, right, bottom, half open.
  RectPack pack_;
  StashFont* fonts_;
  ISC font_count_;
  ISC font_max_;
  ISC glyph_max_;
  IUA* scratch_;
  ISC scratch_size_;
  ISC missing_;           //< glyphs the atlas could not place since the last call.
  StashState states_[StashStatesMax];
  ISC state_count_;
  BOL top_left_;
};

}       //< namespace Exo
}       //< namespace CTK
#endif  //< _CrabsTKExoFontStashH
