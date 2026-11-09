// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKExoVGH
#define _CrabsTKExoVGH
#include <ASCIICrabs/_ConfigHeader.h>
#include <ASCIICrabs/_ConfigDefault.h>
#include <ASCIICrabs/_ConfigFooter.h>
#include <ASCIICrabs/Binary.hpp>

namespace CTK {
namespace Exo {

/* Opaque handle, defined in VG.hxx. The struct tag is the type; no
typedef is needed in C++ (the typedef struct form was removed). */
struct RenderContext;

#define _PiFPC 3.14159265358979323846264338327f

struct ColorFPC {
  union {
    FPC rgba[4];
    struct {
      FPC r,g,b,a;
    };
  };
};

struct VectorPaint {
  FPC xform[6];
  FPC extent[2];
  FPC radius;
  FPC feather;
  ColorFPC inner_color;
  ColorFPC outer_color;
  ISC image;
};

enum Winding {
  CCW = 1,
  CW = 2,
};

enum Solidity {
  IsSolid = 1, 
  IsHole = 2,
};

enum LineCaps {
  LCButt,
  LCRound,
  LCSquare,
  LCBevel,
  LCMiter,
};

enum Alignments {
  AlignLeft     = 1 << 0,
  AlignCenter   = 1 << 1,
  AlignRight    = 1 << 2,
  AlignTop      = 1 << 3,
  AlignMiddle   = 1 << 4,
  AlignBottom   = 1 << 5,
  AlignBaseline = 1 << 6,
};

enum BlendFactors {
  BFZero                     = 1 << 0,
  BFOne                      = 1 << 1,
  BFSourceColor              = 1 << 2,
  BFOneMinusSourceColor      = 1 << 3,
  BFDestinationColor         = 1 << 4,
  BFOneMinusDestinationColor = 1 << 5,
  BFSourceAlpha              = 1 << 6,
  BFOneMinusSourceAlpha      = 1 << 7,
  BFDestinationAlpha         = 1 << 8,
  BFOneMinusDestinationAlpha = 1 << 9,
  BFSourceAlphaSaturate      = 1 << 10,
};

enum CompositeOperations {
  COSourceOver,
  COSourceIn,
  COSourceOut,
  COAtop,
  CODestinationOver,
  CODestinationIn,
  CODestinationOut,
  CODestinationAtop,
  COLighter,
  COCopy,
  COXOr,
};

enum ImageFlags {
  ImageGenerateMipMaps = 1<<0,
  ImageRepeatX         = 1<<1,
  ImageRepeatY         = 1<<2,
  ImageFlipY           = 1<<3,
  ImagePremultiplied   = 1<<4,
  ImageNearest         = 1<<5,
};

enum TextureTypes {
  TextureAlpha = 0x01,
  TextureRGBA  = 0x02,
};

struct CompositeOperationState {
  ISC source_rgb;
  ISC destination_rgb;
  ISC source_alpha;
  ISC destination_alpha;
};

struct GlyphPosition {
  const CHR* str;
  FPC x;
  FPC min_x, max_x;
};

struct TextRow {
  const CHR* start;
  const CHR* end;
  const CHR* next;
  FPC width;
  FPC min_x, max_x;
};

void ExoBeginFrame(RenderContext* context, FPC windowWidth, FPC windowHeight, FPC devicePixelRatio);

void ExoCancelFrame(RenderContext* context);

void ExoEndFrame(RenderContext* context);

void ExoGlobalCompositeOperation(RenderContext* context, ISC op);

void ExoGlobalCompositeBlendFunc(RenderContext* context, ISC sfactor, ISC dfactor);

void ExoGlobalCompositeBlendFuncSeparate(RenderContext* context, ISC srcRGB, ISC dstRGB, ISC srcAlpha, ISC dstAlpha);

ColorFPC ExoRGB(IUA r, IUA g, IUA b);

ColorFPC ExoRGBf(FPC r, FPC g, FPC b);

ColorFPC ExoRGBA(IUA r, IUA g, IUA b, IUA a);

ColorFPC ExoRGBAf(FPC r, FPC g, FPC b, FPC a);

ColorFPC ExoLerpRGBA(ColorFPC c0, ColorFPC c1, FPC u);

ColorFPC ExoTransRGBA(ColorFPC c0, IUA a);

ColorFPC ExoTransRGBAf(ColorFPC c0, FPC a);

ColorFPC ExoHSL(FPC h, FPC s, FPC l);

ColorFPC ExoHSLA(FPC h, FPC s, FPC l, IUA a);

void ExoSave(RenderContext* context);

void ExoRestore(RenderContext* context);

void ExoReset(RenderContext* context);

void ExoShapeAntiAlias(RenderContext* context, ISC enabled);

void ExoStrokeColor(RenderContext* context, ColorFPC color);

void ExoStrokePaint(RenderContext* context, VectorPaint paint);

void ExoFillColor(RenderContext* context, ColorFPC color);

void ExoFillPaint(RenderContext* context, VectorPaint paint);

void ExoMiterLimit(RenderContext* context, FPC limit);

void ExoStrokeWidth(RenderContext* context, FPC size);

void ExoLineCap(RenderContext* context, ISC cap);

void ExoLineJoin(RenderContext* context, ISC join);

void ExoGlobalAlpha(RenderContext* context, FPC alpha);

void ExoResetTransform(RenderContext* context);

//   [a c e]
//   [b d f]
//   [0 0 1]
void ExoTransform(RenderContext* context, FPC a, FPC b, FPC c, FPC d, FPC e, FPC f);

void ExoTranslate(RenderContext* context, FPC x, FPC y);

void ExoRotate(RenderContext* context, FPC radians);

void ExoSkewX(RenderContext* context, FPC radians);

void ExoSkewY(RenderContext* context, FPC radians);

// Scales the current coordinate system.
void ExoScale(RenderContext* context, FPC x, FPC y);

//   [a c e]
//   [b d f]
//   [0 0 1]
// There should be space for 6 floats in the return buffer for the values a-f.
void ExoCurrentTransform(RenderContext* context, FPC* xform);

void ExoTransformIdentity(FPC* dst);

void ExoTransformTranslate(FPC* dst, FPC tx, FPC ty);

void ExoTransformScale(FPC* dst, FPC sx, FPC sy);

void ExoTransformRotate(FPC* dst, FPC a);

void ExoTransformSkewX(FPC* dst, FPC a);

void ExoTransformSkewY(FPC* dst, FPC a);

void ExoTransformMultiply(FPC* dst, const FPC* src);

void ExoTransformPremultiply(FPC* dst, const FPC* src);

ISC ExoTransformInverse(FPC* dst, const FPC* src);

void ExoTransformPoint(FPC* dstx, FPC* dsty, const FPC* xform, FPC srcx, FPC srcy);

FPC ExoDegreeToRadian(FPC deg);
FPC ExoRadianToDegree(FPC rad);



ISC ExoCreateImageRGBA(RenderContext* context, ISC w, ISC h, ISC image_flags, 
  const IUA* data);

void ExoUpdateImage(RenderContext* context, ISC image, const IUA* data);

void ExoImageSize(RenderContext* context, ISC image, ISC* w, ISC* h);

void ExoDeleteImage(RenderContext* context, ISC image);

VectorPaint ExoLinearGradient(RenderContext* context, FPC sx, FPC sy, FPC ex, FPC ey,
               ColorFPC icol, ColorFPC ocol);

VectorPaint ExoBoxGradient(RenderContext* context, FPC x, FPC y, FPC w, FPC h,
            FPC r, FPC f, ColorFPC icol, ColorFPC ocol);

VectorPaint ExoRadialGradient(RenderContext* context, FPC cx, FPC cy, FPC inr, FPC outr,
               ColorFPC icol, ColorFPC ocol);

VectorPaint ExoImagePattern(RenderContext* context, FPC ox, FPC oy, FPC ex, FPC ey,
             FPC radians, ISC image, FPC alpha);

void ExoScissor(RenderContext* context, FPC x, FPC y, FPC w, FPC h);

void ExoIntersectScissor(RenderContext* context, FPC x, FPC y, FPC w, FPC h);

void ExoResetScissor(RenderContext* context);

void ExoBeginPath(RenderContext* context);

void ExoMoveTo(RenderContext* context, FPC x, FPC y);

void ExoLineTo(RenderContext* context, FPC x, FPC y);

void ExoBezierTo(RenderContext* context, FPC c1x, FPC c1y, FPC c2x, FPC c2y, FPC x, FPC y);

void ExoQuadTo(RenderContext* context, FPC cx, FPC cy, FPC x, FPC y);

void ExoArcTo(RenderContext* context, FPC x1, FPC y1, FPC x2, FPC y2, FPC radius);

void ExoClosePath(RenderContext* context);

void ExoPathWinding(RenderContext* context, ISC dir);

void ExoArc(RenderContext* context, FPC cx, FPC cy, FPC r, FPC a0, FPC a1, ISC dir);

void ExoRect(RenderContext* context, FPC x, FPC y, FPC w, FPC h);

void ExoRoundedRect(RenderContext* context, FPC x, FPC y, FPC w, FPC h, FPC r);

void ExoRoundedRectVarying(RenderContext* context, FPC x, FPC y, FPC w, FPC h, FPC radTopLeft, FPC radTopRight, FPC radBottomRight, FPC radBottomLeft);

void ExoEllipse(RenderContext* context, FPC cx, FPC cy, FPC rx, FPC ry);

void ExoCircle(RenderContext* context, FPC cx, FPC cy, FPC r);

void ExoFill(RenderContext* context);

void ExoStroke(RenderContext* context);

ISC ExoCreateFont(RenderContext* context, const CHR* name, const CHR* filename);

ISC ExoCreateFontAtIndex(RenderContext* context, const CHR* name, const CHR* filename, const ISC fontIndex);

ISC ExoCreateFontMem(RenderContext* context, const CHR* name, IUA* data, ISC ndata, ISC freeData);

ISC ExoCreateFontMemAtIndex(RenderContext* context, const CHR* name, IUA* data, ISC ndata, ISC freeData, const ISC fontIndex);

ISC ExoFindFont(RenderContext* context, const CHR* name);

ISC ExoAddFallbackFontId(RenderContext* context, ISC baseFont, ISC fallbackFont);

ISC ExoAddFallbackFont(RenderContext* context, const CHR* baseFont, const CHR* fallbackFont);

void ExoResetFallbackFontsId(RenderContext* context, ISC baseFont);

void ExoResetFallbackFonts(RenderContext* context, const CHR* baseFont);

void ExoFontSize(RenderContext* context, FPC size);

void ExoFontBlur(RenderContext* context, FPC blur);

void ExoTextLetterSpacing(RenderContext* context, FPC spacing);

void ExoTextLineHeight(RenderContext* context, FPC lineHeight);

void ExoTextAlign(RenderContext* context, ISC align);

void ExoFontFaceId(RenderContext* context, ISC font);

void ExoFontFace(RenderContext* context, const CHR* font);

FPC ExoText(RenderContext* context, FPC x, FPC y, const CHR* string, const CHR* end);

void ExoTextBox(RenderContext* context, FPC x, FPC y, FPC breakRowWidth, const CHR* string, const CHR* end);

FPC ExoTextBounds(RenderContext* context, FPC x, FPC y, const CHR* string, const CHR* end, FPC* bounds);

void ExoTextBoxBounds(RenderContext* context, FPC x, FPC y, FPC breakRowWidth, const CHR* string, const CHR* end, FPC* bounds);

ISC ExoTextGlyphPositions(RenderContext* context, FPC x, FPC y, const CHR* string, const CHR* end, GlyphPosition* positions, ISC maxPositions);

void ExoTextMetrics(RenderContext* context, FPC* ascender, FPC* descender, FPC* lineh);

ISC ExoTextBreakLines(RenderContext* context, const CHR* string, const CHR* end, FPC breakRowWidth, TextRow* rows, ISC maxRows);

struct Scissor {
  FPC xform[6];
  FPC extent[2];
};

struct VertexFPC {
  FPC x,y,u,v;
};

struct RenderPath {
  ISC first;
  ISC count;
  CHR closed;
  ISC nbevel;
  VertexFPC* fill;
  ISC nfill;
  VertexFPC* stroke;
  ISC nstroke;
  ISC winding;
  ISC convex;
};

struct RenderParams {

