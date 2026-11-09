#pragma once
#ifndef FONS_H
#define FONS_H
#include <ASCIICrabs/_ConfigHeader.h>
#include <ASCIICrabs/_ConfigDefault.h>
#include <ASCIICrabs/_ConfigFooter.h>
#include <ASCIICrabs/Binary.hpp>
#include <ASCIICrabs/Array.hxx>
#include <ASCIICrabs/Autoject.hxx>

#define FONS_INVALID -1

struct FONScontext;

enum FONSflags {
  FONS_ZERO_TOPLEFT = 1,
  FONS_ZERO_BOTTOMLEFT = 2,
};

enum FONSalign {
  // Horizontal align
  FONS_ALIGN_LEFT   = 1<<0,  // Default
  FONS_ALIGN_CENTER   = 1<<1,
  FONS_ALIGN_RIGHT   = 1<<2,
  // Vertical align
  FONS_ALIGN_TOP     = 1<<3,
  FONS_ALIGN_MIDDLE  = 1<<4,
  FONS_ALIGN_BOTTOM  = 1<<5,
  FONS_ALIGN_BASELINE  = 1<<6, // Default
};

enum FONSglyphBitmap {
  FONS_GLYPH_BITMAP_OPTIONAL = 1,
  FONS_GLYPH_BITMAP_REQUIRED = 2,
};

enum FONSerrorCode {
  // Font atlas is full.
  FONS_ATLAS_FULL = 1,
  // Scratch memory used to render glyphs is full, requested size reported in 'val', you may need to bump up FONS_SCRATCH_BUF_SIZE.
  FONS_SCRATCH_FULL = 2,
  // Calls to fonsPushState has created too large stack, if you need deep state stack bump up FONS_MAX_STATES.
  FONS_STATES_OVERFLOW = 3,
  // Trying to pop too many states fonsPopState().
  FONS_STATES_UNDERFLOW = 4,
};

struct FONSparams {
  ISC width, height;
  IUA flags;
  void* userPtr;
  ISC (*renderCreate)(void* uptr, ISC width, ISC height);
  ISC (*renderResize)(void* uptr, ISC width, ISC height);
  void (*renderUpdate)(void* uptr, ISC* rect, const IUA* data);
  void (*renderDraw)(void* uptr, const FPC* verts, const FPC* tcoords, const IUC* colors, ISC nverts);
  void (*renderDelete)(void* uptr);
};

struct FONSquad
{
  FPC x0,y0,s0,t0;
  FPC x1,y1,s1,t1;
};

struct FONStextIter {
  FPC x, y, nextx, nexty, scale, spacing;
  IUC codepoint;
  ISB isize, iblur;
  struct FONSfont* font;
  ISC prevGlyphIndex;
  const CHR* str;
  const CHR* next;
  const CHR* end;
  IUC utf8state;
  ISC bitmapOption;
};

// Constructor and destructor.
FONScontext* fonsCreateInternal(FONSparams* params);
void fonsDeleteInternal(FONScontext* s);

void fonsSetErrorCallback(FONScontext* s, void (*callback)(void* uptr, ISC error, ISC val), void* uptr);
// Returns current atlas size.
void fonsGetAtlasSize(FONScontext* s, ISC* width, ISC* height);
// Expands the atlas size.
ISC fonsExpandAtlas(FONScontext* s, ISC width, ISC height);
// Resets the whole stash.
ISC fonsResetAtlas(FONScontext* stash, ISC width, ISC height);

// Add fonts
ISC fonsAddFont(FONScontext* s, const CHR* name, const CHR* path, ISC fontIndex);
ISC fonsAddFontMem(FONScontext* s, const CHR* name, IUA* data, ISC ndata, ISC freeData, ISC fontIndex);
ISC fonsGetFontByName(FONScontext* s, const CHR* name);

// State handling
void fonsPushState(FONScontext* s);
void fonsPopState(FONScontext* s);
void fonsClearState(FONScontext* s);

// State setting
void fonsSetSize(FONScontext* s, FPC size);
void fonsSetColor(FONScontext* s, IUC color);
void fonsSetSpacing(FONScontext* s, FPC spacing);
void fonsSetBlur(FONScontext* s, FPC blur);
void fonsSetAlign(FONScontext* s, ISC align);
void fonsSetFont(FONScontext* s, ISC font);

// Draw text
FPC fonsDrawText(FONScontext* s, FPC x, FPC y, const CHR* string, const CHR* end);

// Measure text
FPC fonsTextBounds(FONScontext* s, FPC x, FPC y, const CHR* string, const CHR* end, FPC* bounds);
void fonsLineBounds(FONScontext* s, FPC y, FPC* miny, FPC* maxy);
void fonsVertMetrics(FONScontext* s, FPC* ascender, FPC* descender, FPC* lineh);

// Text iterator
ISC fonsTextIterInit(FONScontext* stash, FONStextIter* iter, FPC x, FPC y, const CHR* str, const CHR* end, ISC bitmapOption);
ISC fonsTextIterNext(FONScontext* stash, FONStextIter* iter, struct FONSquad* quad);

// Pull texture changes
const IUA* fonsGetTextureData(FONScontext* stash, ISC* width, ISC* height);
ISC fonsValidateTexture(FONScontext* s, ISC* dirty);

// Draws the stash texture for debugging
void fonsDrawDebug(FONScontext* s, FPC x, FPC y);

#endif // FONTSTASH_H


#ifdef FONTSTASH_IMPLEMENTATION

#define FONS_NOTUSED(v)  (void)sizeof(v)

#ifdef FONS_USE_FREETYPE

#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_ADVANCES_H
#include <math.h>

struct FONSttFontImpl {
  FT_Face font;
};

#else

#define STB_TRUETYPE_IMPLEMENTATION

void* fons__tmpalloc(ISW size, void* up);
void fons__tmpfree(void* ptr, void* up);
#define STBTT_malloc(x,u)    fons__tmpalloc(x,u)
#define STBTT_free(x,u)      fons__tmpfree(x,u)
#include "imstb_truetype.h"

struct FONSttFontImpl {
  stbtt_fontinfo font;
};

#endif

#ifndef FONS_SCRATCH_BUF_SIZE
#  define FONS_SCRATCH_BUF_SIZE 96000
#endif
#ifndef FONS_HASH_LUT_SIZE
#  define FONS_HASH_LUT_SIZE 256
#endif
#ifndef FONS_INIT_FONTS
#  define FONS_INIT_FONTS 4
#endif
#ifndef FONS_INIT_GLYPHS
#  define FONS_INIT_GLYPHS 256
#endif
#ifndef FONS_INIT_ATLAS_NODES
#  define FONS_INIT_ATLAS_NODES 256
#endif
#ifndef FONS_VERTEX_COUNT
#  define FONS_VERTEX_COUNT 1024
#endif
#ifndef FONS_MAX_STATES
#  define FONS_MAX_STATES 20
#endif
#ifndef FONS_MAX_FALLBACKS
#  define FONS_MAX_FALLBACKS 20
#endif

IUC fons__hashint(IUC a)
{
  a += ~(a<<15);
  a ^=  (a>>10);
  a +=  (a<<3);
  a ^=  (a>>6);
  a += ~(a<<11);
  a ^=  (a>>16);
  return a;
}

ISC fons__mini(ISC a, ISC b)
{
  return a < b ? a : b;
}

ISC fons__maxi(ISC a, ISC b)
{
  return a > b ? a : b;
}

struct FONSglyph
{
  IUC codepoint;
  ISC index;
  ISC next;
  ISB size, blur;
  ISB x0,y0,x1,y1;
  ISB xadv,xoff,yoff;
};

struct FONSfont
{
  FONSttFontImpl font;
  CHR name[64];
  IUA* data;
  ISC dataSize;
  IUA freeData;
  FPC ascender;
  FPC descender;
  FPC lineh;
  FONSglyph* glyphs;
  ISC cglyphs;
  ISC nglyphs;
  ISC lut[FONS_HASH_LUT_SIZE];
  ISC fallbacks[FONS_MAX_FALLBACKS];
  ISC nfallbacks;
};

