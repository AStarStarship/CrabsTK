// Copyright AStarship <https://astarship.net>.
//
// FontStash.hxx — glyph atlas and text layout. The cache is Barrett's
// fontstash, minus its allocator, its UTF-8 decoder, its font loader and its
// packer: those four are the Room Wall, `SScan`, `TrueTypeFont` and `RectPack`
// now, so what is left here is the glyph cache, the quad and the pen.
#pragma once
#ifndef _CrabsTKExoFontStashHXX
#define _CrabsTKExoFontStashHXX
#include "FontStash.h"
#include <ASCIICrabs/Array.h>
#include <ASCIICrabs/Math.hxx>
#include <ASCIICrabs/Stringf.hpp>
namespace CTK {
namespace Exo {

/* The List parameter set a Wall is built on: ISS, ISR, DTB. */
typedef _::TList<ISS, ISR, DTB> StashList;

/* Type code for every block the cache takes: an IUD aligned payload is
  aligned for the atlas bytes, the font table and the glyph table alike. */
enum : DTB { StashBlockType = _::_IUD };

/* Exponential blur precision: the accumulator keeps 16 fractional bits and
  the samples keep 7, which is the pair fontstash tuned for 8 bit coverage. */
enum : ISC {
  BlurPrecision = 16,
  BlurSampleBits = 7,
};

/* Reserves one block in the Room Wall and hands back its payload. */
template <typename T>
T* StashAllocate(_::TWall<>* wall, ISC bytes) {
  if (wall == _NilP || bytes <= 0) return _NilP;
  StashList* root = _::TWallRoot<>(wall);
  if (root == _NilP) return _NilP;
  ISR index = _::TListAlloc<ISS, ISR, DTB>(root, DTB(StashBlockType),
                                          ISS(bytes));
  if (index < 0) return _NilP;
  return _::TListValue<ISS, ISR, DTB, T>(root, index);
}

/* One forward and one backward exponential pass along a row. The border is
  forced to zero so a blurred glyph fades out instead of wrapping its own
  edge back in. */
static void StashBlurColumns(IUA* dst, ISC width, ISC height, ISC stride,
                             ISC alpha) {
  for (ISC y = 0; y < height; ++y) {
    IUA* row = dst + y * stride;
    ISC z = 0;
    for (ISC x = 1; x < width; ++x) {
      z += (alpha * ((ISC(row[x]) << BlurSampleBits) - z)) >> BlurPrecision;
      row[x] = IUA(z >> BlurSampleBits);
    }
    row[width - 1] = 0;
    z = 0;
    for (ISC x = width - 2; x >= 0; --x) {
      z += (alpha * ((ISC(row[x]) << BlurSampleBits) - z)) >> BlurPrecision;
      row[x] = IUA(z >> BlurSampleBits);
    }
    row[0] = 0;
  }
}

/* The same pass down a column. */
static void StashBlurRows(IUA* dst, ISC width, ISC height, ISC stride,
                          ISC alpha) {
  for (ISC x = 0; x < width; ++x) {
    ISC z = 0;
    for (ISC y = 1; y < height; ++y) {
      IUA* here = dst + y * stride + x;
      z += (alpha * ((ISC(*here) << BlurSampleBits) - z)) >> BlurPrecision;
      *here = IUA(z >> BlurSampleBits);
    }
    dst[(height - 1) * stride + x] = 0;
    z = 0;
    for (ISC y = height - 2; y >= 0; --y) {
      IUA* here = dst + y * stride + x;
      z += (alpha * ((ISC(*here) << BlurSampleBits) - z)) >> BlurPrecision;
      *here = IUA(z >> BlurSampleBits);
    }
    dst[x] = 0;
  }
}

FontStash::FontStash()
  : wall_(_NilP),
    texture_(_NilP),
    width_(0),
    height_(0),
    max_width_(0),
    max_height_(0),
    inverse_width_(1),
    inverse_height_(1),
    fonts_(_NilP),
    font_count_(0),
    font_max_(0),
    glyph_max_(0),
    scratch_(_NilP),
    scratch_size_(0),
    missing_(0),
    state_count_(1),
    top_left_(true) {
  dirty_[0] = dirty_[1] = 0;
  dirty_[2] = dirty_[3] = 0;
  _::ArrayFill(&states_[0], sizeof(StashState));
  states_[0].font = -1;
  states_[0].size = 16;
  states_[0].blur = 0;
  states_[0].spacing = 0;
  states_[0].color = 0xFFFFFFFF;
  states_[0].align = TextAlign::Baseline | TextAlign::Left;
}

FontStash::~FontStash() {
  /* Every block belongs to the Wall, so there is nothing to free: dropping
  the pointers is the whole teardown. */
  texture_ = _NilP;
  fonts_ = _NilP;
  scratch_ = _NilP;
  font_count_ = 0;
  font_max_ = 0;
  glyph_max_ = 0;
  state_count_ = 1;
}

BOL FontStash::Start(_::TWall<>* wall, ISC width, ISC height, ISC glyphs,
                     ISC fonts, BOL top_left) {
  if (wall == _NilP) return false;
  if (width <= 0 || height <= 0) return false;
  if (glyphs < StashGlyphsMin) glyphs = StashGlyphsMin;
  if (fonts < 1) fonts = 1;
  if (fonts > StashFontsMax) fonts = StashFontsMax;

  /* Every field is set here, so a cache placed in a Wall block needs no
  constructor: Start is the whole initialization. */
  wall_ = wall;
  top_left_ = top_left;
  font_count_ = 0;

  ISC bytes = width * height;
  if (bytes <= 0 || bytes > StashAtlasBytesMax) return false;
  texture_ = StashAllocate<IUA>(wall, bytes);
  if (texture_ == _NilP) return false;
  _::ArrayFill(texture_, bytes);

  /* One skyline node per column packs without quantizing a width, which is
  what keeps a glyph from being rounded into its neighbour. */
  if (!pack_.Start(wall, width, height, width)) return false;

  font_max_ = fonts;
  fonts_ = StashAllocate<StashFont>(wall, ISC(sizeof(StashFont)) * fonts);
  if (fonts_ == _NilP) return false;
  _::ArrayFill(fonts_, ISC(sizeof(StashFont)) * fonts);

  glyph_max_ = glyphs;
  /* One block for every font's glyph table: a Wall holds a fixed number of
  blocks, and a font table per font would spend that budget on bookkeeping
  instead of pixels. */
  StashGlyph* glyph_table = StashAllocate<StashGlyph>(
      wall, ISC(sizeof(StashGlyph)) * glyphs * fonts);
  if (glyph_table == _NilP) return false;
  for (ISC i = 0; i < fonts; ++i) {
    fonts_[i].glyphs = glyph_table + i * glyphs;
    fonts_[i].glyph_max = glyphs;
    fonts_[i].glyph_count = 0;
  }

  scratch_size_ = width * height;
  if (scratch_size_ > StashScratchMax) scratch_size_ = StashScratchMax;
  scratch_ = StashAllocate<IUA>(wall, scratch_size_);
  if (scratch_ == _NilP) return false;

  width_ = width;
  height_ = height;
  max_width_ = width;
  max_height_ = height;
  missing_ = 0;
  inverse_width_ = 1.0f / FPC(width);
  inverse_height_ = 1.0f / FPC(height);
  dirty_[0] = 0;
  dirty_[1] = 0;
  dirty_[2] = 0;
  dirty_[3] = 0;

  state_count_ = 1;
  _::ArrayFill(&states_[0], sizeof(StashState));
  states_[0].font = -1;
  states_[0].size = 16;
  states_[0].blur = 0;
  states_[0].spacing = 0;
  states_[0].color = 0xFFFFFFFF;
  states_[0].align = TextAlign::Baseline | TextAlign::Left;
  return true;
}

ISC FontStash::AddFont(const CHR* name, const IUA* data, ISC data_size) {
  if (fonts_ == _NilP || data == _NilP || data_size <= 0) return -1;
  if (font_count_ >= font_max_) return -1;

  StashFont* font = &fonts_[font_count_];
  /* The glyph table for this slot came out of the Wall in Start; keep it and
  clear the counters, so a font costs no more Room than it did then. */
  _::ArrayFill(font->glyphs, ISC(sizeof(StashGlyph)) * font->glyph_max);
  font->glyph_count = 0;
  font->data = data;
  font->data_size = data_size;
  font->fallback_count = 0;
  for (ISC i = 0; i < StashLutSize; ++i) font->lut[i] = -1;

  if (name != _NilP) {
    ISC length = TextLength_(name);
    if (length >= StashNameLength) length = StashNameLength - 1;
    _::ArrayCopy(font->name, sizeof(font->name), name, length);
    font->name[length] = 0;
  }

  if (!font->font.Start(wall_, data, 0)) return -1;

  /* A font's own line box rarely matches its size: normalize it so the
  caller's size is the line height and the ascender is a fraction of it. */
  VMetrics metrics;
  font->font.GetVerticalMetrics(metrics);
  ISC ascent = metrics.ascent + metrics.line_gap;
  ISC line = ascent - metrics.descent;
  if (line <= 0) return -1;
  font->ascender = FPC(ascent) / FPC(line);
  font->descender = FPC(metrics.descent) / FPC(line);

  ++font_count_;
  return font_count_ - 1;
}

ISC FontStash::FindFont(const CHR* name) const {
  if (name == _NilP || fonts_ == _NilP) return -1;
  for (ISC i = 0; i < font_count_; ++i) {
    const CHR* other = fonts_[i].name;
    ISC at = 0;
    while (other[at] != 0 && name[at] != 0 && other[at] == name[at]) ++at;
    if (other[at] == name[at]) return i;
  }
  return -1;
}

ISC FontStash::AddFallback(ISC base, ISC fallback) {
  StashFont* font = FontAt_(base);
  if (font == _NilP || FontAt_(fallback) == _NilP) return -1;
  if (font->fallback_count >= StashFallbacksMax) return -1;
  font->fallbacks[font->fallback_count++] = fallback;
  /* A cached glyph remembers the font that supplied it, so the tail of the
  cache that came from a fallback is no longer answerable: dropping the table
  is cheaper than tagging every entry with its source. */
  font->glyph_count = 0;
  for (ISC i = 0; i < StashLutSize; ++i) font->lut[i] = -1;
  return font->fallback_count;
}

void FontStash::ResetFallbacks(ISC base) {
  StashFont* font = FontAt_(base);
  if (font == _NilP) return;
  if (font->fallback_count == 0) return;
  font->fallback_count = 0;
  font->glyph_count = 0;
  for (ISC i = 0; i < StashLutSize; ++i) font->lut[i] = -1;
}

void FontStash::SetFont(ISC index) {
  if (index >= 0 && index < font_count_) Current_().font = index;
}

void FontStash::SetSize(FPC size) {
  if (size > 0) Current_().size = size;
}

void FontStash::SetColor(IUC color) { Current_().color = color; }

void FontStash::SetSpacing(FPC spacing) { Current_().spacing = spacing; }

void FontStash::SetBlur(FPC blur) {
  if (blur < 0) blur = 0;
  if (blur > FPC(StashBlurMax)) blur = FPC(StashBlurMax);
  Current_().blur = blur;
}

void FontStash::SetAlign(TextAlign align) { Current_().align = align; }

void FontStash::PushState() {
  if (state_count_ >= StashStatesMax) return;
  states_[state_count_] = states_[state_count_ - 1];
  ++state_count_;
}

void FontStash::PopState() {
  if (state_count_ > 1) --state_count_;
}

void FontStash::ClearState() {
  state_count_ = 1;
  states_[0].font = font_count_ > 0 ? 0 : -1;
  states_[0].size = 16;
  states_[0].blur = 0;
  states_[0].spacing = 0;
  states_[0].color = 0xFFFFFFFF;
  states_[0].align = TextAlign::Baseline | TextAlign::Left;
}

void FontStash::VerticalMetrics(FPC& ascender, FPC& descender,
                                FPC& line_height) {
  const StashFont* font = FontAt_(Current_().font);
  if (font == _NilP) {
    ascender = 0;
    descender = 0;
    line_height = 0;
    return;
  }
  ascender = font->ascender * Current_().size;
  descender = font->descender * Current_().size;
  line_height = ascender - descender;
}

void FontStash::LineBounds(FPC y, FPC& min_y, FPC& max_y) {
  const StashFont* font = FontAt_(Current_().font);
  if (font == _NilP) {
    min_y = y;
    max_y = y;
    return;
  }
  /* The line runs from one ascender above the baseline to one descender
  below it; the descender is negative, so it lands below in both axes
  conventions, on opposite sides of the pen. */
  FPC ascender = font->ascender * Current_().size;
  FPC descender = font->descender * Current_().size;
  if (top_left_) {
    min_y = y - ascender;
    max_y = y - descender;
  } else {
    min_y = y + descender;
    max_y = y + ascender;
  }
}

const IUA* FontStash::Texture(ISC& width, ISC& height) const {
  width = width_;
  height = height_;
  return texture_;
}

BOL FontStash::ValidateTexture(ISC rect[4]) {
  if (dirty_[0] >= dirty_[2] || dirty_[1] >= dirty_[3]) return false;
  if (rect != _NilP) {
    rect[0] = dirty_[0];
    rect[1] = dirty_[1];
    rect[2] = dirty_[2] - dirty_[0];
    rect[3] = dirty_[3] - dirty_[1];
  }
  dirty_[0] = width_;
  dirty_[1] = height_;
  dirty_[2] = 0;
  dirty_[3] = 0;
  return true;
}

BOL FontStash::ResetAtlas(ISC width, ISC height) {
  if (wall_ == _NilP || texture_ == _NilP) return false;
  if (width <= 0 || height <= 0) return false;
  /* The pixels are recycled, so the sheet cannot be bigger than the block
  Start took: the block is the ceiling, the working sheet may shrink back to
  it and grow again. */
  if (width > max_width_ || height > max_height_) return false;
  if (!pack_.Reset(width, height)) return false;
  ISC bytes = width * height;
  _::ArrayFill(texture_, bytes);
  for (ISC i = 0; i < font_count_; ++i) {
    fonts_[i].glyph_count = 0;
    for (ISC h = 0; h < StashLutSize; ++h) fonts_[i].lut[h] = -1;
  }
  width_ = width;
  height_ = height;
  missing_ = 0;
  inverse_width_ = 1.0f / FPC(width);
  inverse_height_ = 1.0f / FPC(height);
  dirty_[0] = 0;
  dirty_[1] = 0;
  dirty_[2] = width;
  dirty_[3] = height;
  return true;
}

ISC FontStash::Hash_(CHC code_point) {
  IUC a = IUC(code_point);
  a = (a ^ 61u) ^ (a >> 16);
  a = a + (a << 3);
  a = a ^ (a >> 4);
  a = a * 0x27d4eb2du;
  a = a ^ (a >> 15);
  return ISC(a & IUC(StashLutSize - 1));
}

ISC FontStash::TextLength_(const CHR* text) {
  ISC length = 0;
  while (text[length] != 0) ++length;
  return length;
}

StashFont* FontStash::FontAt_(ISC index) {
  if (index < 0 || index >= font_count_) return _NilP;
  return &fonts_[index];
}

const StashFont* FontStash::FontAt_(ISC index) const {
  if (index < 0 || index >= font_count_) return _NilP;
  return &fonts_[index];
}

void FontStash::Blur_(IUA* dst, ISC width, ISC height, ISC stride, ISC blur) {
  if (blur <= 0 || scratch_ == _NilP) return;
  if (width <= 0 || height <= 0) return;
  if (width * height > scratch_size_) return;

  FPC ratio = -2.3f / (FPC(blur) + 1.0f);
  ISC alpha = ISC((FPD(1 << BlurPrecision)) *
                  (1.0 - _::eToThe(FPD(ratio))));
  if (alpha <= 0) return;

  for (ISC y = 0; y < height; ++y)
    _::ArrayCopy(scratch_ + y * width, width, dst + y * stride, width);
  /* Two passes: one exponential sweep is a heavy smear, two make a bell
  that reads as a shadow rather than a motion blur. */
  for (ISC pass = 0; pass < 2; ++pass) {
    StashBlurColumns(scratch_, width, height, width, alpha);
    StashBlurRows(scratch_, width, height, width, alpha);
  }
  for (ISC y = 0; y < height; ++y)
    _::ArrayCopy(dst + y * stride, width, scratch_ + y * width, width);
}

StashGlyph* FontStash::Glyph_(ISC index, CHC code_point, ISC size, ISC blur,
                              BitmapMode mode) {
  StashFont* font = FontAt_(index);
  if (font == _NilP) return _NilP;
  if (size < 2) return _NilP;
  if (blur > StashBlurMax) blur = StashBlurMax;
  ISC border = blur + StashBorder;

  /* The cache holds one entry per codepoint, size and blur: a glyph drawn at
  24 px bold-shadowed is a different tile from the same glyph at 24 px sharp,
  and the hash chain is what keeps both. */
  ISC slot = Hash_(code_point);
  StashGlyph* glyph = _NilP;
  for (ISC i = font->lut[slot]; i >= 0; i = font->glyphs[i].next) {
    StashGlyph& candidate = font->glyphs[i];
    if (candidate.code_point == code_point && candidate.size == size &&
        candidate.blur == blur) {
      glyph = &candidate;
      break;
    }
  }
  if (glyph != _NilP) {
    if (mode == BitmapMode::Optional) return glyph;
    if (glyph->x0 >= 0 && glyph->y0 >= 0) return glyph;
  } else {
    if (font->glyph_count >= font->glyph_max) return _NilP;
    glyph = &font->glyphs[font->glyph_count];
    _::ArrayFill(glyph, sizeof(StashGlyph));
    glyph->code_point = code_point;
    glyph->size = size;
    glyph->blur = blur;
    glyph->x0 = glyph->y0 = glyph->x1 = glyph->y1 = -1;
    glyph->next = font->lut[slot];
    font->lut[slot] = font->glyph_count;
    ++font->glyph_count;
  }

  /* A codepoint the font has no glyph for goes down the fallback chain. When
  none of them has it either, glyph 0 is the notdef box and belongs to this
  font, which is what the caller asked to draw. */
  ISC glyph_index = font->font.FindGlyph(code_point);
  StashFont* render = font;
  if (glyph_index == 0) {
    for (ISC i = 0; i < font->fallback_count; ++i) {
      StashFont* candidate = FontAt_(font->fallbacks[i]);
      if (candidate == _NilP) continue;
      ISC found = candidate->font.FindGlyph(code_point);
      if (found != 0) {
        glyph_index = found;
        render = candidate;
        break;
      }
    }
  }

  FPC pixels = FPC(size) / FPC(StashSizeTenths);
  FPC scale = render->font.ScaleForPixelHeight(pixels);
  HMetrics metrics;
  render->font.GetHorizontalMetrics(glyph_index, metrics);
  BitmapBox box;
  render->font.GetBitmapBox(glyph_index, scale, scale, box);

  glyph->index = glyph_index;
  glyph->w = box.x1 - box.x0 + border * 2;
  glyph->h = box.y1 - box.y0 + border * 2;
  glyph->advance = ISC(scale * FPC(metrics.advance_width) *
                       FPC(StashSizeTenths));
  glyph->x_offset = box.x0 - border;
  glyph->y_offset = box.y0 - border;

  if (mode == BitmapMode::Optional) return glyph;
  if (glyph->w <= 0 || glyph->h <= 0) return glyph;

  ISC gx, gy;
  if (!pack_.Place(glyph->w, glyph->h, gx, gy)) {
    /* The atlas is full. The tile keeps its metrics so the line still
    advances and the glyph shows up once the caller resets the atlas larger. */
    glyph->x0 = glyph->y0 = glyph->x1 = glyph->y1 = -1;
    ++missing_;
    return glyph;
  }
  glyph->x0 = gx;
  glyph->y0 = gy;
  glyph->x1 = gx + glyph->w;
  glyph->y1 = gy + glyph->h;

  IUA* tile = texture_ + glyph->y0 * width_ + glyph->x0;
  render->font.MakeGlyphBitmap(glyph_index, scale, scale,
                               tile + border * width_ + border,
                               glyph->w - border * 2, glyph->h - border * 2,
                               width_);

  /* One clear pixel all the way round: bilinear filtering reaches half a
  texel outside the quad and would otherwise pull the neighbour glyph in. */
  for (ISC y = 0; y < glyph->h; ++y) {
    tile[y * width_] = 0;
    tile[glyph->w - 1 + y * width_] = 0;
  }
  for (ISC x = 0; x < glyph->w; ++x) {
    tile[x] = 0;
    tile[x + (glyph->h - 1) * width_] = 0;
  }

  if (blur > 0) Blur_(tile, glyph->w, glyph->h, width_, blur);

  if (dirty_[0] > glyph->x0) dirty_[0] = glyph->x0;
  if (dirty_[1] > glyph->y0) dirty_[1] = glyph->y0;
  if (dirty_[2] < glyph->x1) dirty_[2] = glyph->x1;
  if (dirty_[3] < glyph->y1) dirty_[3] = glyph->y1;
  return glyph;
}

void FontStash::Quad_(StashFont& font, ISC prev_glyph, StashGlyph& glyph,
                      FPC spacing, FPC& x, FPC& y, StashQuad& quad) {
  FPC scale = font.font.ScaleForPixelHeight(
      FPC(glyph.size) / FPC(StashSizeTenths));

  if (prev_glyph >= 0) {
    FPC kern = FPC(font.font.GetKernAdvance(prev_glyph, glyph.index));
    x += FPC(ISC(kern * scale + spacing + 0.5f));
  }

  /* The tile carries a clear border on every side. The quad covers the inner
  part of it and is floored to whole pixels, which is what keeps a stem one
  pixel wide instead of one and a bit of smeared grey. */
  FPC width = FPC(glyph.w - StashBorder);
  FPC height = FPC(glyph.h - StashBorder);
  FPC left = _::Floor(x + FPC(glyph.x_offset + 1));
  FPC top;
  if (top_left_) {
    top = _::Floor(y + FPC(glyph.y_offset + 1));
    quad.x0 = left;
    quad.y0 = top;
    quad.x1 = left + width;
    quad.y1 = top + height;
  } else {
    top = _::Floor(y - FPC(glyph.y_offset + 1));
    quad.x0 = left;
    quad.y0 = top;
    quad.x1 = left + width;
    quad.y1 = top - height;
  }

  /* The rectangle is always known: it comes from the font's box, not from
  the atlas. Only the texture coordinates need a tile, which is what lets a
  measuring pass report a box without rasterizing anything. */
    if (glyph.x0 >= 0 && glyph.y0 >= 0) {
      quad.s0 = FPC(glyph.x0 + 1) * inverse_width_;
      quad.t0 = FPC(glyph.y0 + 1) * inverse_height_;
      quad.s1 = FPC(glyph.x1 - 1) * inverse_width_;
      quad.t1 = FPC(glyph.y1 - 1) * inverse_height_;
    } else {
      quad.s0 = quad.t0 = quad.s1 = quad.t1 = 0;
    }

  x += FPC(ISC(FPC(glyph.advance) / FPC(StashSizeTenths) + 0.5f));
}

FPC FontStash::VerticalAlign_(const StashFont& font, TextAlign align,
                               ISC size) const {
  FPC pixels = FPC(size) / FPC(StashSizeTenths);
  FPC ascender = font.ascender * pixels;
  FPC descender = font.descender * pixels;

  /* Y grows down from the top left, so the top of the text sits below the
  pen by the ascender. In bottom left coordinates the signs invert. */
  if (top_left_) {
    if (HasAlign(align, TextAlign::Top)) return ascender;
    if (HasAlign(align, TextAlign::Middle)) return (ascender + descender) * 0.5f;
    if (HasAlign(align, TextAlign::Bottom)) return descender;
    return 0;
  }
  if (HasAlign(align, TextAlign::Top)) return -descender;
  if (HasAlign(align, TextAlign::Middle)) return -(ascender + descender) * 0.5f;
  if (HasAlign(align, TextAlign::Bottom)) return -ascender;
  return 0;
}

BOL FontStash::TextIterInit(TextIter& iter, FPC x, FPC y, const CHR* text,
                            const CHR* end, BitmapMode mode) {
  const StashState& state = Current_();
  StashFont* font = FontAt_(state.font);
  if (text == _NilP || font == _NilP) return false;
  if (end == _NilP) end = text + TextLength_(text);
  if (end < text) return false;

  iter.font = font;
  iter.mode = mode;
  iter.size = ISC(state.size * FPC(StashSizeTenths));
  iter.blur = ISC(state.blur);
  iter.scale = font->font.ScaleForPixelHeight(state.size);
  iter.spacing = state.spacing;

  /* Horizontal anchoring needs the width before the first glyph is placed,
  and measuring leaves the atlas alone. */
  if (HasAlign(state.align, TextAlign::Right))
    x -= TextBounds(x, y, text, end, _NilP);
  else if (HasAlign(state.align, TextAlign::Center))
    x -= TextBounds(x, y, text, end, _NilP) * 0.5f;
  y += VerticalAlign_(*font, state.align, iter.size);

  iter.cursor = text;
  iter.end = end;
  iter.x = iter.next_x = x;
  iter.y = iter.next_y = y;
  iter.code_point = 0;
  iter.prev_glyph = -1;
  return true;
}

BOL FontStash::TextIterNext(TextIter& iter, StashQuad& quad) {
  if (iter.cursor == _NilP || iter.cursor >= iter.end) return false;

  CHC code_point = 0;
  const CHR* next = _::SScan(iter.cursor, code_point);
  if (next == _NilP || next > iter.end) {
    /* A malformed sequence costs one unit and draws nothing, so a damaged
    string loses a character instead of the rest of the line. */
    ++iter.cursor;
    iter.x = iter.next_x;
    iter.y = iter.next_y;
    iter.prev_glyph = -1;
    return true;
  }
  iter.cursor = next;
  iter.x = iter.next_x;
  iter.y = iter.next_y;
  iter.code_point = code_point;

  ISC index = ISC(iter.font - fonts_);
  StashGlyph* glyph = Glyph_(index, code_point, iter.size, iter.blur,
                             iter.mode);
  if (glyph == _NilP) {
    iter.prev_glyph = -1;
    return true;
  }
  Quad_(*iter.font, iter.prev_glyph, *glyph, iter.spacing, iter.next_x,
        iter.next_y, quad);
  if (glyph->x0 < 0 || glyph->y0 < 0) {
    /* The pen moved, but there is no tile to sample: hand back a degenerate
    quad so the caller draws nothing instead of whatever was in the last one. */
    quad.x0 = quad.x1 = quad.y0 = quad.y1 = 0;
  }
  iter.prev_glyph = glyph->index;
  return true;
}

FPC FontStash::TextBounds(FPC x, FPC y, const CHR* text, const CHR* end,
                          FPC bounds[4]) {
  const StashState& state = Current_();
  ISC index = state.font;
  StashFont* font = FontAt_(index);
  if (text == _NilP || font == _NilP) return 0;
  if (end == _NilP) end = text + TextLength_(text);
  if (end < text) return 0;

  ISC size = ISC(state.size * FPC(StashSizeTenths));
  ISC blur = ISC(state.blur);

  FPC pen_x = x;
  FPC pen_y = y;
  FPC min_x = 0, min_y = 0, max_x = 0, max_y = 0;
  ISC prev_glyph = -1;
  BOL any = false;
  StashQuad quad;
  const CHR* cursor = text;

  while (cursor < end) {
    CHC code_point = 0;
    const CHR* next = _::SScan(cursor, code_point);
    if (next == _NilP || next > end) {
      ++cursor;
      prev_glyph = -1;
      continue;
    }
    cursor = next;
    StashGlyph* glyph = Glyph_(index, code_point, size, blur,
                               BitmapMode::Optional);
    if (glyph == _NilP) {
      prev_glyph = -1;
      continue;
    }
    Quad_(*font, prev_glyph, *glyph, state.spacing, pen_x, pen_y, quad);
    prev_glyph = glyph->index;
    if (!any) {
      min_x = quad.x0;
      min_y = quad.y0;
      max_x = quad.x1;
      max_y = quad.y1;
      any = true;
    } else {
      if (min_x > quad.x0) min_x = quad.x0;
      if (min_y > quad.y0) min_y = quad.y0;
      if (max_x < quad.x1) max_x = quad.x1;
      if (max_y < quad.y1) max_y = quad.y1;
    }
  }

  if (bounds != _NilP) {
    bounds[0] = min_x;
    bounds[1] = min_y;
    bounds[2] = max_x;
    bounds[3] = max_y;
  }
  return pen_x - x;
}

}  // namespace Exo
}  // namespace CTK
#endif