  void* userPtr;

  ISC edgeAntiAlias;

  ISC (*renderCreate)(void* uptr);

  ISC (*renderCreateTexture)(void* uptr, ISC type, ISC w, ISC h, ISC image_flags, const IUA* data);

  ISC (*renderDeleteTexture)(void* uptr, ISC image);

  ISC (*renderUpdateTexture)(void* uptr, ISC image, ISC x, ISC y, ISC w, ISC h, const IUA* data);

  ISC (*renderGetTextureSize)(void* uptr, ISC image, ISC* w, ISC* h);

  void (*renderViewport)(void* uptr, FPC width, FPC height, FPC devicePixelRatio);

  void (*renderCancel)(void* uptr);

  void (*renderFlush)(void* uptr);

  void (*renderFill)(void* uptr, VectorPaint* paint, CompositeOperationState compositeOperation, Scissor* scissor, FPC fringe, const FPC* bounds, const RenderPath* paths, ISC npaths);

  void (*renderStroke)(void* uptr, VectorPaint* paint, CompositeOperationState compositeOperation, Scissor* scissor, FPC fringe, FPC strokeWidth, const RenderPath* paths, ISC npaths);

  void (*renderTriangles)(void* uptr, VectorPaint* paint, CompositeOperationState compositeOperation, Scissor* scissor, const VertexFPC* verts, ISC nverts, FPC fringe);

  void (*renderDelete)(void* uptr);
};

RenderContext* ExoCreateInternal(RenderParams* params);
void ExoDeleteInternal(RenderContext* context);

RenderParams* ExoInternalParams(RenderContext* context);

#define _NotUsed(v) for (;;) { (void)(1 ? (void)0 : ( (void)(v) ) ); break; }

}  //< namespace Exo
}  //< namespace CTK
#endif  // _CrabsTKExoVGH