struct FONSstate
{
  ISC font;
  ISC align;
  FPC size;
  IUC color;
  FPC blur;
  FPC spacing;
};

struct FONSatlasNode {
    ISB x, y, width;
};

struct FONSatlas
{
  ISC width, height;
  FONSatlasNode* nodes;
  ISC nnodes;
  ISC cnodes;
};

struct FONScontext
{
  FONSparams params;
  FPC itw,ith;
  IUA* texData;
  ISC dirtyRect[4];
  FONSfont** fonts;
  FONSatlas* atlas;
  ISC cfonts;
  ISC nfonts;
  FPC verts[FONS_VERTEX_COUNT*2];
  FPC tcoords[FONS_VERTEX_COUNT*2];
  IUC colors[FONS_VERTEX_COUNT];
  ISC nverts;
  IUA* scratch;
  ISC nscratch;
  FONSstate states[FONS_MAX_STATES];
  ISC nstates;
  void (*handleError)(void* uptr, ISC error, ISC val);
  void* errorUptr;
#ifdef FONS_USE_FREETYPE
  FT_Library ftLibrary;
#endif
};

#ifdef FONS_USE_FREETYPE

ISC fons__tt_init(FONScontext *context)
{
  FT_Error ftError;
  FONS_NOTUSED(context);
  ftError = FT_Init_FreeType(&context->ftLibrary);
  return ftError == 0;
}

ISC fons__tt_done(FONScontext *context)
{
  FT_Error ftError;
  FONS_NOTUSED(context);
  ftError = FT_Done_FreeType(context->ftLibrary);
  return ftError == 0;
}

ISC fons__tt_loadFont(FONScontext *context, FONSttFontImpl *font, IUA *data, ISC dataSize, ISC fontIndex)
{
  FT_Error ftError;
  FONS_NOTUSED(context);

  ftError = FT_New_Memory_Face(context->ftLibrary, (const FT_Byte*)data, dataSize, fontIndex, &font->font);
  return ftError == 0;
}

void fons__tt_getFontVMetrics(FONSttFontImpl *font, ISC *ascent, ISC *descent, ISC *lineGap)
{
  *ascent = font->font->ascender;
  *descent = font->font->descender;
  *lineGap = font->font->height - (*ascent - *descent);
}

FPC fons__tt_getPixelHeightScale(FONSttFontImpl *font, FPC size)
{
  return size / font->font->units_per_EM;
}

ISC fons__tt_getGlyphIndex(FONSttFontImpl *font, ISC codepoint)
{
  return FT_Get_Char_Index(font->font, codepoint);
}

ISC fons__tt_buildGlyphBitmap(FONSttFontImpl *font, ISC glyph, FPC size, FPC scale,
                ISC *advance, ISC *lsb, ISC *x0, ISC *y0, ISC *x1, ISC *y1)
{
  FT_Error ftError;
  FT_GlyphSlot ftGlyph;
  FT_Fixed advFixed;
  FONS_NOTUSED(scale);

  ftError = FT_Set_Pixel_Sizes(font->font, 0, size);
  if (ftError) return 0;
  ftError = FT_Load_Glyph(font->font, glyph, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
  if (ftError) return 0;
  ftError = FT_Get_Advance(font->font, glyph, FT_LOAD_NO_SCALE, &advFixed);
  if (ftError) return 0;
  ftGlyph = font->font->glyph;
  *advance = (ISC)advFixed;
  *lsb = (ISC)ftGlyph->metrics.horiBearingX;
  *x0 = ftGlyph->bitmap_left;
  *x1 = *x0 + ftGlyph->bitmap.width;
  *y0 = -ftGlyph->bitmap_top;
  *y1 = *y0 + ftGlyph->bitmap.rows;
  return 1;
}

void fons__tt_renderGlyphBitmap(FONSttFontImpl *font, IUA *output, ISC outWidth, ISC outHeight, ISC outStride,
                FPC scaleX, FPC scaleY, ISC glyph)
{
  FT_GlyphSlot ftGlyph = font->font->glyph;
  ISC ftGlyphOffset = 0;
  IUC x, y;
  FONS_NOTUSED(outWidth);
  FONS_NOTUSED(outHeight);
  FONS_NOTUSED(scaleX);
  FONS_NOTUSED(scaleY);
  FONS_NOTUSED(glyph);  // glyph has already been loaded by fons__tt_buildGlyphBitmap

  for ( y = 0; y < ftGlyph->bitmap.rows; y++ ) {
    for ( x = 0; x < ftGlyph->bitmap.width; x++ ) {
      output[(y * outStride) + x] = ftGlyph->bitmap.buffer[ftGlyphOffset++];
    }
  }
}

ISC fons__tt_getGlyphKernAdvance(FONSttFontImpl *font, ISC glyph1, ISC glyph2)
{
  FT_Vector ftKerning;
  FT_Get_Kerning(font->font, glyph1, glyph2, FT_KERNING_DEFAULT, &ftKerning);
  return (ISC)((ftKerning.x + 32) >> 6);  // Round up and convert to integer
}

#else

ISC fons__tt_init(FONScontext *context)
{
  FONS_NOTUSED(context);
  return 1;
}

ISC fons__tt_done(FONScontext *context)
{
  FONS_NOTUSED(context);
  return 1;
}

ISC fons__tt_loadFont(FONScontext *context, FONSttFontImpl *font, IUA *data, ISC dataSize, ISC fontIndex)
{
  ISC offset, stbError;
  FONS_NOTUSED(dataSize);

  font->font.userdata = context;
  offset = stbtt_GetFontOffsetForIndex(data, fontIndex);
  if (offset == -1) {
    stbError = 0;
  } else {
    stbError = stbtt_InitFont(&font->font, data, offset);
  }
  return stbError;
}

void fons__tt_getFontVMetrics(FONSttFontImpl *font, ISC *ascent, ISC *descent, ISC *lineGap)
{
  stbtt_GetFontVMetrics(&font->font, ascent, descent, lineGap);
}

FPC fons__tt_getPixelHeightScale(FONSttFontImpl *font, FPC size)
{
  return stbtt_ScaleForMappingEmToPixels(&font->font, size);
}

ISC fons__tt_getGlyphIndex(FONSttFontImpl *font, ISC codepoint)
{
  return stbtt_FindGlyphIndex(&font->font, codepoint);
}

ISC fons__tt_buildGlyphBitmap(FONSttFontImpl *font, ISC glyph, FPC size, FPC scale,
                ISC *advance, ISC *lsb, ISC *x0, ISC *y0, ISC *x1, ISC *y1)
{
  FONS_NOTUSED(size);
  stbtt_GetGlyphHMetrics(&font->font, glyph, advance, lsb);
  stbtt_GetGlyphBitmapBox(&font->font, glyph, scale, scale, x0, y0, x1, y1);
  return 1;
}

void fons__tt_renderGlyphBitmap(FONSttFontImpl *font, IUA *output, ISC outWidth, ISC outHeight, ISC outStride,
                FPC scaleX, FPC scaleY, ISC glyph)
{
  stbtt_MakeGlyphBitmap(&font->font, output, outWidth, outHeight, outStride, scaleX, scaleY, glyph);
}

ISC fons__tt_getGlyphKernAdvance(FONSttFontImpl *font, ISC glyph1, ISC glyph2)
{
  return stbtt_GetGlyphKernAdvance(&font->font, glyph1, glyph2);
}

#endif

#ifdef STB_TRUETYPE_IMPLEMENTATION

void* fons__tmpalloc(ISW size, void* up)
{
  IUA* ptr;
  FONScontext* stash = (FONScontext*)up;

  // 16-byte align the returned pointer
  size = (size + 0xf) & ~0xf;

  if (stash->nscratch+(ISC)size > FONS_SCRATCH_BUF_SIZE) {
    if (stash->handleError)
      stash->handleError(stash->errorUptr, FONS_SCRATCH_FULL, stash->nscratch+(ISC)size);
    return nullptr;
  }
  ptr = stash->scratch + stash->nscratch;
  stash->nscratch += (ISC)size;
  return ptr;
}

void fons__tmpfree(void* ptr, void* up)
{
  (void)ptr;
  (void)up;
  // empty
}

#endif // STB_TRUETYPE_IMPLEMENTATION

// Copyright (c) 2008-2010 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

#define FONS_UTF8_ACCEPT 0
#define FONS_UTF8_REJECT 12

IUC fons__decutf8(IUC* state, IUC* codep, IUC byte)
{
  const IUA utf8d[] = {
    // The first part of the table maps bytes to character classes that
    // to reduce the size of the transition table and create bitmasks.
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,

    // The second part is a transition table that maps a combination
    // of a state of the automaton and a character class to a state.
    0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
    12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
    12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
    12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
    12,36,12,12,12,12,12,12,12,12,12,12,
    };

  IUC type = utf8d[byte];

    *codep = (*state != FONS_UTF8_ACCEPT) ?
    (byte & 0x3fu) | (*codep << 6) :
    (0xff >> type) & (byte);

  *state = utf8d[256 + *state + type];
  return *state;
}

// Atlas based on Skyline Bin Packer by Jukka Jylänki

/* Font-atlas allocation via the ASCIICrabs RAMFactory. No in-place grow, so
   realloc allocs the bigger block, copies the old_bytes live, frees the old. */
void* fons__alloc(ISW bytes) {
  if (bytes <= 0) return _NilP;
  return _::ObjectFactoryHeap(_NilP, bytes);
}
void fons__free(void* origin) {
  if (origin != _NilP) _::ObjectFactoryHeap((IUW*)origin, 0);
}
void* fons__realloc(void* origin, ISW old_bytes, ISW new_bytes) {
  if (origin == _NilP) return fons__alloc(new_bytes);
  if (new_bytes <= 0) { fons__free(origin); return _NilP; }
  if (new_bytes <= old_bytes) return origin;
  void* grown = fons__alloc(new_bytes);
  if (grown == _NilP) return origin;
  _::ArrayCopy(grown, new_bytes, origin, old_bytes);
  fons__free(origin);
  return grown;
}
ISC fons__cstrlen(const CHR* s) {
  ISC n = 0;
  while (s[n] != '\0') ++n;
  return n;
}
ISC fons__cstrcmp(const CHR* a, const CHR* b) {
  while (*a && *a == *b) { ++a; ++b; }
  return (ISC)(IUA)*a - (ISC)(IUA)*b;
}

void fons__deleteAtlas(FONSatlas* atlas)
{
  if (atlas == nullptr) return;
  if (atlas->nodes != nullptr) fons__free(atlas->nodes);
  fons__free(atlas);
}

FONSatlas* fons__allocAtlas(ISC w, ISC h, ISC nnodes)
{
  FONSatlas* atlas = nullptr;

  // Allocate memory for the font stash.
  atlas = (FONSatlas*)fons__alloc(sizeof(FONSatlas));
  if (atlas == nullptr) goto error;
  _::ArrayFill(atlas, sizeof(FONSatlas));

  atlas->width = w;
  atlas->height = h;

  // Allocate space for skyline nodes
  atlas->nodes = (FONSatlasNode*)fons__alloc(sizeof(FONSatlasNode) * nnodes);
  if (atlas->nodes == nullptr) goto error;
  _::ArrayFill(atlas->nodes, sizeof(FONSatlasNode) * nnodes);
  atlas->nnodes = 0;
  atlas->cnodes = nnodes;

  // Init root node.
  atlas->nodes[0].x = 0;
  atlas->nodes[0].y = 0;
  atlas->nodes[0].width = (ISB)w;
  atlas->nnodes++;

  return atlas;

error:
  if (atlas) fons__deleteAtlas(atlas);
  return nullptr;
}

ISC fons__atlasInsertNode(FONSatlas* atlas, ISC idx, ISC x, ISC y, ISC w)
{
  ISC i;
  // Insert node
  if (atlas->nnodes+1 > atlas->cnodes) {
    atlas->cnodes = atlas->cnodes == 0 ? 8 : atlas->cnodes * 2;
    atlas->nodes = (FONSatlasNode*)fons__realloc(atlas->nodes, atlas->nnodes * sizeof(FONSatlasNode), sizeof(FONSatlasNode) * atlas->cnodes);
    if (atlas->nodes == nullptr)
      return 0;
  }
  for (i = atlas->nnodes; i > idx; i--)
    atlas->nodes[i] = atlas->nodes[i-1];
  atlas->nodes[idx].x = (ISB)x;
  atlas->nodes[idx].y = (ISB)y;
  atlas->nodes[idx].width = (ISB)w;
  atlas->nnodes++;

  return 1;
}

void fons__atlasRemoveNode(FONSatlas* atlas, ISC idx)
{
  ISC i;
  if (atlas->nnodes == 0) return;
  for (i = idx; i < atlas->nnodes-1; i++)
    atlas->nodes[i] = atlas->nodes[i+1];
  atlas->nnodes--;
}

void fons__atlasExpand(FONSatlas* atlas, ISC w, ISC h)
{
  // Insert node for empty space
  if (w > atlas->width)
    fons__atlasInsertNode(atlas, atlas->nnodes, atlas->width, 0, w - atlas->width);
  atlas->width = w;
  atlas->height = h;
}

void fons__atlasReset(FONSatlas* atlas, ISC w, ISC h)
{
  atlas->width = w;
  atlas->height = h;
  atlas->nnodes = 0;

  // Init root node.
  atlas->nodes[0].x = 0;
  atlas->nodes[0].y = 0;
  atlas->nodes[0].width = (ISB)w;
  atlas->nnodes++;
}

ISC fons__atlasAddSkylineLevel(FONSatlas* atlas, ISC idx, ISC x, ISC y, ISC w, ISC h)
{
  ISC i;

  // Insert new node
  if (fons__atlasInsertNode(atlas, idx, x, y+h, w) == 0)
    return 0;

  // Delete skyline segments that fall under the shadow of the new segment.
  for (i = idx+1; i < atlas->nnodes; i++) {
    if (atlas->nodes[i].x < atlas->nodes[i-1].x + atlas->nodes[i-1].width) {
      ISC shrink = atlas->nodes[i-1].x + atlas->nodes[i-1].width - atlas->nodes[i].x;
      atlas->nodes[i].x += (ISB)shrink;
      atlas->nodes[i].width -= (ISB)shrink;
      if (atlas->nodes[i].width <= 0) {
        fons__atlasRemoveNode(atlas, i);
        i--;
      } else {
        break;
      }
    } else {
      break;
    }
  }

  // Merge same height skyline segments that are next to each other.
  for (i = 0; i < atlas->nnodes-1; i++) {
    if (atlas->nodes[i].y == atlas->nodes[i+1].y) {
      atlas->nodes[i].width += atlas->nodes[i+1].width;
      fons__atlasRemoveNode(atlas, i+1);
      i--;
    }
  }

  return 1;
}

ISC fons__atlasRectFits(FONSatlas* atlas, ISC i, ISC w, ISC h)
{
  // Checks if there is enough space at the location of skyline span 'i',
  // and return the max height of all skyline spans under that at that location,
  // (think tetris block being dropped at that position). Or -1 if no space found.
  ISC x = atlas->nodes[i].x;
  ISC y = atlas->nodes[i].y;
  ISC spaceLeft;
  if (x + w > atlas->width)
    return -1;
  spaceLeft = w;
  while (spaceLeft > 0) {
    if (i == atlas->nnodes) return -1;
    y = fons__maxi(y, atlas->nodes[i].y);
    if (y + h > atlas->height) return -1;
    spaceLeft -= atlas->nodes[i].width;
    ++i;
  }
  return y;
}

ISC fons__atlasAddRect(FONSatlas* atlas, ISC rw, ISC rh, ISC* rx, ISC* ry)
{
  ISC besth = atlas->height, bestw = atlas->width, besti = -1;
  ISC bestx = -1, besty = -1, i;

  // Bottom left fit heuristic.
  for (i = 0; i < atlas->nnodes; i++) {
    ISC y = fons__atlasRectFits(atlas, i, rw, rh);
    if (y != -1) {
      if (y + rh < besth || (y + rh == besth && atlas->nodes[i].width < bestw)) {
        besti = i;
        bestw = atlas->nodes[i].width;
        besth = y + rh;
        bestx = atlas->nodes[i].x;
        besty = y;
      }
    }
  }

  if (besti == -1)
    return 0;

  // Perform the actual packing.
  if (fons__atlasAddSkylineLevel(atlas, besti, bestx, besty, rw, rh) == 0)
    return 0;

  *rx = bestx;
  *ry = besty;

  return 1;
}

void fons__addWhiteRect(FONScontext* stash, ISC w, ISC h)
{
  ISC x, y, gx, gy;
  IUA* dst;
  if (fons__atlasAddRect(stash->atlas, w, h, &gx, &gy) == 0)
    return;

  // Rasterize
  dst = &stash->texData[gx + gy * stash->params.width];
  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++)
      dst[x] = 0xff;
    dst += stash->params.width;
  }

  stash->dirtyRect[0] = fons__mini(stash->dirtyRect[0], gx);
  stash->dirtyRect[1] = fons__mini(stash->dirtyRect[1], gy);
  stash->dirtyRect[2] = fons__maxi(stash->dirtyRect[2], gx+w);
  stash->dirtyRect[3] = fons__maxi(stash->dirtyRect[3], gy+h);
}

FONScontext* fonsCreateInternal(FONSparams* params)
{
  FONScontext* stash = nullptr;

  // Allocate memory for the font stash.
  stash = (FONScontext*)fons__alloc(sizeof(FONScontext));
  if (stash == nullptr) goto error;
  _::ArrayFill(stash, sizeof(FONScontext));

  stash->params = *params;

  // Allocate scratch buffer.
  stash->scratch = (IUA*)fons__alloc(FONS_SCRATCH_BUF_SIZE);
  if (stash->scratch == nullptr) goto error;

  // Initialize implementation library
  if (!fons__tt_init(stash)) goto error;

  if (stash->params.renderCreate != nullptr) {
    if (stash->params.renderCreate(stash->params.userPtr, stash->params.width, stash->params.height) == 0)
      goto error;
  }

  stash->atlas = fons__allocAtlas(stash->params.width, stash->params.height, FONS_INIT_ATLAS_NODES);
  if (stash->atlas == nullptr) goto error;

  // Allocate space for fonts.
  stash->fonts = (FONSfont**)fons__alloc(sizeof(FONSfont*) * FONS_INIT_FONTS);
  if (stash->fonts == nullptr) goto error;
  _::ArrayFill(stash->fonts, sizeof(FONSfont*) * FONS_INIT_FONTS);
  stash->cfonts = FONS_INIT_FONTS;
  stash->nfonts = 0;

  // Create texture for the cache.
  stash->itw = 1.0f/stash->params.width;
  stash->ith = 1.0f/stash->params.height;
  stash->texData = (IUA*)fons__alloc(stash->params.width * stash->params.height);
  if (stash->texData == nullptr) goto error;
  _::ArrayFill(stash->texData, stash->params.width * stash->params.height);

  stash->dirtyRect[0] = stash->params.width;
  stash->dirtyRect[1] = stash->params.height;
  stash->dirtyRect[2] = 0;
  stash->dirtyRect[3] = 0;

  // Add white rect at 0,0 for debug drawing.
  fons__addWhiteRect(stash, 2,2);

  fonsPushState(stash);
  fonsClearState(stash);

  return stash;

error:
  fonsDeleteInternal(stash);
  return nullptr;
}

FONSstate* fons__getState(FONScontext* stash)
{
  return &stash->states[stash->nstates-1];
}

ISC fonsAddFallbackFont(FONScontext* stash, ISC base, ISC fallback)
{
  FONSfont* baseFont = stash->fonts[base];
  if (baseFont->nfallbacks < FONS_MAX_FALLBACKS) {
    baseFont->fallbacks[baseFont->nfallbacks++] = fallback;
    return 1;
  }
  return 0;
}

void fonsResetFallbackFont(FONScontext* stash, ISC base)
{
  ISC i;

  FONSfont* baseFont = stash->fonts[base];
  baseFont->nfallbacks = 0;
  baseFont->nglyphs = 0;
  for (i = 0; i < FONS_HASH_LUT_SIZE; i++)
    baseFont->lut[i] = -1;
}

void fonsSetSize(FONScontext* stash, FPC size)
{
  fons__getState(stash)->size = size;
}

void fonsSetColor(FONScontext* stash, IUC color)
{
  fons__getState(stash)->color = color;
}

void fonsSetSpacing(FONScontext* stash, FPC spacing)
{
  fons__getState(stash)->spacing = spacing;
}

void fonsSetBlur(FONScontext* stash, FPC blur)
{
  fons__getState(stash)->blur = blur;
}

void fonsSetAlign(FONScontext* stash, ISC align)
{
  fons__getState(stash)->align = align;
}

void fonsSetFont(FONScontext* stash, ISC font)
{
  fons__getState(stash)->font = font;
}

void fonsPushState(FONScontext* stash)
{
  if (stash->nstates >= FONS_MAX_STATES) {
    if (stash->handleError)
      stash->handleError(stash->errorUptr, FONS_STATES_OVERFLOW, 0);
    return;
  }
  if (stash->nstates > 0)
    _::ArrayCopy(&stash->states[stash->nstates], sizeof(FONSstate), &stash->states[stash->nstates-1], sizeof(FONSstate));
  stash->nstates++;
}

void fonsPopState(FONScontext* stash)
{
  if (stash->nstates <= 1) {
    if (stash->handleError)
      stash->handleError(stash->errorUptr, FONS_STATES_UNDERFLOW, 0);
    return;
  }
  stash->nstates--;
}

void fonsClearState(FONScontext* stash)
{
  FONSstate* state = fons__getState(stash);
  state->size = 12.0f;
  state->color = 0xffffffff;
  state->font = 0;
  state->blur = 0;
  state->spacing = 0;
  state->align = FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE;
}

void fons__freeFont(FONSfont* font)
{
  if (font == nullptr) return;
  if (font->glyphs) fons__free(font->glyphs);
  if (font->freeData && font->data) fons__free(font->data);
  fons__free(font);
}

ISC fons__allocFont(FONScontext* stash)
{
  FONSfont* font = nullptr;
  if (stash->nfonts+1 > stash->cfonts) {
    stash->cfonts = stash->cfonts == 0 ? 8 : stash->cfonts * 2;
    stash->fonts = (FONSfont**)fons__realloc(stash->fonts, stash->nfonts * sizeof(FONSfont*), sizeof(FONSfont*) * stash->cfonts);
    if (stash->fonts == nullptr)
      return -1;
  }
  font = (FONSfont*)fons__alloc(sizeof(FONSfont));
  if (font == nullptr) goto error;
  _::ArrayFill(font, sizeof(FONSfont));

  font->glyphs = (FONSglyph*)fons__alloc(sizeof(FONSglyph) * FONS_INIT_GLYPHS);
  if (font->glyphs == nullptr) goto error;
  font->cglyphs = FONS_INIT_GLYPHS;
  font->nglyphs = 0;

  stash->fonts[stash->nfonts++] = font;
  return stash->nfonts-1;

error:
  fons__freeFont(font);

  return FONS_INVALID;
}

ISC fonsAddFont(FONScontext* stash, const CHR* name, const CHR* path, ISC fontIndex)
{
  FILE* fp = 0;
  ISC dataSize = 0;
  ISW readed;
  IUA* data = nullptr;

  // Read in the font data.
  fp = fopen(path, "rb");
  if (fp == nullptr) goto error;
  fseek(fp,0,SEEK_END);
  dataSize = (ISC)ftell(fp);
  fseek(fp,0,SEEK_SET);
  data = (IUA*)fons__alloc(dataSize);
  if (data == nullptr) goto error;
  readed = fread(data, 1, dataSize, fp);
  fclose(fp);
  fp = 0;
  if (readed != (ISW)dataSize) goto error;

  return fonsAddFontMem(stash, name, data, dataSize, 1, fontIndex);

error:
  if (data) fons__free(data);
  if (fp) fclose(fp);
  return FONS_INVALID;
}

ISC fonsAddFontMem(FONScontext* stash, const CHR* name, IUA* data, ISC dataSize, ISC freeData, ISC fontIndex)
{
  ISC i, ascent, descent, fh, lineGap;
  FONSfont* font;

  ISC idx = fons__allocFont(stash);
  if (idx == FONS_INVALID)
    return FONS_INVALID;

  font = stash->fonts[idx];

  { ISC ncopy = fons__cstrlen(name); if (ncopy > (ISC)sizeof(font->name)) ncopy = (ISC)sizeof(font->name);
    _::ArrayCopy(font->name, sizeof(font->name), name, ncopy); }
  font->name[sizeof(font->name)-1] = '\0';

  // Init hash lookup.
  for (i = 0; i < FONS_HASH_LUT_SIZE; ++i)
    font->lut[i] = -1;

  // Read in the font data.
  font->dataSize = dataSize;
  font->data = data;
  font->freeData = (IUA)freeData;

  // Init font
  stash->nscratch = 0;
  if (!fons__tt_loadFont(stash, &font->font, data, dataSize, fontIndex)) goto error;

  // Store normalized line height. The real line height is got
  // by multiplying the lineh by font size.
  fons__tt_getFontVMetrics( &font->font, &ascent, &descent, &lineGap);
  ascent += lineGap;
  fh = ascent - descent;
  font->ascender = (FPC)ascent / (FPC)fh;
  font->descender = (FPC)descent / (FPC)fh;
  font->lineh = font->ascender - font->descender;

  return idx;

error:
  fons__freeFont(font);
  stash->nfonts--;
  return FONS_INVALID;
}

ISC fonsGetFontByName(FONScontext* s, const CHR* name)
{
  ISC i;
  for (i = 0; i < s->nfonts; i++) {
    if (fons__cstrcmp(s->fonts[i]->name, name) == 0)
      return i;
  }
  return FONS_INVALID;
}


FONSglyph* fons__allocGlyph(FONSfont* font)
{
  if (font->nglyphs+1 > font->cglyphs) {
    font->cglyphs = font->cglyphs == 0 ? 8 : font->cglyphs * 2;
    font->glyphs = (FONSglyph*)fons__realloc(font->glyphs, font->nglyphs * sizeof(FONSglyph), sizeof(FONSglyph) * font->cglyphs);
    if (font->glyphs == nullptr) return nullptr;
  }
  font->nglyphs++;
  return &font->glyphs[font->nglyphs-1];
}


// Based on Exponential blur, Jani Huhtanen, 2006

#define APREC 16
#define ZPREC 7

void fons__blurCols(IUA* dst, ISC w, ISC h, ISC dstStride, ISC alpha)
{
  ISC x, y;
  for (y = 0; y < h; y++) {
    ISC z = 0; // force zero border
    for (x = 1; x < w; x++) {
      z += (alpha * (((ISC)(dst[x]) << ZPREC) - z)) >> APREC;
      dst[x] = (IUA)(z >> ZPREC);
    }
    dst[w-1] = 0; // force zero border
    z = 0;
    for (x = w-2; x >= 0; x--) {
      z += (alpha * (((ISC)(dst[x]) << ZPREC) - z)) >> APREC;
      dst[x] = (IUA)(z >> ZPREC);
    }
    dst[0] = 0; // force zero border
    dst += dstStride;
  }
}

void fons__blurRows(IUA* dst, ISC w, ISC h, ISC dstStride, ISC alpha)
{
  ISC x, y;
  for (x = 0; x < w; x++) {
    ISC z = 0; // force zero border
    for (y = dstStride; y < h*dstStride; y += dstStride) {
      z += (alpha * (((ISC)(dst[y]) << ZPREC) - z)) >> APREC;
      dst[y] = (IUA)(z >> ZPREC);
    }
    dst[(h-1)*dstStride] = 0; // force zero border
    z = 0;
    for (y = (h-2)*dstStride; y >= 0; y -= dstStride) {
      z += (alpha * (((ISC)(dst[y]) << ZPREC) - z)) >> APREC;
      dst[y] = (IUA)(z >> ZPREC);
    }
    dst[0] = 0; // force zero border
    dst++;
  }
}


void fons__blur(FONScontext* stash, IUA* dst, ISC w, ISC h, ISC dstStride, ISC blur)
{
  ISC alpha;
  FPC sigma;
  (void)stash;

  if (blur < 1)
    return;
  // Calculate the alpha such that 90% of the kernel is within the radius. (Kernel extends to infinity)
  sigma = (FPC)blur * 0.57735f; // 1 / sqrt(3)
  alpha = (ISC)((1<<APREC) * (1.0f - expf(-2.3f / (sigma+1.0f))));
  fons__blurRows(dst, w, h, dstStride, alpha);
  fons__blurCols(dst, w, h, dstStride, alpha);
  fons__blurRows(dst, w, h, dstStride, alpha);
  fons__blurCols(dst, w, h, dstStride, alpha);
//  fons__blurrows(dst, w, h, dstStride, alpha);
//  fons__blurcols(dst, w, h, dstStride, alpha);
}

FONSglyph* fons__getGlyph(FONScontext* stash, FONSfont* font, IUC codepoint,
                 ISB isize, ISB iblur, ISC bitmapOption)
{
  ISC i, g, advance, lsb, x0, y0, x1, y1, gw, gh, gx, gy, x, y;
  FPC scale;
  FONSglyph* glyph = nullptr;
  IUC h;
  FPC size = isize/10.0f;
  ISC pad, added;
  IUA* bdst;
  IUA* dst;
  FONSfont* renderFont = font;

  if (isize < 2) return nullptr;
  if (iblur > 20) iblur = 20;
  pad = iblur+2;

  // Reset allocator.
  stash->nscratch = 0;

  // Find code point and size.
  h = fons__hashint(codepoint) & (FONS_HASH_LUT_SIZE-1);
  i = font->lut[h];
  while (i != -1) {
    if (font->glyphs[i].codepoint == codepoint && font->glyphs[i].size == isize && font->glyphs[i].blur == iblur) {
      glyph = &font->glyphs[i];
      if (bitmapOption == FONS_GLYPH_BITMAP_OPTIONAL || (glyph->x0 >= 0 && glyph->y0 >= 0)) {
        return glyph;
      }
      // At this point, glyph exists but the bitmap data is not yet created.
      break;
    }
    i = font->glyphs[i].next;
  }

  // Create a new glyph or rasterize bitmap data for a cached glyph.
  g = fons__tt_getGlyphIndex(&font->font, codepoint);
  // Try to find the glyph in fallback fonts.
  if (g == 0) {
    for (i = 0; i < font->nfallbacks; ++i) {
      FONSfont* fallbackFont = stash->fonts[font->fallbacks[i]];
      ISC fallbackIndex = fons__tt_getGlyphIndex(&fallbackFont->font, codepoint);
      if (fallbackIndex != 0) {
        g = fallbackIndex;
        renderFont = fallbackFont;
        break;
      }
    }
    // It is possible that we did not find a fallback glyph.
    // In that case the glyph index 'g' is 0, and we'll proceed below and cache empty glyph.
  }
  scale = fons__tt_getPixelHeightScale(&renderFont->font, size);
  fons__tt_buildGlyphBitmap(&renderFont->font, g, size, scale, &advance, &lsb, &x0, &y0, &x1, &y1);
  gw = x1-x0 + pad*2;
  gh = y1-y0 + pad*2;

  // Determines the spot to draw glyph in the atlas.
  if (bitmapOption == FONS_GLYPH_BITMAP_REQUIRED) {
    // Find free spot for the rect in the atlas
    added = fons__atlasAddRect(stash->atlas, gw, gh, &gx, &gy);
    if (added == 0 && stash->handleError != nullptr) {
      // Atlas is full, let the user to resize the atlas (or not), and try again.
      stash->handleError(stash->errorUptr, FONS_ATLAS_FULL, 0);
      added = fons__atlasAddRect(stash->atlas, gw, gh, &gx, &gy);
    }
    if (added == 0) return nullptr;
  } else {
    // Negative coordinate indicates there is no bitmap data created.
    gx = -1;
    gy = -1;
  }

  // Init glyph.
  if (glyph == nullptr) {
    glyph = fons__allocGlyph(font);
    glyph->codepoint = codepoint;
    glyph->size = isize;
    glyph->blur = iblur;
    glyph->next = 0;

    // Insert CHR to hash lookup.
    glyph->next = font->lut[h];
    font->lut[h] = font->nglyphs-1;
  }
  glyph->index = g;
  glyph->x0 = (ISB)gx;
  glyph->y0 = (ISB)gy;
  glyph->x1 = (ISB)(glyph->x0+gw);
  glyph->y1 = (ISB)(glyph->y0+gh);
  glyph->xadv = (ISB)(scale * advance * 10.0f);
  glyph->xoff = (ISB)(x0 - pad);
  glyph->yoff = (ISB)(y0 - pad);

  if (bitmapOption == FONS_GLYPH_BITMAP_OPTIONAL) {
    return glyph;
  }

  // Rasterize
  dst = &stash->texData[(glyph->x0+pad) + (glyph->y0+pad) * stash->params.width];
  fons__tt_renderGlyphBitmap(&renderFont->font, dst, gw-pad*2,gh-pad*2, stash->params.width, scale, scale, g);

  // Make sure there is one pixel empty border.
  dst = &stash->texData[glyph->x0 + glyph->y0 * stash->params.width];
  for (y = 0; y < gh; y++) {
    dst[y*stash->params.width] = 0;
    dst[gw-1 + y*stash->params.width] = 0;
  }
  for (x = 0; x < gw; x++) {
    dst[x] = 0;
    dst[x + (gh-1)*stash->params.width] = 0;
  }

  // Debug code to color the glyph background
/*  IUA* fdst = &stash->texData[glyph->x0 + glyph->y0 * stash->params.width];
  for (y = 0; y < gh; y++) {
    for (x = 0; x < gw; x++) {
      ISC a = (ISC)fdst[x+y*stash->params.width] + 20;
      if (a > 255) a = 255;
      fdst[x+y*stash->params.width] = a;
    }
  }*/

  // Blur
  if (iblur > 0) {
    stash->nscratch = 0;
    bdst = &stash->texData[glyph->x0 + glyph->y0 * stash->params.width];
    fons__blur(stash, bdst, gw, gh, stash->params.width, iblur);
  }

  stash->dirtyRect[0] = fons__mini(stash->dirtyRect[0], glyph->x0);
  stash->dirtyRect[1] = fons__mini(stash->dirtyRect[1], glyph->y0);
  stash->dirtyRect[2] = fons__maxi(stash->dirtyRect[2], glyph->x1);
  stash->dirtyRect[3] = fons__maxi(stash->dirtyRect[3], glyph->y1);

  return glyph;
}

void fons__getQuad(FONScontext* stash, FONSfont* font,
               ISC prevGlyphIndex, FONSglyph* glyph,
               FPC scale, FPC spacing, FPC* x, FPC* y, FONSquad* q)
{
  FPC rx,ry,xoff,yoff,x0,y0,x1,y1;

  if (prevGlyphIndex != -1) {
    FPC adv = fons__tt_getGlyphKernAdvance(&font->font, prevGlyphIndex, glyph->index) * scale;
    *x += (ISC)(adv + spacing + 0.5f);
  }

  // Each glyph has 2px border to allow good interpolation,
  // one pixel to prevent leaking, and one to allow good interpolation for rendering.
  // Inset the texture region by one pixel for correct interpolation.
  xoff = (ISB)(glyph->xoff+1);
  yoff = (ISB)(glyph->yoff+1);
  x0 = (FPC)(glyph->x0+1);
  y0 = (FPC)(glyph->y0+1);
  x1 = (FPC)(glyph->x1-1);
  y1 = (FPC)(glyph->y1-1);

  if (stash->params.flags & FONS_ZERO_TOPLEFT) {
    rx = floorf(*x + xoff);
    ry = floorf(*y + yoff);

    q->x0 = rx;
    q->y0 = ry;
    q->x1 = rx + x1 - x0;
    q->y1 = ry + y1 - y0;

    q->s0 = x0 * stash->itw;
    q->t0 = y0 * stash->ith;
    q->s1 = x1 * stash->itw;
    q->t1 = y1 * stash->ith;
  } else {
    rx = floorf(*x + xoff);
    ry = floorf(*y - yoff);

    q->x0 = rx;
    q->y0 = ry;
    q->x1 = rx + x1 - x0;
    q->y1 = ry - y1 + y0;

    q->s0 = x0 * stash->itw;
    q->t0 = y0 * stash->ith;
    q->s1 = x1 * stash->itw;
    q->t1 = y1 * stash->ith;
  }

  *x += (ISC)(glyph->xadv / 10.0f + 0.5f);
}

void fons__flush(FONScontext* stash)
{
  // Flush texture
  if (stash->dirtyRect[0] < stash->dirtyRect[2] && stash->dirtyRect[1] < stash->dirtyRect[3]) {
    if (stash->params.renderUpdate != nullptr)
      stash->params.renderUpdate(stash->params.userPtr, stash->dirtyRect, stash->texData);
    // Reset dirty rect
    stash->dirtyRect[0] = stash->params.width;
    stash->dirtyRect[1] = stash->params.height;
    stash->dirtyRect[2] = 0;
    stash->dirtyRect[3] = 0;
  }

  // Flush triangles
  if (stash->nverts > 0) {
    if (stash->params.renderDraw != nullptr)
      stash->params.renderDraw(stash->params.userPtr, stash->verts, stash->tcoords, stash->colors, stash->nverts);
    stash->nverts = 0;
  }
}

__inline void fons__vertex(FONScontext* stash, FPC x, FPC y, FPC s, FPC t, IUC c)
{
  stash->verts[stash->nverts*2+0] = x;
  stash->verts[stash->nverts*2+1] = y;
  stash->tcoords[stash->nverts*2+0] = s;
  stash->tcoords[stash->nverts*2+1] = t;
  stash->colors[stash->nverts] = c;
  stash->nverts++;
}

FPC fons__getVertAlign(FONScontext* stash, FONSfont* font, ISC align, ISB isize)
{
  if (stash->params.flags & FONS_ZERO_TOPLEFT) {
    if (align & FONS_ALIGN_TOP) {
      return font->ascender * (FPC)isize/10.0f;
    } else if (align & FONS_ALIGN_MIDDLE) {
      return (font->ascender + font->descender) / 2.0f * (FPC)isize/10.0f;
    } else if (align & FONS_ALIGN_BASELINE) {
      return 0.0f;
    } else if (align & FONS_ALIGN_BOTTOM) {
      return font->descender * (FPC)isize/10.0f;
    }
  } else {
    if (align & FONS_ALIGN_TOP) {
      return -font->ascender * (FPC)isize/10.0f;
    } else if (align & FONS_ALIGN_MIDDLE) {
      return -(font->ascender + font->descender) / 2.0f * (FPC)isize/10.0f;
    } else if (align & FONS_ALIGN_BASELINE) {
      return 0.0f;
    } else if (align & FONS_ALIGN_BOTTOM) {
      return -font->descender * (FPC)isize/10.0f;
    }
  }
  return 0.0;
}

FPC fonsDrawText(FONScontext* stash,
           FPC x, FPC y,
           const CHR* str, const CHR* end)
{
  FONSstate* state = fons__getState(stash);
  IUC codepoint;
  IUC utf8state = 0;
  FONSglyph* glyph = nullptr;
  FONSquad q;
  ISC prevGlyphIndex = -1;
  ISB isize = (ISB)(state->size*10.0f);
  ISB iblur = (ISB)state->blur;
  FPC scale;
  FONSfont* font;
  FPC width;

  if (stash == nullptr) return x;
  if (state->font < 0 || state->font >= stash->nfonts) return x;
  font = stash->fonts[state->font];
  if (font->data == nullptr) return x;

  scale = fons__tt_getPixelHeightScale(&font->font, (FPC)isize/10.0f);

  if (end == nullptr)
    end = str + fons__cstrlen(str);

  // Align horizontally
  if (state->align & FONS_ALIGN_LEFT) {
    // empty
  } else if (state->align & FONS_ALIGN_RIGHT) {
    width = fonsTextBounds(stash, x,y, str, end, nullptr);
    x -= width;
  } else if (state->align & FONS_ALIGN_CENTER) {
    width = fonsTextBounds(stash, x,y, str, end, nullptr);
    x -= width * 0.5f;
  }
  // Align vertically.
  y += fons__getVertAlign(stash, font, state->align, isize);

  for (; str != end; ++str) {
    if (fons__decutf8(&utf8state, &codepoint, *(const IUA*)str))
      continue;
    glyph = fons__getGlyph(stash, font, codepoint, isize, iblur, FONS_GLYPH_BITMAP_REQUIRED);
    if (glyph != nullptr) {
      fons__getQuad(stash, font, prevGlyphIndex, glyph, scale, state->spacing, &x, &y, &q);

      if (stash->nverts+6 > FONS_VERTEX_COUNT)
        fons__flush(stash);

      fons__vertex(stash, q.x0, q.y0, q.s0, q.t0, state->color);
      fons__vertex(stash, q.x1, q.y1, q.s1, q.t1, state->color);
      fons__vertex(stash, q.x1, q.y0, q.s1, q.t0, state->color);

      fons__vertex(stash, q.x0, q.y0, q.s0, q.t0, state->color);
      fons__vertex(stash, q.x0, q.y1, q.s0, q.t1, state->color);
      fons__vertex(stash, q.x1, q.y1, q.s1, q.t1, state->color);
    }
    prevGlyphIndex = glyph != nullptr ? glyph->index : -1;
  }
  fons__flush(stash);

  return x;
}

ISC fonsTextIterInit(FONScontext* stash, FONStextIter* iter,
           FPC x, FPC y, const CHR* str, const CHR* end, ISC bitmapOption)
{
  FONSstate* state = fons__getState(stash);
  FPC width;

  _::ArrayFill(iter, sizeof(*iter));

  if (stash == nullptr) return 0;
  if (state->font < 0 || state->font >= stash->nfonts) return 0;
  iter->font = stash->fonts[state->font];
  if (iter->font->data == nullptr) return 0;

  iter->isize = (ISB)(state->size*10.0f);
  iter->iblur = (ISB)state->blur;
  iter->scale = fons__tt_getPixelHeightScale(&iter->font->font, (FPC)iter->isize/10.0f);

  // Align horizontally
  if (state->align & FONS_ALIGN_LEFT) {
    // empty
  } else if (state->align & FONS_ALIGN_RIGHT) {
    width = fonsTextBounds(stash, x,y, str, end, nullptr);
    x -= width;
  } else if (state->align & FONS_ALIGN_CENTER) {
    width = fonsTextBounds(stash, x,y, str, end, nullptr);
    x -= width * 0.5f;
  }
  // Align vertically.
  y += fons__getVertAlign(stash, iter->font, state->align, iter->isize);

  if (end == nullptr)
    end = str + fons__cstrlen(str);

  iter->x = iter->nextx = x;
  iter->y = iter->nexty = y;
  iter->spacing = state->spacing;
  iter->str = str;
  iter->next = str;
  iter->end = end;
  iter->codepoint = 0;
  iter->prevGlyphIndex = -1;
  iter->bitmapOption = bitmapOption;

  return 1;
}

ISC fonsTextIterNext(FONScontext* stash, FONStextIter* iter, FONSquad* quad)
{
  FONSglyph* glyph = nullptr;
  const CHR* str = iter->next;
  iter->str = iter->next;

  if (str == iter->end)
    return 0;

  for (; str != iter->end; str++) {
    if (fons__decutf8(&iter->utf8state, &iter->codepoint, *(const IUA*)str))
      continue;
    str++;
    // Get glyph and quad
    iter->x = iter->nextx;
    iter->y = iter->nexty;
    glyph = fons__getGlyph(stash, iter->font, iter->codepoint, iter->isize, iter->iblur, iter->bitmapOption);
    // If the iterator was initialized with FONS_GLYPH_BITMAP_OPTIONAL, then the UV coordinates of the quad will be invalid.
    if (glyph != nullptr)
      fons__getQuad(stash, iter->font, iter->prevGlyphIndex, glyph, iter->scale, iter->spacing, &iter->nextx, &iter->nexty, quad);
    iter->prevGlyphIndex = glyph != nullptr ? glyph->index : -1;
    break;
  }
  iter->next = str;

  return 1;
}

void fonsDrawDebug(FONScontext* stash, FPC x, FPC y)
{
  ISC i;
  ISC w = stash->params.width;
  ISC h = stash->params.height;
  FPC u = w == 0 ? 0 : (1.0f / w);
  FPC v = h == 0 ? 0 : (1.0f / h);

  if (stash->nverts+6+6 > FONS_VERTEX_COUNT)
    fons__flush(stash);

  // Draw background
  fons__vertex(stash, x+0, y+0, u, v, 0x0fffffff);
  fons__vertex(stash, x+w, y+h, u, v, 0x0fffffff);
  fons__vertex(stash, x+w, y+0, u, v, 0x0fffffff);

  fons__vertex(stash, x+0, y+0, u, v, 0x0fffffff);
  fons__vertex(stash, x+0, y+h, u, v, 0x0fffffff);
  fons__vertex(stash, x+w, y+h, u, v, 0x0fffffff);

  // Draw texture
  fons__vertex(stash, x+0, y+0, 0, 0, 0xffffffff);
  fons__vertex(stash, x+w, y+h, 1, 1, 0xffffffff);
  fons__vertex(stash, x+w, y+0, 1, 0, 0xffffffff);

  fons__vertex(stash, x+0, y+0, 0, 0, 0xffffffff);
  fons__vertex(stash, x+0, y+h, 0, 1, 0xffffffff);
  fons__vertex(stash, x+w, y+h, 1, 1, 0xffffffff);

  // Drawbug draw atlas
  for (i = 0; i < stash->atlas->nnodes; i++) {
    FONSatlasNode* n = &stash->atlas->nodes[i];

    if (stash->nverts+6 > FONS_VERTEX_COUNT)
      fons__flush(stash);

    fons__vertex(stash, x+n->x+0, y+n->y+0, u, v, 0xc00000ff);
    fons__vertex(stash, x+n->x+n->width, y+n->y+1, u, v, 0xc00000ff);
    fons__vertex(stash, x+n->x+n->width, y+n->y+0, u, v, 0xc00000ff);

    fons__vertex(stash, x+n->x+0, y+n->y+0, u, v, 0xc00000ff);
    fons__vertex(stash, x+n->x+0, y+n->y+1, u, v, 0xc00000ff);
    fons__vertex(stash, x+n->x+n->width, y+n->y+1, u, v, 0xc00000ff);
  }

  fons__flush(stash);
}

FPC fonsTextBounds(FONScontext* stash,
           FPC x, FPC y,
           const CHR* str, const CHR* end,
           FPC* bounds)
{
  FONSstate* state = fons__getState(stash);
  IUC codepoint;
  IUC utf8state = 0;
  FONSquad q;
  FONSglyph* glyph = nullptr;
  ISC prevGlyphIndex = -1;
  ISB isize = (ISB)(state->size*10.0f);
  ISB iblur = (ISB)state->blur;
  FPC scale;
  FONSfont* font;
  FPC startx, advance;
  FPC minx, miny, maxx, maxy;

  if (stash == nullptr) return 0;
  if (state->font < 0 || state->font >= stash->nfonts) return 0;
  font = stash->fonts[state->font];
  if (font->data == nullptr) return 0;

  scale = fons__tt_getPixelHeightScale(&font->font, (FPC)isize/10.0f);

  // Align vertically.
  y += fons__getVertAlign(stash, font, state->align, isize);

  minx = maxx = x;
  miny = maxy = y;
  startx = x;

  if (end == nullptr)
    end = str + fons__cstrlen(str);

  for (; str != end; ++str) {
    if (fons__decutf8(&utf8state, &codepoint, *(const IUA*)str))
      continue;
    glyph = fons__getGlyph(stash, font, codepoint, isize, iblur, FONS_GLYPH_BITMAP_OPTIONAL);
    if (glyph != nullptr) {
      fons__getQuad(stash, font, prevGlyphIndex, glyph, scale, state->spacing, &x, &y, &q);
      if (q.x0 < minx) minx = q.x0;
      if (q.x1 > maxx) maxx = q.x1;
      if (stash->params.flags & FONS_ZERO_TOPLEFT) {
        if (q.y0 < miny) miny = q.y0;
        if (q.y1 > maxy) maxy = q.y1;
      } else {
        if (q.y1 < miny) miny = q.y1;
        if (q.y0 > maxy) maxy = q.y0;
      }
    }
    prevGlyphIndex = glyph != nullptr ? glyph->index : -1;
  }

  advance = x - startx;

  // Align horizontally
  if (state->align & FONS_ALIGN_LEFT) {
    // empty
  } else if (state->align & FONS_ALIGN_RIGHT) {
    minx -= advance;
    maxx -= advance;
  } else if (state->align & FONS_ALIGN_CENTER) {
    minx -= advance * 0.5f;
    maxx -= advance * 0.5f;
  }

  if (bounds) {
    bounds[0] = minx;
    bounds[1] = miny;
    bounds[2] = maxx;
    bounds[3] = maxy;
  }

  return advance;
}

void fonsVertMetrics(FONScontext* stash,
           FPC* ascender, FPC* descender, FPC* lineh)
{
  FONSfont* font;
  FONSstate* state = fons__getState(stash);
  ISB isize;

  if (stash == nullptr) return;
  if (state->font < 0 || state->font >= stash->nfonts) return;
  font = stash->fonts[state->font];
  isize = (ISB)(state->size*10.0f);
  if (font->data == nullptr) return;

  if (ascender)
    *ascender = font->ascender*isize/10.0f;
  if (descender)
    *descender = font->descender*isize/10.0f;
  if (lineh)
    *lineh = font->lineh*isize/10.0f;
}

void fonsLineBounds(FONScontext* stash, FPC y, FPC* miny, FPC* maxy)
{
  FONSfont* font;
  FONSstate* state = fons__getState(stash);
  ISB isize;

  if (stash == nullptr) return;
  if (state->font < 0 || state->font >= stash->nfonts) return;
  font = stash->fonts[state->font];
  isize = (ISB)(state->size*10.0f);
  if (font->data == nullptr) return;

  y += fons__getVertAlign(stash, font, state->align, isize);

  if (stash->params.flags & FONS_ZERO_TOPLEFT) {
    *miny = y - font->ascender * (FPC)isize/10.0f;
    *maxy = *miny + font->lineh*isize/10.0f;
  } else {
    *maxy = y + font->descender * (FPC)isize/10.0f;
    *miny = *maxy - font->lineh*isize/10.0f;
  }
}

const IUA* fonsGetTextureData(FONScontext* stash, ISC* width, ISC* height)
{
  if (width != nullptr)
    *width = stash->params.width;
  if (height != nullptr)
    *height = stash->params.height;
  return stash->texData;
}

ISC fonsValidateTexture(FONScontext* stash, ISC* dirty)
{
  if (stash->dirtyRect[0] < stash->dirtyRect[2] && stash->dirtyRect[1] < stash->dirtyRect[3]) {
    dirty[0] = stash->dirtyRect[0];
    dirty[1] = stash->dirtyRect[1];
    dirty[2] = stash->dirtyRect[2];
    dirty[3] = stash->dirtyRect[3];
    // Reset dirty rect
    stash->dirtyRect[0] = stash->params.width;
    stash->dirtyRect[1] = stash->params.height;
    stash->dirtyRect[2] = 0;
    stash->dirtyRect[3] = 0;
    return 1;
  }
  return 0;
}

void fonsDeleteInternal(FONScontext* stash)
{
  ISC i;
  if (stash == nullptr) return;

  if (stash->params.renderDelete)
    stash->params.renderDelete(stash->params.userPtr);

  for (i = 0; i < stash->nfonts; ++i)
    fons__freeFont(stash->fonts[i]);

  if (stash->atlas) fons__deleteAtlas(stash->atlas);
  if (stash->fonts) fons__free(stash->fonts);
  if (stash->texData) fons__free(stash->texData);
  if (stash->scratch) fons__free(stash->scratch);
  fons__tt_done(stash);
  fons__free(stash);
}

void fonsSetErrorCallback(FONScontext* stash, void (*callback)(void* uptr, ISC error, ISC val), void* uptr)
{
  if (stash == nullptr) return;
  stash->handleError = callback;
  stash->errorUptr = uptr;
}

void fonsGetAtlasSize(FONScontext* stash, ISC* width, ISC* height)
{
  if (stash == nullptr) return;
  *width = stash->params.width;
  *height = stash->params.height;
}

ISC fonsExpandAtlas(FONScontext* stash, ISC width, ISC height)
{
  ISC i, maxy = 0;
  IUA* data = nullptr;
  if (stash == nullptr) return 0;

  width = fons__maxi(width, stash->params.width);
  height = fons__maxi(height, stash->params.height);

  if (width == stash->params.width && height == stash->params.height)
    return 1;

  // Flush pending glyphs.
  fons__flush(stash);

  // Create new texture
  if (stash->params.renderResize != nullptr) {
    if (stash->params.renderResize(stash->params.userPtr, width, height) == 0)
      return 0;
  }
  // Copy old texture data over.
  data = (IUA*)fons__alloc(width * height);
  if (data == nullptr)
    return 0;
  for (i = 0; i < stash->params.height; i++) {
    IUA* dst = &data[i*width];
    IUA* src = &stash->texData[i*stash->params.width];
    _::ArrayCopy(dst, stash->params.width, src, stash->params.width);
    if (width > stash->params.width)
      _::ArrayFill(dst+stash->params.width, width - stash->params.width);
  }
  if (height > stash->params.height)
    _::ArrayFill(&data[stash->params.height * width], (height - stash->params.height) * width);

  fons__free(stash->texData);
  stash->texData = data;

  // Increase atlas size
  fons__atlasExpand(stash->atlas, width, height);

  // Add existing data as dirty.
  for (i = 0; i < stash->atlas->nnodes; i++)
    maxy = fons__maxi(maxy, stash->atlas->nodes[i].y);
  stash->dirtyRect[0] = 0;
  stash->dirtyRect[1] = 0;
  stash->dirtyRect[2] = stash->params.width;
  stash->dirtyRect[3] = maxy;

  stash->params.width = width;
  stash->params.height = height;
  stash->itw = 1.0f/stash->params.width;
  stash->ith = 1.0f/stash->params.height;

  return 1;
}

ISC fonsResetAtlas(FONScontext* stash, ISC width, ISC height)
{
  ISC i, j;
  if (stash == nullptr) return 0;

  // Flush pending glyphs.
  fons__flush(stash);

  // Create new texture
  if (stash->params.renderResize != nullptr) {
    if (stash->params.renderResize(stash->params.userPtr, width, height) == 0)
      return 0;
  }

  // Reset atlas
  fons__atlasReset(stash->atlas, width, height);

  // Clear texture data.
  stash->texData = (IUA*)fons__realloc(stash->texData, 0, width * height);
  if (stash->texData == nullptr) return 0;
  _::ArrayFill(stash->texData, width * height);

  // Reset dirty rect
  stash->dirtyRect[0] = width;
  stash->dirtyRect[1] = height;
  stash->dirtyRect[2] = 0;
  stash->dirtyRect[3] = 0;

  // Reset cached glyphs
  for (i = 0; i < stash->nfonts; i++) {
    FONSfont* font = stash->fonts[i];
    font->nglyphs = 0;
    for (j = 0; j < FONS_HASH_LUT_SIZE; j++)
      font->lut[j] = -1;
  }

  stash->params.width = width;
  stash->params.height = height;
  stash->itw = 1.0f/stash->params.width;
  stash->ith = 1.0f/stash->params.height;

  // Add white rect at 0,0 for debug drawing.
  fons__addWhiteRect(stash, 2,2);

  return 1;
}


#endif
