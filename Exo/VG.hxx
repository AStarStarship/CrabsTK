#pragma once
#include "VG.h"
#include <ASCIICrabs/Math.hxx>
#include <ASCIICrabs/Array.hxx>
#include <ASCIICrabs/Room.hpp>
#include "FontStash.hxx"
namespace CTK {
namespace Exo {

#define _CommandsSizeInit  256
#define _PointsSizeInit    128
#define _PathsSizeInit     16
#define _VertexsSizeInit   256
#ifndef RenderStateCount
#define RenderStateCount   32
#endif
#define _Kappa90 0.5522847493f  
#define _CountOf(arr) (sizeof(arr) / sizeof(0[arr]))
enum RenderCommands {
  RCMoveTo = 0,
  RCLineTo = 1,
  RCBiezerTo = 2,
  RCClose = 3,
  RCWinding = 4,
};
enum PointFlags {
  PFPointCorner = 0x01,
  PFPointLeft = 0x02,
  PFPointBevel = 0x04,
  PFPointInterBevel = 0x08,
};

struct RenderState {
  CompositeOperationState composite_operation;
  ISC shape_antialias;
  VectorPaint fill;
  VectorPaint stroke;
  FPC stroke_width;
  FPC miter_limit;
  ISC line_join;
  ISC line_cap;
  FPC alpha;
  FPC xform[6];
  Scissor scissor;
  FPC font_size;
  FPC letter_spacing;
  FPC line_height;
  FPC font_blur;
  ISC text_align;
  ISC font_id;
};

struct Point2DFPC {
  FPC x,y;
  FPC dx, dy;
  FPC len;
  FPC dmx, dmy;
  CHR flags;
};

struct PathCache {
  Point2DFPC* points;
  ISC npoints;
  ISC cpoints;
  RenderPath* paths;
  ISC npaths;
  ISC cpaths;
  VertexFPC* verts;
  ISC nverts;
  ISC cverts;
  FPC bounds[4];
};

/* One block type for everything the renderer carves out of the Wall: an IUD
aligned payload covers the context, the command floats, the path tables, the
vertex scratch and the font cache alike. */
enum : DTB { ExoBlockType = _::_IUD };

/* The List parameter set a Wall is built on: ISS, ISR, DTB. */
typedef _::TList<ISS, ISR, DTB> ExoList;

/* The vertex scratch outlives a single path: RenderPath::fill and
RenderPath::stroke point into it until the frame is drawn. */
struct RenderContext {
  RenderParams params;
  _::TWall<>* wall_;       //< the Room Wall every block below came from.
  FPC* commands;
  ISC ccommands;
  ISC ncommands;
  FPC commandx, commandy;
  RenderState states[RenderStateCount];
  ISC nstates;
  PathCache cache;         //< points, paths and the vertex scratch.
  FPC tess_tol;
  FPC distance_tol;
  FPC fringe_width;
  FPC device_pixel_ratio;
  FontStash* fs;           //< glyph atlas and text layout, in the same Wall.
  ISC font_image;          //< backend texture the atlas is uploaded to.
  ISC atlas_max;           //< largest atlas edge the cache was started with.
  ISC draw_call_count;
  ISC fill_tri_count;
  ISC stroke_tri_count;
  ISC text_tri_count;
};

/* Reserves one block in the Room Wall and hands back its payload.
A Wall hands out whole blocks and never takes one back, so a grow below is an
allocate and a copy and the old block stays dead until the Wall is reset. */
void* Exo__alloc(_::TWall<>* wall, ISW bytes) {
  if (wall == _NilP || bytes <= 0) return _NilP;
  ExoList* root = _::TWallRoot<>(wall);
  if (root == _NilP) return _NilP;
  ISR index = _::TListAlloc<ISS, ISR, DTB>(root, DTB(ExoBlockType),
                                          ISS(bytes));
  if (index < 0) return _NilP;
  return _::TListValue<ISS, ISR, DTB>(root, index);
}

/* Copies into a larger Wall block. Failure leaves the original untouched
and returns nil so callers retain their original pointer and capacity. */
void* Exo__realloc(_::TWall<>* wall, void* origin, ISW old_bytes,
                   ISW new_bytes) {
  if (origin == _NilP) return Exo__alloc(wall, new_bytes);
  if (new_bytes <= old_bytes) return origin;
  void* grown = Exo__alloc(wall, new_bytes);
  if (grown == _NilP) return _NilP;
  _::ArrayCopy(grown, new_bytes, origin, old_bytes);
  return grown;
}

ISC Exo__cstrlen(const CHR* s) {
  ISC n = 0;
  while (s[n] != '\0') ++n;
  return n;
}

FPC Exo__modf(FPC a, FPC b) {
  return a - b * _::Floor(a / b);
}

ISC Min2(ISC a, ISC b) { return a < b ? a : b; }

ISC Max2(ISC a, ISC b) { return a > b ? a : b; }

ISC Clamp(ISC a, ISC mn, ISC mx) { return a < mn ? mn : (a > mx ? mx : a); }

FPC Minf(FPC a, FPC b) { return a < b ? a : b; }

FPC Maxf(FPC a, FPC b) { return a > b ? a : b; }

FPC Absolute(FPC a) { return a >= 0.0f ? a : -a; }

FPC Signf(FPC a) { return a >= 0.0f ? 1.0f : -1.0f; }

FPC Clampf(FPC a, FPC mn, FPC mx) { return a < mn ? mn : (a > mx ? mx : a); }

FPC Cross(FPC dx0, FPC dy0, FPC dx1, FPC dy1) { return dx1*dy0 - dx0*dy1; }

FPC Normalize(FPC *x, FPC* y) {
  FPC d = _::SquareRoot(*x * *x + *y * *y);
  if (d > 1e-6f) {
    FPC id = 1.0f / d;
    *x = *x * id;
    *y = *y * id;
  }
  return d;
}
/* Carves the points, the paths and the vertex scratch out of the Wall at
their configured capacities. Nothing here grows at run time unless the scene
outruns them, and then the grow is one more block. */
BOL Exo__initPathCache(RenderContext* context, RenderConfig config) {
  PathCache* cache = &context->cache;
  _::ArrayFill(cache, sizeof(PathCache));
  cache->points = (Point2DFPC*)Exo__alloc(
      context->wall_, sizeof(Point2DFPC) * config.points);
  if (cache->points == _NilP) return false;
  cache->cpoints = config.points;
  cache->paths = (RenderPath*)Exo__alloc(
      context->wall_, sizeof(RenderPath) * config.paths);
  if (cache->paths == _NilP) return false;
  cache->cpaths = config.paths;
  cache->verts = (VertexFPC*)Exo__alloc(
      context->wall_, sizeof(VertexFPC) * config.verts);
  if (cache->verts == _NilP) return false;
  cache->cverts = config.verts;
  return true;
}

void Exo__setDevicePixelRatio(RenderContext* context, FPC ratio) {
  context->tess_tol = 0.25f / ratio;
  context->distance_tol = 0.01f / ratio;
  context->fringe_width = 1.0f / ratio;
  context->device_pixel_ratio = ratio;
}

/* Uploads whatever the glyph atlas changed since the last call. Defined with
the rest of the text code; the frame loop needs it before then. */
void Exo__flushTextTexture(RenderContext* context);

CompositeOperationState Exo__compositeOperationState(ISC op){
  ISC sfactor, dfactor;
  if (op == COSourceOver) {
    sfactor = BFOne;
    dfactor = BFOneMinusSourceAlpha;
  }
  else if (op == COSourceIn) {
    sfactor = BFDestinationAlpha;
    dfactor = BFZero;
  }
  else if (op == COSourceOut) {
    sfactor = BFOneMinusDestinationAlpha;
    dfactor = BFZero;
  }
  else if (op == COAtop) {
    sfactor = BFDestinationAlpha;
    dfactor = BFOneMinusSourceAlpha;
  }
  else if (op == CODestinationOver) {
    sfactor = BFOneMinusDestinationAlpha;
    dfactor = BFOne;
  }
  else if (op == CODestinationIn) {
    sfactor = BFZero;
    dfactor = BFSourceAlpha;
  }
  else if (op == CODestinationOut) {
    sfactor = BFZero;
    dfactor = BFOneMinusSourceAlpha;
  }
  else if (op == CODestinationAtop) {
    sfactor = BFOneMinusDestinationAlpha;
    dfactor = BFSourceAlpha;
  }
  else if (op == COLighter) {
    sfactor = BFOne;
    dfactor = BFOne;
  }
  else if (op == COCopy) {
    sfactor = BFOne;
    dfactor = BFZero;
  }
  else if (op == COXOr) {
    sfactor = BFOneMinusDestinationAlpha;
    dfactor = BFOneMinusSourceAlpha;
  }
  else {
    sfactor = BFOne;
    dfactor = BFZero;
  }
  CompositeOperationState state;
  state.source_rgb = sfactor;
  state.destination_rgb = dfactor;
  state.source_alpha = sfactor;
  state.destination_alpha = dfactor;
  return state;
}

RenderState* Exo__getState(RenderContext* context) {
  return &context->states[context->nstates-1];
}

RenderContext* ExoCreateInternal(RenderParams* params, _::TWall<>* wall,
                                 RenderConfig config) {
  ISC i;
  if (params == _NilP || wall == _NilP) return _NilP;
  RenderContext* context = (RenderContext*)Exo__alloc(wall, sizeof(RenderContext));
  if (context == _NilP) return _NilP;
  _::ArrayFill(context, sizeof(RenderContext));
  context->wall_ = wall;
  context->params = *params;
  context->commands = (FPC*)Exo__alloc(wall, sizeof(FPC) * config.commands);
  if (context->commands == _NilP) goto error;
  context->ncommands = 0;
  context->ccommands = config.commands;
  if (!Exo__initPathCache(context, config)) goto error;
  ExoSave(context);
  ExoReset(context);
  Exo__setDevicePixelRatio(context, 1.0f);
  if (context->params.renderCreate(context->params.userPtr) == 0) goto error;

  /* The atlas block is taken at its largest size so the cache can be reset
  larger later without a second block; the working size starts smaller. */
  context->atlas_max = config.atlas_max;
  context->fs = (FontStash*)Exo__alloc(wall, sizeof(FontStash));
  if (context->fs == _NilP) goto error;
  if (!context->fs->Start(wall, config.atlas_max, config.atlas_max,
                          config.glyphs, config.fonts, true)) goto error;
  if (config.atlas_start < config.atlas_max &&
      !context->fs->ResetAtlas(config.atlas_start, config.atlas_start))
    goto error;
  context->font_image = context->params.renderCreateTexture(
      context->params.userPtr, TextureAlpha, config.atlas_start,
      config.atlas_start, 0, _NilP);
  if (context->font_image == 0) goto error;
  return context;
error:
  ExoDeleteInternal(context);
  return _NilP;
}

RenderParams* ExoInternalParams(RenderContext* context) {
    return &context->params;
}

void ExoDeleteInternal(RenderContext* context) {
  if (context == _NilP) return;
  /* Every block belongs to the Wall, so there is nothing to free: the backend
  and its textures are the only things that own anything outside it. The
  context can still be reached after a failed create, when the Wall may have
  handed out only part of the table. */
  if (context->font_image != 0) {
    ExoDeleteImage(context, context->font_image);
    context->font_image = 0;
  }
  if (context->params.renderDelete != _NilP)
    context->params.renderDelete(context->params.userPtr);
  context->commands = _NilP;
  context->cache.points = _NilP;
  context->cache.paths = _NilP;
  context->cache.verts = _NilP;
  context->fs = _NilP;
  context->wall_ = _NilP;
}

void ExoBeginFrame(RenderContext* context, FPC windowWidth, FPC windowHeight,
  FPC devicePixelRatio) {
  context->nstates = 0;
  ExoSave(context);
  ExoReset(context);
  Exo__setDevicePixelRatio(context, devicePixelRatio);
  context->params.renderViewport(context->params.userPtr, windowWidth, windowHeight, devicePixelRatio);
  context->draw_call_count = 0;
  context->fill_tri_count = 0;
  context->stroke_tri_count = 0;
  context->text_tri_count = 0;
}

void ExoCancelFrame(RenderContext* context) {
  context->params.renderCancel(context->params.userPtr);
}

void ExoEndFrame(RenderContext* context) {
  /* The atlas is uploaded once per frame, after the last text call, so a
  frame that draws ten strings costs one texture update and not ten. */
  Exo__flushTextTexture(context);
  context->params.renderFlush(context->params.userPtr);
}

ColorFPC ExoRGB(IUA r, IUA g, IUA b) {
  return ExoRGBA(r,g,b,255);
}

ColorFPC ExoRGBf(FPC r, FPC g, FPC b) {
  return ExoRGBAf(r,g,b,1.0f);
}

ColorFPC ExoRGBA(IUA r, IUA g, IUA b, IUA a) {
  ColorFPC color;
  color.r = r / 255.0f;
  color.g = g / 255.0f;
  color.b = b / 255.0f;
  color.a = a / 255.0f;
  return color;
}

ColorFPC ExoRGBAf(FPC r, FPC g, FPC b, FPC a) {
  ColorFPC color;
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = a;
  return color;
}
ColorFPC ExoTransRGBA(ColorFPC c, IUA a){
  c.a = a / 255.0f;
  return c;
}

ColorFPC ExoTransRGBAf(ColorFPC c, FPC a) {
  c.a = a;
  return c;
}

ColorFPC ExoLerpRGBA(ColorFPC c0, ColorFPC c1, FPC u) {
  ISC i;
  FPC oneminu;
  ColorFPC cint = {{{0}}};
  u = Clampf(u, 0.0f, 1.0f);
  oneminu = 1.0f - u;
  for( i = 0; i <4; i++ )
  {
    cint.rgba[i] = c0.rgba[i] * oneminu + c1.rgba[i] * u;
  }
  return cint;
}

ColorFPC ExoHSL(FPC h, FPC s, FPC l) {
  return ExoHSLA(h,s,l,IUA(255));
}

FPC Exo__hue(FPC h, FPC m1, FPC m2) {
  if (h < 0) h += 1;
  if (h > 1) h -= 1;
  if (h < 1.0f/6.0f)
    return m1 + (m2 - m1) * h * 6.0f;
  else if (h < 3.0f/6.0f)
    return m2;
  else if (h < 4.0f/6.0f)
    return m1 + (m2 - m1) * (2.0f/3.0f - h) * 6.0f;
  return m1;
}

ColorFPC ExoHSLA(FPC h, FPC s, FPC l, IUA a) {
  FPC m1, m2;
  ColorFPC col;
  h = Exo__modf(h, 1.0f);
  if (h < 0.0f) h += 1.0f;
  s = Clampf(s, 0.0f, 1.0f);
  l = Clampf(l, 0.0f, 1.0f);
  m2 = l <= 0.5f ? (l * (1 + s)) : (l + s - l * s);
  m1 = 2 * l - m2;
  col.r = Clampf(Exo__hue(h + 1.0f/3.0f, m1, m2), 0.0f, 1.0f);
  col.g = Clampf(Exo__hue(h, m1, m2), 0.0f, 1.0f);
  col.b = Clampf(Exo__hue(h - 1.0f/3.0f, m1, m2), 0.0f, 1.0f);
  col.a = a/255.0f;
  return col;
}

void ExoTransformIdentity(FPC* t) {
  t[0] = 1.0f; t[1] = 0.0f;
  t[2] = 0.0f; t[3] = 1.0f;
  t[4] = 0.0f; t[5] = 0.0f;
}

void ExoTransformTranslate(FPC* t, FPC tx, FPC ty) {
  t[0] = 1.0f; t[1] = 0.0f;
  t[2] = 0.0f; t[3] = 1.0f;
  t[4] = tx; t[5] = ty;
}
void ExoTransformScale(FPC* t, FPC sx, FPC sy) {
  t[0] = sx; t[1] = 0.0f;
  t[2] = 0.0f; t[3] = sy;
  t[4] = 0.0f; t[5] = 0.0f;
}

void ExoTransformRotate(FPC* t, FPC a) {
  FPC cs = _::Cosine(a), sn = _::Sine(a);
  t[0] = cs; t[1] = sn;
  t[2] = -sn; t[3] = cs;
  t[4] = 0.0f; t[5] = 0.0f;
}

void ExoTransformSkewX(FPC* t, FPC a) {
  t[0] = 1.0f; t[1] = 0.0f;
  t[2] = _::Tan(a); t[3] = 1.0f;
  t[4] = 0.0f; t[5] = 0.0f;
}

void ExoTransformSkewY(FPC* t, FPC a) {
  t[0] = 1.0f; t[1] = _::Tan(a);
  t[2] = 0.0f; t[3] = 1.0f;
  t[4] = 0.0f; t[5] = 0.0f;
}

void ExoTransformMultiply(FPC* t, const FPC* s) {
  FPC t0 = t[0] * s[0] + t[1] * s[2];
  FPC t2 = t[2] * s[0] + t[3] * s[2];
  FPC t4 = t[4] * s[0] + t[5] * s[2] + s[4];
  t[1] = t[0] * s[1] + t[1] * s[3];
  t[3] = t[2] * s[1] + t[3] * s[3];
  t[5] = t[4] * s[1] + t[5] * s[3] + s[5];
  t[0] = t0;
  t[2] = t2;
  t[4] = t4;
}

void ExoTransformPremultiply(FPC* t, const FPC* s) {
  FPC s2[6];
  _::ArrayCopy(s2, sizeof(FPC)*6, s, sizeof(FPC)*6);
  ExoTransformMultiply(s2, t);
  _::ArrayCopy(t, sizeof(FPC)*6, s2, sizeof(FPC)*6);
}

ISC ExoTransformInverse(FPC* inv, const FPC* t) {
  FPD invdet, det = FPD(t[0]) * FPD(t[3]) - FPD(t[2]) * FPD(t[1]);
  if (det > -1e-6 && det < 1e-6) {
    ExoTransformIdentity(inv);
    return 0;
  }
  invdet = 1.0 / det;
  inv[0] = FPC(FPD(t[3]) * invdet);
  inv[2] = FPC(-FPD(t[2]) * invdet);
  inv[4] = FPC((FPD(t[2]) * FPD(t[5]) - FPD(t[3]) * FPD(t[4])) * invdet);
  inv[1] = FPC(-FPD(t[1]) * invdet);
  inv[3] = FPC(FPD(t[0]) * invdet);
  inv[5] = FPC((FPD(t[1]) * FPD(t[4]) - FPD(t[0]) * FPD(t[5])) * invdet);
  return 1;
}

void ExoTransformPoint(FPC* dx, FPC* dy, const FPC* t, FPC sx, FPC sy) {
  *dx = sx*t[0] + sy*t[2] + t[4];
  *dy = sx*t[1] + sy*t[3] + t[5];
}

FPC ExoDegreeToRadian(FPC deg) {
  return deg / 180.0f * _PiFPC;
}

FPC ExoRadianToDegree(FPC rad) {
  return rad / _PiFPC * 180.0f;
}

void Exo__setPaintColor(VectorPaint* p, ColorFPC color) {
  _::ArrayFill(p, sizeof(*p));
  ExoTransformIdentity(p->xform);
  p->radius = 0.0f;
  p->feather = 1.0f;
  p->inner_color = color;
  p->outer_color = color;
}

void ExoSave(RenderContext* context) {
  if (context->nstates >= RenderStateCount)
    return;
  if (context->nstates > 0)
    _::ArrayCopy(&context->states[context->nstates], sizeof(RenderState), &context->states[context->nstates-1], sizeof(RenderState));
  context->nstates++;
}

void ExoRestore(RenderContext* context) {
  if (context->nstates <= 1)
    return;
  context->nstates--;
}

void ExoReset(RenderContext* context) {
  RenderState* state = Exo__getState(context);
  _::ArrayFill(state, sizeof(*state));
  Exo__setPaintColor(&state->fill, ExoRGBA(255,255,255,255));
  Exo__setPaintColor(&state->stroke, ExoRGBA(0,0,0,255));
  state->composite_operation = Exo__compositeOperationState(COSourceOver);
  state->shape_antialias = 1;
  state->stroke_width = 1.0f;
  state->miter_limit = 10.0f;
  state->line_cap = LCButt;
  state->line_join = LCMiter;
  state->alpha = 1.0f;
  ExoTransformIdentity(state->xform);
  state->scissor.extent[0] = -1.0f;
  state->scissor.extent[1] = -1.0f;
  state->font_size = 16.0f;
  state->letter_spacing = 0.0f;
  state->line_height = 1.0f;
  state->font_blur = 0.0f;
  state->text_align = AlignLeft | AlignBaseline;
  state->font_id = 0;
}

void ExoShapeAntiAlias(RenderContext* context, ISC enabled) {
  RenderState* state = Exo__getState(context);
  state->shape_antialias = enabled;
}

void ExoStrokeWidth(RenderContext* context, FPC width) {
  RenderState* state = Exo__getState(context);
  state->stroke_width = width;
}

void ExoMiterLimit(RenderContext* context, FPC limit) {
  RenderState* state = Exo__getState(context);
  state->miter_limit = limit;
}

void ExoLineCap(RenderContext* context, ISC cap) {
  RenderState* state = Exo__getState(context);
  state->line_cap = cap;
}

void ExoLineJoin(RenderContext* context, ISC join) {
  RenderState* state = Exo__getState(context);
  state->line_join = join;
}

void ExoGlobalAlpha(RenderContext* context, FPC alpha) {
  RenderState* state = Exo__getState(context);
  state->alpha = alpha;
}
void ExoTransform(RenderContext* context, FPC a, FPC b, FPC c, FPC d, FPC e, FPC f) {
  RenderState* state = Exo__getState(context);
  FPC t[6] = { a, b, c, d, e, f };
  ExoTransformPremultiply(state->xform, t);
}

void ExoResetTransform(RenderContext* context) {
  RenderState* state = Exo__getState(context);
  ExoTransformIdentity(state->xform);
}

void ExoTranslate(RenderContext* context, FPC x, FPC y) {
  RenderState* state = Exo__getState(context);
  FPC t[6];
  ExoTransformTranslate(t, x,y);
  ExoTransformPremultiply(state->xform, t);
}

void ExoRotate(RenderContext* context, FPC radians) {
  RenderState* state = Exo__getState(context);
  FPC t[6];
  ExoTransformRotate(t, radians);
  ExoTransformPremultiply(state->xform, t);
}

void ExoSkewX(RenderContext* context, FPC radians) {
  RenderState* state = Exo__getState(context);
  FPC t[6];
  ExoTransformSkewX(t, radians);
  ExoTransformPremultiply(state->xform, t);
}

void ExoSkewY(RenderContext* context, FPC radians) {
  RenderState* state = Exo__getState(context);
  FPC t[6];
  ExoTransformSkewY(t, radians);
  ExoTransformPremultiply(state->xform, t);
}

void ExoScale(RenderContext* context, FPC x, FPC y) {
  RenderState* state = Exo__getState(context);
  FPC t[6];
  ExoTransformScale(t, x,y);
  ExoTransformPremultiply(state->xform, t);
}

void ExoCurrentTransform(RenderContext* context, FPC* xform) {
  RenderState* state = Exo__getState(context);
  if (xform == nullptr) return;
  _::ArrayCopy(xform, sizeof(FPC)*6, state->xform, sizeof(FPC)*6);
}

void ExoStrokeColor(RenderContext* context, ColorFPC color) {
  RenderState* state = Exo__getState(context);
  Exo__setPaintColor(&state->stroke, color);
}

void ExoStrokePaint(RenderContext* context, VectorPaint paint) {
  RenderState* state = Exo__getState(context);
  state->stroke = paint;
  ExoTransformMultiply(state->stroke.xform, state->xform);
}

void ExoFillColor(RenderContext* context, ColorFPC color) {
  RenderState* state = Exo__getState(context);
  Exo__setPaintColor(&state->fill, color);
}
void ExoFillPaint(RenderContext* context, VectorPaint paint) {
  RenderState* state = Exo__getState(context);
  state->fill = paint;
  ExoTransformMultiply(state->fill.xform, state->xform);
}

ISC ExoCreateImageRGBA(RenderContext* context, ISC w, ISC h, ISC image_flags, 
  const IUA* data) {
  return context->params.renderCreateTexture(context->params.userPtr, 
    TextureRGBA, w, h, image_flags, data);
}

void ExoUpdateImage(RenderContext* context, ISC image, const IUA* data) {
  ISC w, h;
  context->params.renderGetTextureSize(context->params.userPtr, image, &w, &h);
  context->params.renderUpdateTexture(context->params.userPtr, image, 0,0, w,h, data);
}

void ExoImageSize(RenderContext* context, ISC image, ISC* w, ISC* h) {
  context->params.renderGetTextureSize(context->params.userPtr, image, w, h);
}

void ExoDeleteImage(RenderContext* context, ISC image) {
  context->params.renderDeleteTexture(context->params.userPtr, image);
}

VectorPaint ExoLinearGradient(RenderContext* context, FPC sx, FPC sy, FPC ex, 
  FPC ey, ColorFPC icol, ColorFPC ocol) {
  VectorPaint p;
  FPC dx, dy, d;
  const FPC large = 1e5;
  _NotUsed(context);
  _::ArrayFill(&p, sizeof(p));
  dx = ex - sx;
  dy = ey - sy;
  d = _::SquareRoot(dx*dx + dy*dy);
  if (d > 0.0001f) {
    dx /= d;
    dy /= d;
  } else {
    dx = 0;
    dy = 1;
  }
  p.xform[0] = dy; p.xform[1] = -dx;
  p.xform[2] = dx; p.xform[3] = dy;
  p.xform[4] = sx - dx*large; p.xform[5] = sy - dy*large;
  p.extent[0] = large;
  p.extent[1] = large + d*0.5f;
  p.radius = 0.0f;
  p.feather = Maxf(1.0f, d);
  p.inner_color = icol;
  p.outer_color = ocol;
  return p;
}
VectorPaint ExoRadialGradient(RenderContext* context, FPC cx, FPC cy, FPC inr, 
  FPC outr, ColorFPC icol, ColorFPC ocol) {
  VectorPaint p;
  FPC r = (inr+outr)*0.5f;
  FPC f = (outr-inr);
  _NotUsed(context);
  _::ArrayFill(&p, sizeof(p));
  ExoTransformIdentity(p.xform);
  p.xform[4] = cx;
  p.xform[5] = cy;
  p.extent[0] = r;
  p.extent[1] = r;
  p.radius = r;
  p.feather = Maxf(1.0f, f);
  p.inner_color = icol;
  p.outer_color = ocol;
  return p;
}
VectorPaint ExoBoxGradient(RenderContext* context, FPC x, FPC y, FPC w, FPC h, FPC r,
  FPC f, ColorFPC icol, ColorFPC ocol) {
  VectorPaint p;
  _NotUsed(context);
  _::ArrayFill(&p, sizeof(p));
  ExoTransformIdentity(p.xform);
  p.xform[4] = x+w*0.5f;
  p.xform[5] = y+h*0.5f;
  p.extent[0] = w*0.5f;
  p.extent[1] = h*0.5f;
  p.radius = r;
  p.feather = Maxf(1.0f, f);
  p.inner_color = icol;
  p.outer_color = ocol;
  return p;
}

VectorPaint ExoImagePattern(RenderContext* context, FPC cx, FPC cy, FPC w, FPC h, 
  FPC radians, ISC image, FPC alpha) {
  VectorPaint p;
  _NotUsed(context);
  _::ArrayFill(&p, sizeof(p));
  ExoTransformRotate(p.xform, radians);
  p.xform[4] = cx;
  p.xform[5] = cy;
  p.extent[0] = w;
  p.extent[1] = h;
  p.image = image;
  p.inner_color = p.outer_color = ExoRGBAf(1,1,1,alpha);
  return p;
}

void ExoScissor(RenderContext* context, FPC x, FPC y, FPC w, FPC h) {
  RenderState* state = Exo__getState(context);
  w = Maxf(0.0f, w);
  h = Maxf(0.0f, h);
  ExoTransformIdentity(state->scissor.xform);
  state->scissor.xform[4] = x+w*0.5f;
  state->scissor.xform[5] = y+h*0.5f;
  ExoTransformMultiply(state->scissor.xform, state->xform);
  state->scissor.extent[0] = w*0.5f;
  state->scissor.extent[1] = h*0.5f;
}

void Exo__isectRects(FPC* dst, FPC ax, FPC ay, FPC aw, FPC ah, FPC bx, FPC by, 
  FPC bw, FPC bh) {
  FPC min_x = Maxf(ax, bx);
  FPC miny = Maxf(ay, by);
  FPC max_x = Minf(ax+aw, bx+bw);
  FPC maxy = Minf(ay+ah, by+bh);
  dst[0] = min_x;
  dst[1] = miny;
  dst[2] = Maxf(0.0f, max_x - min_x);
  dst[3] = Maxf(0.0f, maxy - miny);
}

void ExoIntersectScissor(RenderContext* context, FPC x, FPC y, FPC w, FPC h) {
  RenderState* state = Exo__getState(context);
  FPC pxform[6], invxorm[6];
  FPC rect[4];
  FPC ex, ey, tex, tey;
  if (state->scissor.extent[0] < 0) {
    ExoScissor(context, x, y, w, h);
    return;
  }
  _::ArrayCopy(pxform, sizeof(FPC)*6, state->scissor.xform, sizeof(FPC)*6);
  ex = state->scissor.extent[0];
  ey = state->scissor.extent[1];
  ExoTransformInverse(invxorm, state->xform);
  ExoTransformMultiply(pxform, invxorm);
  tex = ex*Absolute(pxform[0]) + ey*Absolute(pxform[2]);
  tey = ex*Absolute(pxform[1]) + ey*Absolute(pxform[3]);
  Exo__isectRects(rect, pxform[4]-tex,pxform[5]-tey,tex*2,tey*2, x,y,w,h);
  ExoScissor(context, rect[0], rect[1], rect[2], rect[3]);
}

void ExoResetScissor(RenderContext* context) {
  RenderState* state = Exo__getState(context);
  _::ArrayFill(state->scissor.xform, sizeof(state->scissor.xform));
  state->scissor.extent[0] = -1.0f;
  state->scissor.extent[1] = -1.0f;
}

void ExoGlobalCompositeOperation(RenderContext* context, ISC op) {
  RenderState* state = Exo__getState(context);
  state->composite_operation = Exo__compositeOperationState(op);
}

void ExoGlobalCompositeBlendFunc(RenderContext* context, ISC sfactor, ISC dfactor) {
  ExoGlobalCompositeBlendFuncSeparate(context, sfactor, dfactor, sfactor, dfactor);
}

void ExoGlobalCompositeBlendFuncSeparate(RenderContext* context, ISC source_rgb, 
  ISC destination_rgb, ISC source_alpha, ISC destination_alpha) {
  CompositeOperationState op;
  op.source_rgb = source_rgb;
  op.destination_rgb = destination_rgb;
  op.source_alpha = source_alpha;
  op.destination_alpha = destination_alpha;
  RenderState* state = Exo__getState(context);
  state->composite_operation = op;
}

ISC Exo__ptEquals(FPC x1, FPC y1, FPC x2, FPC y2, FPC tol) {
  FPC dx = x2 - x1;
  FPC dy = y2 - y1;
  return dx*dx + dy*dy < tol*tol;
}

FPC Exo__distPtSeg(FPC x, FPC y, FPC px, FPC py, FPC qx, FPC qy) {
  FPC pqx, pqy, dx, dy, d, t;
  pqx = qx-px;
  pqy = qy-py;
  dx = x-px;
  dy = y-py;
  d = pqx*pqx + pqy*pqy;
  t = pqx*dx + pqy*dy;
  if (d > 0) t /= d;
  if (t < 0) t = 0;
  else if (t > 1) t = 1;
  dx = px + t*pqx - x;
  dy = py + t*pqy - y;
  return dx*dx + dy*dy;
}

void Exo__appendCommands(RenderContext* context, FPC* vals, ISC nvals) {
  RenderState* state = Exo__getState(context);
  ISC i;
  if (context->ncommands+nvals > context->ccommands) {
    FPC* commands;
    ISC ccommands = context->ncommands+nvals + context->ccommands/2;
    commands = (FPC*)Exo__realloc(context->wall_, context->commands,
      sizeof(FPC)*context->ncommands, sizeof(FPC)*ccommands);
    if (commands == nullptr) return;
    context->commands = commands;
    context->ccommands = ccommands;
  }
  if ((ISC)vals[0] != RCClose && (ISC)vals[0] != RCWinding) {
    context->commandx = vals[nvals-2];
    context->commandy = vals[nvals-1];
  }
  i = 0;
  while (i < nvals) {
    ISC cmd = (ISC)vals[i];
    switch (cmd) {
    case RCMoveTo:
      ExoTransformPoint(&vals[i+1],&vals[i+2], state->xform, vals[i+1],vals[i+2]);
      i += 3;
      break;
    case RCLineTo:
      ExoTransformPoint(&vals[i+1],&vals[i+2], state->xform, vals[i+1],vals[i+2]);
      i += 3;
      break;
    case RCBiezerTo:
      ExoTransformPoint(&vals[i+1],&vals[i+2], state->xform, vals[i+1],vals[i+2]);
      ExoTransformPoint(&vals[i+3],&vals[i+4], state->xform, vals[i+3],vals[i+4]);
      ExoTransformPoint(&vals[i+5],&vals[i+6], state->xform, vals[i+5],vals[i+6]);
      i += 7;
      break;
    case RCClose:
      i++;
      break;
    case RCWinding:
      i += 2;
      break;
    default:
      i++;
    }
  }
  _::ArrayCopy(&context->commands[context->ncommands], nvals*sizeof(FPC), vals, nvals*sizeof(FPC));
  context->ncommands += nvals;
}

void Exo__clearPathCache(RenderContext* context) {
  context->cache.npoints = 0;
  context->cache.npaths = 0;
}

RenderPath* Exo__lastPath(RenderContext* context) {
  if (context->cache.npaths > 0)
    return &context->cache.paths[context->cache.npaths-1];
  return nullptr;
}

void Exo__addPath(RenderContext* context) {
  RenderPath* path;
  if (context->cache.npaths+1 > context->cache.cpaths) {
    RenderPath* paths;
    ISC cpaths = context->cache.npaths+1 + context->cache.cpaths/2;
    paths = (RenderPath*)Exo__realloc(context->wall_, context->cache.paths, sizeof(RenderPath)*context->cache.npaths, sizeof(RenderPath)*cpaths);
    if (paths == nullptr) return;
    context->cache.paths = paths;
    context->cache.cpaths = cpaths;
  }
  path = &context->cache.paths[context->cache.npaths];
  _::ArrayFill(path, sizeof(*path));
  path->first = context->cache.npoints;
  path->winding = CCW;
  context->cache.npaths++;
}
Point2DFPC* Exo__lastPoint(RenderContext* context) {
  if (context->cache.npoints > 0)
    return &context->cache.points[context->cache.npoints-1];
  return nullptr;
}
void Exo__addPoint(RenderContext* context, FPC x, FPC y, ISC flags) {
  RenderPath* path = Exo__lastPath(context);
  Point2DFPC* pt;
  if (path == nullptr) return;
  if (path->count > 0 && context->cache.npoints > 0) {
    pt = Exo__lastPoint(context);
    if (Exo__ptEquals(pt->x,pt->y, x,y, context->distance_tol)) {
      pt->flags |= flags;
      return;
    }
  }
  if (context->cache.npoints+1 > context->cache.cpoints) {
    Point2DFPC* points;
    ISC cpoints = context->cache.npoints+1 + context->cache.cpoints/2;
    points = (Point2DFPC*)Exo__realloc(context->wall_, context->cache.points, sizeof(Point2DFPC)*context->cache.npoints, sizeof(Point2DFPC)*cpoints);
    if (points == nullptr) return;
    context->cache.points = points;
    context->cache.cpoints = cpoints;
  }
  pt = &context->cache.points[context->cache.npoints];
  _::ArrayFill(pt, sizeof(*pt));
  pt->x = x;
  pt->y = y;
  pt->flags = (CHR)flags;
  context->cache.npoints++;
  path->count++;
}

void Exo__closePath(RenderContext* context) {
  RenderPath* path = Exo__lastPath(context);
  if (path == nullptr) return;
  path->closed = 1;
}

void Exo__pathWinding(RenderContext* context, ISC winding) {
  RenderPath* path = Exo__lastPath(context);
  if (path == nullptr) return;
  path->winding = winding;
}

FPC Exo__getAverageScale(FPC *t) {
  FPC sx = _::SquareRoot(t[0]*t[0] + t[2]*t[2]);
  FPC sy = _::SquareRoot(t[1]*t[1] + t[3]*t[3]);
  return (sx + sy) * 0.5f;
}

VertexFPC* Exo__allocTempVerts(RenderContext* context, ISC nverts) {
  if (nverts > context->cache.cverts) {
    VertexFPC* verts;
    ISC cverts = (nverts + 0xff) & ~0xff; 
    verts = (VertexFPC*)Exo__realloc(context->wall_, context->cache.verts, 0, sizeof(VertexFPC)*cverts);
    if (verts == nullptr) return nullptr;
    context->cache.verts = verts;
    context->cache.cverts = cverts;
  }
  return context->cache.verts;
}

FPC Exo__triarea2(FPC ax, FPC ay, FPC bx, FPC by, FPC cx, FPC cy) {
  FPC abx = bx - ax;
  FPC aby = by - ay;
  FPC acx = cx - ax;
  FPC acy = cy - ay;
  return acx*aby - abx*acy;
}

FPC Exo__polyArea(Point2DFPC* pts, ISC npts) {
  ISC i;
  FPC area = 0;
  for (i = 2; i < npts; i++) {
    Point2DFPC* a = &pts[0];
    Point2DFPC* b = &pts[i-1];
    Point2DFPC* c = &pts[i];
    area += Exo__triarea2(a->x,a->y, b->x,b->y, c->x,c->y);
  }
  return area * 0.5f;
}

void Exo__polyReverse(Point2DFPC* pts, ISC npts) {
  Point2DFPC tmp;
  ISC i = 0, j = npts-1;
  while (i < j) {
    tmp = pts[i];
    pts[i] = pts[j];
    pts[j] = tmp;
    i++;
    j--;
  }
}

void Exo__vset(VertexFPC* vtx, FPC x, FPC y, FPC u, FPC v) {
  vtx->x = x;
  vtx->y = y;
  vtx->u = u;
  vtx->v = v;
}
 
void Exo__tesselateBezier(RenderContext* context, FPC x1, FPC y1, FPC x2, FPC y2,
  FPC x3, FPC y3, FPC x4, FPC y4, ISC level, ISC type) {
  FPC x12,y12,x23,y23,x34,y34,x123,y123,x234,y234,x1234,y1234;
  FPC dx,dy,d2,d3;
  if (level > 10) return;
  x12 = (x1+x2)*0.5f;
  y12 = (y1+y2)*0.5f;
  x23 = (x2+x3)*0.5f;
  y23 = (y2+y3)*0.5f;
  x34 = (x3+x4)*0.5f;
  y34 = (y3+y4)*0.5f;
  x123 = (x12+x23)*0.5f;
  y123 = (y12+y23)*0.5f;
  dx = x4 - x1;
  dy = y4 - y1;
  d2 = Absolute(((x2 - x4) * dy - (y2 - y4) * dx));
  d3 = Absolute(((x3 - x4) * dy - (y3 - y4) * dx));
  if ((d2 + d3)*(d2 + d3) < context->tess_tol * (dx*dx + dy*dy)) {
    Exo__addPoint(context, x4, y4, type);
    return;
  }
  x234 = (x23+x34)*0.5f;
  y234 = (y23+y34)*0.5f;
  x1234 = (x123+x234)*0.5f;
  y1234 = (y123+y234)*0.5f;
  Exo__tesselateBezier(context, x1,y1, x12,y12, x123,y123, x1234,y1234, level+1, 0);
  Exo__tesselateBezier(context, x1234,y1234, x234,y234, x34,y34, x4,y4, level+1, type);
}

void Exo__flattenPaths(RenderContext* context) {
  PathCache* cache = &context->cache;
  Point2DFPC* last;
  Point2DFPC* p0;
  Point2DFPC* p1;
  Point2DFPC* pts;
  RenderPath* path;
  ISC i, j;
  FPC* cp1;
  FPC* cp2;
  FPC* p;
  FPC area;
  if (cache->npaths > 0)
    return;
  i = 0;
  while (i < context->ncommands) {
    ISC cmd = (ISC)context->commands[i];
    switch (cmd) {
    case RCMoveTo:
      Exo__addPath(context);
      p = &context->commands[i+1];
      Exo__addPoint(context, p[0], p[1], PFPointCorner);
      i += 3;
      break;
    case RCLineTo:
      p = &context->commands[i+1];
      Exo__addPoint(context, p[0], p[1], PFPointCorner);
      i += 3;
      break;
    case RCBiezerTo:
      last = Exo__lastPoint(context);
      if (last != nullptr) {
        cp1 = &context->commands[i+1];
        cp2 = &context->commands[i+3];
        p = &context->commands[i+5];
        Exo__tesselateBezier(context, last->x,last->y, cp1[0],cp1[1], cp2[0],cp2[1], p[0],p[1], 0, PFPointCorner);
      }
      i += 7;
      break;
    case RCClose:
      Exo__closePath(context);
      i++;
      break;
    case RCWinding:
      Exo__pathWinding(context, (ISC)context->commands[i+1]);
      i += 2;
      break;
    default:
      i++;
    }
  }
  cache->bounds[0] = cache->bounds[1] = 1e6f;
  cache->bounds[2] = cache->bounds[3] = -1e6f;
  for (j = 0; j < cache->npaths; j++) {
    path = &cache->paths[j];
    pts = &cache->points[path->first];
    p0 = &pts[path->count-1];
    p1 = &pts[0];
    if (Exo__ptEquals(p0->x,p0->y, p1->x,p1->y, context->distance_tol)) {
      path->count--;
      p0 = &pts[path->count-1];
      path->closed = 1;
    }
    if (path->count > 2) {
      area = Exo__polyArea(pts, path->count);
      if (path->winding == CCW && area < 0.0f)
        Exo__polyReverse(pts, path->count);
      if (path->winding == CW && area > 0.0f)
        Exo__polyReverse(pts, path->count);
    }
    for(i = 0; i < path->count; i++) {
      p0->dx = p1->x - p0->x;
      p0->dy = p1->y - p0->y;
      p0->len = Normalize(&p0->dx, &p0->dy);
      cache->bounds[0] = Minf(cache->bounds[0], p0->x);
      cache->bounds[1] = Minf(cache->bounds[1], p0->y);
      cache->bounds[2] = Maxf(cache->bounds[2], p0->x);
      cache->bounds[3] = Maxf(cache->bounds[3], p0->y);
      p0 = p1++;
    }
  }
}

ISC Exo__curveDivs(FPC r, FPC arc, FPC tol) {
  FPC da = _::ArcCosine(r / (r + tol)) * 2.0f;
  return Max2(2, (ISC)_::Ceiling(arc / da));
}

void Exo__chooseBevel(ISC bevel, Point2DFPC* p0, Point2DFPC* p1, FPC w,
  FPC* x0, FPC* y0, FPC* x1, FPC* y1) {
  if (bevel) {
    *x0 = p1->x + p0->dy * w;
    *y0 = p1->y - p0->dx * w;
    *x1 = p1->x + p1->dy * w;
    *y1 = p1->y - p1->dx * w;
  } else {
    *x0 = p1->x + p1->dmx * w;
    *y0 = p1->y + p1->dmy * w;
    *x1 = p1->x + p1->dmx * w;
    *y1 = p1->y + p1->dmy * w;
  }
}

VertexFPC* Exo__roundJoin(VertexFPC* dst, Point2DFPC* p0, Point2DFPC* p1,
  FPC lw, FPC rw, FPC lu, FPC ru, ISC ncap, FPC fringe) {
  ISC i, n;
  FPC dlx0 = p0->dy;
  FPC dly0 = -p0->dx;
  FPC dlx1 = p1->dy;
  FPC dly1 = -p1->dx;
  _NotUsed(fringe);
  if (p1->flags & PFPointLeft) {
    FPC lx0,ly0,lx1,ly1,a0,a1;
    Exo__chooseBevel(p1->flags & PFPointInterBevel, p0, p1, lw, &lx0,&ly0, &lx1,&ly1);
    a0 = _::ArcTangent2(-dly0, -dlx0);
    a1 = _::ArcTangent2(-dly1, -dlx1);
    if (a1 > a0) a1 -= _PiFPC*2;
    Exo__vset(dst, lx0, ly0, lu,1); dst++;
    Exo__vset(dst, p1->x - dlx0*rw, p1->y - dly0*rw, ru,1); dst++;
    n = Clamp((ISC)_::Ceiling(((a0 - a1) / _PiFPC) * ncap), 2, ncap);
    for (i = 0; i < n; i++) {
      FPC u = i/(FPC)(n-1);
      FPC a = a0 + u*(a1-a0);
      FPC rx = p1->x + _::Cosine(a) * rw;
      FPC ry = p1->y + _::Sine(a) * rw;
      Exo__vset(dst, p1->x, p1->y, 0.5f,1); dst++;
      Exo__vset(dst, rx, ry, ru,1); dst++;
    }
    Exo__vset(dst, lx1, ly1, lu,1); dst++;
    Exo__vset(dst, p1->x - dlx1*rw, p1->y - dly1*rw, ru,1); dst++;
  } else {
    FPC rx0,ry0,rx1,ry1,a0,a1;
    Exo__chooseBevel(p1->flags & PFPointInterBevel, p0, p1, -rw, &rx0,&ry0, &rx1,&ry1);
    a0 = _::ArcTangent2(dly0, dlx0);
    a1 = _::ArcTangent2(dly1, dlx1);
    if (a1 < a0) a1 += _PiFPC*2;
    Exo__vset(dst, p1->x + dlx0*rw, p1->y + dly0*rw, lu,1); dst++;
    Exo__vset(dst, rx0, ry0, ru,1); dst++;
    n = Clamp((ISC)_::Ceiling(((a1 - a0) / _PiFPC) * ncap), 2, ncap);
    for (i = 0; i < n; i++) {
      FPC u = i/(FPC)(n-1);
      FPC a = a0 + u*(a1-a0);
      FPC lx = p1->x + _::Cosine(a) * lw;
      FPC ly = p1->y + _::Sine(a) * lw;
      Exo__vset(dst, lx, ly, lu,1); dst++;
      Exo__vset(dst, p1->x, p1->y, 0.5f,1); dst++;
    }
    Exo__vset(dst, p1->x + dlx1*rw, p1->y + dly1*rw, lu,1); dst++;
    Exo__vset(dst, rx1, ry1, ru,1); dst++;
  }
  return dst;
}
VertexFPC* Exo__bevelJoin(VertexFPC* dst, Point2DFPC* p0, Point2DFPC* p1,
  FPC lw, FPC rw, FPC lu, FPC ru, FPC fringe) {
  FPC rx0,ry0,rx1,ry1;
  FPC lx0,ly0,lx1,ly1;
  FPC dlx0 = p0->dy;
  FPC dly0 = -p0->dx;
  FPC dlx1 = p1->dy;
  FPC dly1 = -p1->dx;
  _NotUsed(fringe);
  if (p1->flags & PFPointLeft) {
    Exo__chooseBevel(p1->flags & PFPointInterBevel, p0, p1, lw, &lx0,&ly0, &lx1,&ly1);
    Exo__vset(dst, lx0, ly0, lu,1); dst++;
    Exo__vset(dst, p1->x - dlx0*rw, p1->y - dly0*rw, ru,1); dst++;
    if (p1->flags & PFPointBevel) {
      Exo__vset(dst, lx0, ly0, lu,1); dst++;
      Exo__vset(dst, p1->x - dlx0*rw, p1->y - dly0*rw, ru,1); dst++;
      Exo__vset(dst, lx1, ly1, lu,1); dst++;
      Exo__vset(dst, p1->x - dlx1*rw, p1->y - dly1*rw, ru,1); dst++;
    } else {
      rx0 = p1->x - p1->dmx * rw;
      ry0 = p1->y - p1->dmy * rw;
      Exo__vset(dst, p1->x, p1->y, 0.5f,1); dst++;
      Exo__vset(dst, p1->x - dlx0*rw, p1->y - dly0*rw, ru,1); dst++;
      Exo__vset(dst, rx0, ry0, ru,1); dst++;
      Exo__vset(dst, rx0, ry0, ru,1); dst++;
      Exo__vset(dst, p1->x, p1->y, 0.5f,1); dst++;
      Exo__vset(dst, p1->x - dlx1*rw, p1->y - dly1*rw, ru,1); dst++;
    }
    Exo__vset(dst, lx1, ly1, lu,1); dst++;
    Exo__vset(dst, p1->x - dlx1*rw, p1->y - dly1*rw, ru,1); dst++;
  } else {
    Exo__chooseBevel(p1->flags & PFPointInterBevel, p0, p1, -rw, &rx0,&ry0, &rx1,&ry1);
    Exo__vset(dst, p1->x + dlx0*lw, p1->y + dly0*lw, lu,1); dst++;
    Exo__vset(dst, rx0, ry0, ru,1); dst++;
    if (p1->flags & PFPointBevel) {
      Exo__vset(dst, p1->x + dlx0*lw, p1->y + dly0*lw, lu,1); dst++;
      Exo__vset(dst, rx0, ry0, ru,1); dst++;
      Exo__vset(dst, p1->x + dlx1*lw, p1->y + dly1*lw, lu,1); dst++;
      Exo__vset(dst, rx1, ry1, ru,1); dst++;
    } else {
      lx0 = p1->x + p1->dmx * lw;
      ly0 = p1->y + p1->dmy * lw;
      Exo__vset(dst, p1->x + dlx0*lw, p1->y + dly0*lw, lu,1); dst++;
      Exo__vset(dst, p1->x, p1->y, 0.5f,1); dst++;
      Exo__vset(dst, lx0, ly0, lu,1); dst++;
      Exo__vset(dst, lx0, ly0, lu,1); dst++;
      Exo__vset(dst, p1->x + dlx1*lw, p1->y + dly1*lw, lu,1); dst++;
      Exo__vset(dst, p1->x, p1->y, 0.5f,1); dst++;
    }
    Exo__vset(dst, p1->x + dlx1*lw, p1->y + dly1*lw, lu,1); dst++;
    Exo__vset(dst, rx1, ry1, ru,1); dst++;
  }
  return dst;
}
VertexFPC* Exo__buttCapStart(VertexFPC* dst, Point2DFPC* p, FPC dx, FPC dy, 
  FPC w, FPC d, FPC aa, FPC u0, FPC u1) {
  FPC px = p->x - dx*d;
  FPC py = p->y - dy*d;
  FPC dlx = dy;
  FPC dly = -dx;
  Exo__vset(dst, px + dlx*w - dx*aa, py + dly*w - dy*aa, u0,0); dst++;
  Exo__vset(dst, px - dlx*w - dx*aa, py - dly*w - dy*aa, u1,0); dst++;
  Exo__vset(dst, px + dlx*w, py + dly*w, u0,1); dst++;
  Exo__vset(dst, px - dlx*w, py - dly*w, u1,1); dst++;
  return dst;
}

VertexFPC* Exo__buttCapEnd(VertexFPC* dst, Point2DFPC* p, FPC dx, FPC dy, FPC w, FPC d, FPC aa, FPC u0, FPC u1) {
  FPC px = p->x + dx*d;
  FPC py = p->y + dy*d;
  FPC dlx = dy;
  FPC dly = -dx;
  Exo__vset(dst, px + dlx*w, py + dly*w, u0,1); dst++;
  Exo__vset(dst, px - dlx*w, py - dly*w, u1,1); dst++;
  Exo__vset(dst, px + dlx*w + dx*aa, py + dly*w + dy*aa, u0,0); dst++;
  Exo__vset(dst, px - dlx*w + dx*aa, py - dly*w + dy*aa, u1,0); dst++;
  return dst;
}

VertexFPC* Exo__roundCapStart(VertexFPC* dst, Point2DFPC* p, FPC dx, FPC dy, FPC w, ISC ncap, FPC aa, FPC u0, FPC u1) {
  ISC i;
  FPC px = p->x;
  FPC py = p->y;
  FPC dlx = dy;
  FPC dly = -dx;
  _NotUsed(aa);
  for (i = 0; i < ncap; i++) {
    FPC a = i/(FPC)(ncap-1)*_PiFPC;
    FPC ax = _::Cosine(a) * w, ay = _::Sine(a) * w;
    Exo__vset(dst, px - dlx*ax - dx*ay, py - dly*ax - dy*ay, u0,1); dst++;
    Exo__vset(dst, px, py, 0.5f,1); dst++;
  }
  Exo__vset(dst, px + dlx*w, py + dly*w, u0,1); dst++;
  Exo__vset(dst, px - dlx*w, py - dly*w, u1,1); dst++;
  return dst;
}

VertexFPC* Exo__roundCapEnd(VertexFPC* dst, Point2DFPC* p, FPC dx, FPC dy, FPC w, ISC ncap, FPC aa, FPC u0, FPC u1) {
  ISC i;
  FPC px = p->x;
  FPC py = p->y;
  FPC dlx = dy;
  FPC dly = -dx;
  _NotUsed(aa);
  Exo__vset(dst, px + dlx*w, py + dly*w, u0,1); dst++;
  Exo__vset(dst, px - dlx*w, py - dly*w, u1,1); dst++;
  for (i = 0; i < ncap; i++) {
    FPC a = i/(FPC)(ncap-1)*_PiFPC;
    FPC ax = _::Cosine(a) * w, ay = _::Sine(a) * w;
    Exo__vset(dst, px, py, 0.5f,1); dst++;
    Exo__vset(dst, px - dlx*ax + dx*ay, py - dly*ax + dy*ay, u0,1); dst++;
  }
  return dst;
}

void Exo__calculateJoins(RenderContext* context, FPC w, ISC line_join, 
  FPC miter_limit) {
  PathCache* cache = &context->cache;
  ISC i, j;
  FPC iw = 0.0f;
  if (w > 0.0f) iw = 1.0f / w;
  for (i = 0; i < cache->npaths; i++) {
    RenderPath* path = &cache->paths[i];
    Point2DFPC* pts = &cache->points[path->first];
    Point2DFPC* p0 = &pts[path->count-1];
    Point2DFPC* p1 = &pts[0];
    ISC nleft = 0;
    path->nbevel = 0;
    for (j = 0; j < path->count; j++) {
      FPC dlx0, dly0, dlx1, dly1, dmr2, cross, limit;
      dlx0 = p0->dy;
      dly0 = -p0->dx;
      dlx1 = p1->dy;
      dly1 = -p1->dx;
      p1->dmx = (dlx0 + dlx1) * 0.5f;
      p1->dmy = (dly0 + dly1) * 0.5f;
      dmr2 = p1->dmx*p1->dmx + p1->dmy*p1->dmy;
      if (dmr2 > 0.000001f) {
        FPC scale = 1.0f / dmr2;
        if (scale > 600.0f) {
          scale = 600.0f;
        }
        p1->dmx *= scale;
        p1->dmy *= scale;
      }
      p1->flags = (p1->flags & PFPointCorner) ? PFPointCorner : 0;
      cross = p1->dx * p0->dy - p0->dx * p1->dy;
      if (cross > 0.0f) {
        nleft++;
        p1->flags |= PFPointLeft;
      }
      limit = Maxf(1.01f, Minf(p0->len, p1->len) * iw);
      if ((dmr2 * limit*limit) < 1.0f)
        p1->flags |= PFPointInterBevel;
      if (p1->flags & PFPointCorner) {
        if ((dmr2 * miter_limit*miter_limit) < 1.0f || line_join == LCBevel || line_join == LCRound) {
          p1->flags |= PFPointBevel;
        }
      }
      if ((p1->flags & (PFPointBevel | PFPointInterBevel)) != 0)
        path->nbevel++;
      p0 = p1++;
    }
    path->convex = (nleft == path->count) ? 1 : 0;
  }
}

ISC Exo__expandStroke(RenderContext* context, FPC w, FPC fringe, ISC line_cap,
  ISC line_join, FPC miter_limit) {
  PathCache* cache = &context->cache;
  VertexFPC* verts;
  VertexFPC* dst;
  ISC cverts, i, j;
  FPC aa = fringe;
  FPC u0 = 0.0f, u1 = 1.0f;
  ISC ncap = Exo__curveDivs(w, _PiFPC, context->tess_tol);  
  w += aa * 0.5f;
  if (aa == 0.0f) {
    u0 = 0.5f;
    u1 = 0.5f;
  }
  Exo__calculateJoins(context, w, line_join, miter_limit);
  cverts = 0;
  for (i = 0; i < cache->npaths; i++) {
    RenderPath* path = &cache->paths[i];
    ISC loop = (path->closed == 0) ? 0 : 1;
    if (line_join == LCRound)
      cverts += (path->count + path->nbevel*(ncap+2) + 1) * 2; 
    else
      cverts += (path->count + path->nbevel*5 + 1) * 2; 
    if (loop == 0) {
      if (line_cap == LCRound) {
        cverts += (ncap*2 + 2)*2;
      } else {
        cverts += (3+3)*2;
      }
    }
  }
  verts = Exo__allocTempVerts(context, cverts);
  if (verts == nullptr) return 0;
  for (i = 0; i < cache->npaths; i++) {
    RenderPath* path = &cache->paths[i];
    Point2DFPC* pts = &cache->points[path->first];
    Point2DFPC* p0;
    Point2DFPC* p1;
    ISC s, e, loop;
    FPC dx, dy;
    path->fill = 0;
    path->nfill = 0;
    loop = (path->closed == 0) ? 0 : 1;
    dst = verts;
    path->stroke = dst;
    if (loop) {
      p0 = &pts[path->count-1];
      p1 = &pts[0];
      s = 0;
      e = path->count;
    } else {
      p0 = &pts[0];
      p1 = &pts[1];
      s = 1;
      e = path->count-1;
    }
    if (loop == 0) {
      dx = p1->x - p0->x;
      dy = p1->y - p0->y;
      Normalize(&dx, &dy);
      if (line_cap == LCButt)
        dst = Exo__buttCapStart(dst, p0, dx, dy, w, -aa*0.5f, aa, u0, u1);
      else if (line_cap == LCButt || line_cap == LCSquare)
        dst = Exo__buttCapStart(dst, p0, dx, dy, w, w-aa, aa, u0, u1);
      else if (line_cap == LCRound)
        dst = Exo__roundCapStart(dst, p0, dx, dy, w, ncap, aa, u0, u1);
    }
    for (j = s; j < e; ++j) {
      if ((p1->flags & (PFPointBevel | PFPointInterBevel)) != 0) {
        if (line_join == LCRound) {
          dst = Exo__roundJoin(dst, p0, p1, w, w, u0, u1, ncap, aa);
        } else {
          dst = Exo__bevelJoin(dst, p0, p1, w, w, u0, u1, aa);
        }
      } else {
        Exo__vset(dst, p1->x + (p1->dmx * w), p1->y + (p1->dmy * w), u0,1); dst++;
        Exo__vset(dst, p1->x - (p1->dmx * w), p1->y - (p1->dmy * w), u1,1); dst++;
      }
      p0 = p1++;
    }
    if (loop) {
      Exo__vset(dst, verts[0].x, verts[0].y, u0,1); dst++;
      Exo__vset(dst, verts[1].x, verts[1].y, u1,1); dst++;
    } else {
      dx = p1->x - p0->x;
      dy = p1->y - p0->y;
      Normalize(&dx, &dy);
      if (line_cap == LCButt)
        dst = Exo__buttCapEnd(dst, p1, dx, dy, w, -aa*0.5f, aa, u0, u1);
      else if (line_cap == LCButt || line_cap == LCSquare)
        dst = Exo__buttCapEnd(dst, p1, dx, dy, w, w-aa, aa, u0, u1);
      else if (line_cap == LCRound)
        dst = Exo__roundCapEnd(dst, p1, dx, dy, w, ncap, aa, u0, u1);
    }
    path->nstroke = (ISC)(dst - verts);
    verts = dst;
  }
  return 1;
}

ISC Exo__expandFill(RenderContext* context, FPC w, ISC line_join, 
  FPC miter_limit) {
  PathCache* cache = &context->cache;
  VertexFPC* verts;
  VertexFPC* dst;
  ISC cverts, convex, i, j;
  FPC aa = context->fringe_width;
  ISC fringe = w > 0.0f;
  Exo__calculateJoins(context, w, line_join, miter_limit);
  cverts = 0;
  for (i = 0; i < cache->npaths; i++) {
    RenderPath* path = &cache->paths[i];
    cverts += path->count + path->nbevel + 1;
    if (fringe)
      cverts += (path->count + path->nbevel*5 + 1) * 2; 
  }
  verts = Exo__allocTempVerts(context, cverts);
  if (verts == nullptr) return 0;
  convex = cache->npaths == 1 && cache->paths[0].convex;
  for (i = 0; i < cache->npaths; i++) {
    RenderPath* path = &cache->paths[i];
    Point2DFPC* pts = &cache->points[path->first];
    Point2DFPC* p0;
    Point2DFPC* p1;
    FPC rw, lw, woff;
    FPC ru, lu;
    woff = 0.5f*aa;
    dst = verts;
    path->fill = dst;
    if (fringe) {
      p0 = &pts[path->count-1];
      p1 = &pts[0];
      for (j = 0; j < path->count; ++j) {
        if (p1->flags & PFPointBevel) {
          FPC dlx0 = p0->dy;
          FPC dly0 = -p0->dx;
          FPC dlx1 = p1->dy;
          FPC dly1 = -p1->dx;
          if (p1->flags & PFPointLeft) {
            FPC lx = p1->x + p1->dmx * woff;
            FPC ly = p1->y + p1->dmy * woff;
            Exo__vset(dst, lx, ly, 0.5f,1); dst++;
          } else {
            FPC lx0 = p1->x + dlx0 * woff;
            FPC ly0 = p1->y + dly0 * woff;
            FPC lx1 = p1->x + dlx1 * woff;
            FPC ly1 = p1->y + dly1 * woff;
            Exo__vset(dst, lx0, ly0, 0.5f,1); dst++;
            Exo__vset(dst, lx1, ly1, 0.5f,1); dst++;
          }
        } else {
          Exo__vset(dst, p1->x + (p1->dmx * woff), p1->y + (p1->dmy * woff), 0.5f,1); dst++;
        }
        p0 = p1++;
      }
    } else {
      for (j = 0; j < path->count; ++j) {
        Exo__vset(dst, pts[j].x, pts[j].y, 0.5f,1);
        dst++;
      }
    }
    path->nfill = (ISC)(dst - verts);
    verts = dst;
    if (fringe) {
      lw = w + woff;
      rw = w - woff;
      lu = 0;
      ru = 1;
      dst = verts;
      path->stroke = dst;
      if (convex) {
        lw = woff;  
        lu = 0.5f;  
      }
      p0 = &pts[path->count-1];
      p1 = &pts[0];
      for (j = 0; j < path->count; ++j) {
        if ((p1->flags & (PFPointBevel | PFPointInterBevel)) != 0) {
          dst = Exo__bevelJoin(dst, p0, p1, lw, rw, lu, ru, context->fringe_width);
        } else {
          Exo__vset(dst, p1->x + (p1->dmx * lw), p1->y + (p1->dmy * lw), lu,1); dst++;
          Exo__vset(dst, p1->x - (p1->dmx * rw), p1->y - (p1->dmy * rw), ru,1); dst++;
        }
        p0 = p1++;
      }
      Exo__vset(dst, verts[0].x, verts[0].y, lu,1); dst++;
      Exo__vset(dst, verts[1].x, verts[1].y, ru,1); dst++;
      path->nstroke = (ISC)(dst - verts);
      verts = dst;
    } else {
      path->stroke = nullptr;
      path->nstroke = 0;
    }
  }
  return 1;
}

void ExoBeginPath(RenderContext* context) {
  context->ncommands = 0;
  Exo__clearPathCache(context);
}

void ExoMoveTo(RenderContext* context, FPC x, FPC y) {
  FPC vals[] = { RCMoveTo, x, y };
  Exo__appendCommands(context, vals, _CountOf(vals));
}

void ExoLineTo(RenderContext* context, FPC x, FPC y) {
  FPC vals[] = { RCLineTo, x, y };
  Exo__appendCommands(context, vals, _CountOf(vals));
}

void ExoBezierTo(RenderContext* context, FPC c1x, FPC c1y, FPC c2x, FPC c2y, 
  FPC x, FPC y) {
  FPC vals[] = { RCBiezerTo, c1x, c1y, c2x, c2y, x, y };
  Exo__appendCommands(context, vals, _CountOf(vals));
}

void ExoQuadTo(RenderContext* context, FPC cx, FPC cy, FPC x, FPC y) {
    FPC x0 = context->commandx;
    FPC y0 = context->commandy;
    FPC vals[] = { RCBiezerTo,
        x0 + 2.0f/3.0f*(cx - x0), y0 + 2.0f/3.0f*(cy - y0),
        x + 2.0f/3.0f*(cx - x), y + 2.0f/3.0f*(cy - y),
        x, y };
    Exo__appendCommands(context, vals, _CountOf(vals));
}

void ExoArcTo(RenderContext* context, FPC x1, FPC y1, FPC x2, FPC y2, 
  FPC radius) {
  FPC x0 = context->commandx;
  FPC y0 = context->commandy;
  FPC dx0,dy0, dx1,dy1, a, d, cx,cy, a0,a1;
  ISC dir;
  if (context->ncommands == 0) {
    return;
  }
  if (Exo__ptEquals(x0,y0, x1,y1, context->distance_tol) ||
    Exo__ptEquals(x1,y1, x2,y2, context->distance_tol) ||
    Exo__distPtSeg(x1,y1, x0,y0, x2,y2) < context->distance_tol*context->distance_tol ||
    radius < context->distance_tol) {
    ExoLineTo(context, x1,y1);
    return;
  }
  dx0 = x0-x1;
  dy0 = y0-y1;
  dx1 = x2-x1;
  dy1 = y2-y1;
  Normalize(&dx0,&dy0);
  Normalize(&dx1,&dy1);
  a = _::ArcCosine(dx0*dx1 + dy0*dy1);
  d = radius / _::Tan(a/2.0f);
  if (d > 10000.0f) {
    ExoLineTo(context, x1,y1);
    return;
  }
  if (Cross(dx0,dy0, dx1,dy1) > 0.0f) {
    cx = x1 + dx0*d + dy0*radius;
    cy = y1 + dy0*d + -dx0*radius;
    a0 = _::ArcTangent2(dx0, -dy0);
    a1 = _::ArcTangent2(-dx1, dy1);
    dir = CW;
  } else {
    cx = x1 + dx0*d + -dy0*radius;
    cy = y1 + dy0*d + dx0*radius;
    a0 = _::ArcTangent2(-dx0, dy0);
    a1 = _::ArcTangent2(dx1, -dy1);
    dir = CCW;
  }
  ExoArc(context, cx, cy, radius, a0, a1, dir);
}

void ExoClosePath(RenderContext* context) {
  FPC vals[] = { RCClose };
  Exo__appendCommands(context, vals, _CountOf(vals));
}

void ExoPathWinding(RenderContext* context, ISC dir) {
  FPC vals[] = { RCWinding, (FPC)dir };
  Exo__appendCommands(context, vals, _CountOf(vals));
}

void ExoArc(RenderContext* context, FPC cx, FPC cy, FPC r, FPC a0, FPC a1, 
  ISC dir) {
  FPC a = 0, da = 0, hda = 0, kappa = 0;
  FPC dx = 0, dy = 0, x = 0, y = 0, tanx = 0, tany = 0;
  FPC px = 0, py = 0, ptanx = 0, ptany = 0;
  FPC vals[3 + 5*7 + 100];
  ISC i, ndivs, nvals;
  ISC move = context->ncommands > 0 ? RCLineTo : RCMoveTo;
  da = a1 - a0;
  if (dir == CW) {
    if (Absolute(da) >= _PiFPC*2) {
      da = _PiFPC*2;
    } else {
      while (da < 0.0f) da += _PiFPC*2;
    }
  } else {
    if (Absolute(da) >= _PiFPC*2) {
      da = -_PiFPC*2;
    } else {
      while (da > 0.0f) da -= _PiFPC*2;
    }
  }
  ndivs = Max2(1, Min2((ISC)(Absolute(da) / (_PiFPC*0.5f) + 0.5f), 5));
  hda = (da / (FPC)ndivs) / 2.0f;
  kappa = Absolute(4.0f / 3.0f * (1.0f - _::Cosine(hda)) / _::Sine(hda));
  if (dir == CCW)
    kappa = -kappa;
  nvals = 0;
  for (i = 0; i <= ndivs; i++) {
    a = a0 + da * (i/(FPC)ndivs);
    dx = _::Cosine(a);
    dy = _::Sine(a);
    x = cx + dx*r;
    y = cy + dy*r;
    tanx = -dy*r*kappa;
    tany = dx*r*kappa;
    if (i == 0) {
      vals[nvals++] = (FPC)move;
      vals[nvals++] = x;
      vals[nvals++] = y;
    } else {
      vals[nvals++] = RCBiezerTo;
      vals[nvals++] = px+ptanx;
      vals[nvals++] = py+ptany;
      vals[nvals++] = x-tanx;
      vals[nvals++] = y-tany;
      vals[nvals++] = x;
      vals[nvals++] = y;
    }
    px = x;
    py = y;
    ptanx = tanx;
    ptany = tany;
  }
  Exo__appendCommands(context, vals, nvals);
}
void ExoRect(RenderContext* context, FPC x, FPC y, FPC w, FPC h) {
  FPC vals[] = {
    RCMoveTo, x,y,
    RCLineTo, x,y+h,
    RCLineTo, x+w,y+h,
    RCLineTo, x+w,y,
    RCClose
  };
  Exo__appendCommands(context, vals, _CountOf(vals));
}

void ExoRoundedRect(RenderContext* context, FPC x, FPC y, FPC w, FPC h, FPC r) {
  ExoRoundedRectVarying(context, x, y, w, h, r, r, r, r);
}

void ExoRoundedRectVarying(RenderContext* context, FPC x, FPC y, FPC w, FPC h, FPC radTopLeft, FPC radTopRight, FPC radBottomRight, FPC radBottomLeft) {
  if(radTopLeft < 0.1f && radTopRight < 0.1f && radBottomRight < 0.1f && radBottomLeft < 0.1f) {
    ExoRect(context, x, y, w, h);
    return;
  } else {
    FPC halfw = Absolute(w)*0.5f;
    FPC halfh = Absolute(h)*0.5f;
    FPC rxBL = Minf(radBottomLeft, halfw) * Signf(w), ryBL = Minf(radBottomLeft, halfh) * Signf(h);
    FPC rxBR = Minf(radBottomRight, halfw) * Signf(w), ryBR = Minf(radBottomRight, halfh) * Signf(h);
    FPC rxTR = Minf(radTopRight, halfw) * Signf(w), ryTR = Minf(radTopRight, halfh) * Signf(h);
    FPC rxTL = Minf(radTopLeft, halfw) * Signf(w), ryTL = Minf(radTopLeft, halfh) * Signf(h);
    FPC vals[] = {
      RCMoveTo, x, y + ryTL,
      RCLineTo, x, y + h - ryBL,
      RCBiezerTo, x, y + h - ryBL*(1 - _Kappa90), x + rxBL*(1 - _Kappa90), y + h, x + rxBL, y + h,
      RCLineTo, x + w - rxBR, y + h,
      RCBiezerTo, x + w - rxBR*(1 - _Kappa90), y + h, x + w, y + h - ryBR*(1 - _Kappa90), x + w, y + h - ryBR,
      RCLineTo, x + w, y + ryTR,
      RCBiezerTo, x + w, y + ryTR*(1 - _Kappa90), x + w - rxTR*(1 - _Kappa90), y, x + w - rxTR, y,
      RCLineTo, x + rxTL, y,
      RCBiezerTo, x + rxTL*(1 - _Kappa90), y, x, y + ryTL*(1 - _Kappa90), x, y + ryTL,
      RCClose
    };
    Exo__appendCommands(context, vals, _CountOf(vals));
  }
}

void ExoEllipse(RenderContext* context, FPC cx, FPC cy, FPC rx, FPC ry) {
  FPC vals[] = {
    RCMoveTo, cx-rx, cy,
    RCBiezerTo, cx-rx, cy+ry*_Kappa90, cx-rx*_Kappa90, cy+ry, cx, cy+ry,
    RCBiezerTo, cx+rx*_Kappa90, cy+ry, cx+rx, cy+ry*_Kappa90, cx+rx, cy,
    RCBiezerTo, cx+rx, cy-ry*_Kappa90, cx+rx*_Kappa90, cy-ry, cx, cy-ry,
    RCBiezerTo, cx-rx*_Kappa90, cy-ry, cx-rx, cy-ry*_Kappa90, cx-rx, cy,
    RCClose
  };
  Exo__appendCommands(context, vals, _CountOf(vals));
}

void ExoCircle(RenderContext* context, FPC cx, FPC cy, FPC r) {
  ExoEllipse(context, cx,cy, r,r);
}

void ExoFill(RenderContext* context) {
  RenderState* state = Exo__getState(context);
  const RenderPath* path;
  VectorPaint fillPaint = state->fill;
  ISC i;
  Exo__flattenPaths(context);
  if (context->params.edgeAntiAlias && state->shape_antialias)
    Exo__expandFill(context, context->fringe_width, LCMiter, 2.4f);
  else
    Exo__expandFill(context, 0.0f, LCMiter, 2.4f);
  fillPaint.inner_color.a *= state->alpha;
  fillPaint.outer_color.a *= state->alpha;
  context->params.renderFill(context->params.userPtr, &fillPaint, state->composite_operation, &state->scissor, context->fringe_width,
               context->cache.bounds, context->cache.paths, context->cache.npaths);
  for (i = 0; i < context->cache.npaths; i++) {
    path = &context->cache.paths[i];
    context->fill_tri_count += path->nfill-2;
    context->fill_tri_count += path->nstroke-2;
    context->draw_call_count += 2;
  }
}

void ExoStroke(RenderContext* context) {
  RenderState* state = Exo__getState(context);
  FPC scale = Exo__getAverageScale(state->xform);
  FPC stroke_width = Clampf(state->stroke_width * scale, 0.0f, 200.0f);
  VectorPaint strokePaint = state->stroke;
  const RenderPath* path;
  ISC i;
  if (stroke_width < context->fringe_width) {
    FPC alpha = Clampf(stroke_width / context->fringe_width, 0.0f, 1.0f);
    strokePaint.inner_color.a *= alpha*alpha;
    strokePaint.outer_color.a *= alpha*alpha;
    stroke_width = context->fringe_width;
  }
  strokePaint.inner_color.a *= state->alpha;
  strokePaint.outer_color.a *= state->alpha;
  Exo__flattenPaths(context);
  if (context->params.edgeAntiAlias && state->shape_antialias)
    Exo__expandStroke(context, stroke_width*0.5f, context->fringe_width, state->line_cap, state->line_join, state->miter_limit);
  else
    Exo__expandStroke(context, stroke_width*0.5f, 0.0f, state->line_cap, state->line_join, state->miter_limit);
  context->params.renderStroke(context->params.userPtr, &strokePaint, state->composite_operation, &state->scissor, context->fringe_width,
               stroke_width, context->cache.paths, context->cache.npaths);
  for (i = 0; i < context->cache.npaths; i++) {
    path = &context->cache.paths[i];
    context->stroke_tri_count += path->nstroke-2;
    context->draw_call_count++;
  }
}

/* ── Fonts ─────────────────────────────────────────────────────────────
The cache is FontStash: it takes the font bytes, decodes them with TrueTypeFont
and fills one 8 bit atlas, all out of the same Room Wall this renderer uses.
Nothing here opens a file, so a font arrives as bytes. */

ISC ExoCreateFont(RenderContext* context, const CHR* name, const CHR* filename) {
  /* There is no file reader in this tree: a caller with a font on disk loads
  it and hands the bytes to ExoCreateFontMem. */
  (void)context; (void)name; (void)filename;
  return -1;
}

ISC ExoCreateFontAtIndex(RenderContext* context, const CHR* name,
  const CHR* filename, const ISC fontIndex) {
  (void)context; (void)name; (void)filename; (void)fontIndex;
  return -1;
}

ISC ExoCreateFontMem(RenderContext* context, const CHR* name, IUA* data,
  ISC ndata, ISC freeData) {
  /* The bytes are borrowed, not copied: they have to outlive the renderer, so
  freeData is not something the stash can honour. */
  (void)freeData;
  if (context == _NilP || context->fs == _NilP) return -1;
  return context->fs->AddFont(name, data, ndata);
}

ISC ExoCreateFontMemAtIndex(RenderContext* context, const CHR* name, IUA* data,
  ISC ndata, ISC freeData, const ISC fontIndex) {
  /* One face per font block: a collection member is selected by handing the
  block the offset fontIndex reports, which is what TrueTypeFont::FontOffset
  answers. */
  (void)freeData;
  if (context == _NilP || context->fs == _NilP || data == _NilP) return -1;
  ISC offset = TrueTypeFont::FontOffset(data, fontIndex);
  if (offset < 0) return -1;
  return context->fs->AddFont(name, data + offset, ndata - offset);
}

ISC ExoFindFont(RenderContext* context, const CHR* name) {
  if (context == _NilP || context->fs == _NilP || name == _NilP) return -1;
  return context->fs->FindFont(name);
}

ISC ExoAddFallbackFontId(RenderContext* context, ISC baseFont, ISC fallbackFont) {
  if (context == _NilP || context->fs == _NilP) return -1;
  return context->fs->AddFallback(baseFont, fallbackFont);
}

ISC ExoAddFallbackFont(RenderContext* context, const CHR* baseFont,
  const CHR* fallbackFont) {
  return ExoAddFallbackFontId(context, ExoFindFont(context, baseFont),
                              ExoFindFont(context, fallbackFont));
}

void ExoResetFallbackFontsId(RenderContext* context, ISC baseFont) {
  if (context == _NilP || context->fs == _NilP) return;
  context->fs->ResetFallbacks(baseFont);
}

void ExoResetFallbackFonts(RenderContext* context, const CHR* baseFont) {
  ExoResetFallbackFontsId(context, ExoFindFont(context, baseFont));
}

void ExoFontSize(RenderContext* context, FPC size) {
  RenderState* state = Exo__getState(context);
  state->font_size = size;
}

void ExoFontBlur(RenderContext* context, FPC blur) {
  RenderState* state = Exo__getState(context);
  state->font_blur = blur;
}

void ExoTextLetterSpacing(RenderContext* context, FPC spacing) {
  RenderState* state = Exo__getState(context);
  state->letter_spacing = spacing;
}

void ExoTextLineHeight(RenderContext* context, FPC line_height) {
  RenderState* state = Exo__getState(context);
  state->line_height = line_height;
}

void ExoTextAlign(RenderContext* context, ISC align) {
  RenderState* state = Exo__getState(context);
  state->text_align = align;
}

void ExoFontFaceId(RenderContext* context, ISC font) {
  RenderState* state = Exo__getState(context);
  state->font_id = font;
}

void ExoFontFace(RenderContext* context, const CHR* font) {
  RenderState* state = Exo__getState(context);
  state->font_id = context->fs == _NilP ? -1 : context->fs->FindFont(font);
}

FPC Exo__quantize(FPC a, FPC d) {
  return (ISC(a / d + 0.5f)) * d;
}

/* The text is laid out at the device scale and transformed back per vertex, so
a zoomed view rasterizes glyphs for the pixels it will actually cover. */
FPC Exo__getFontScale(RenderState* state) {
  return Minf(Exo__quantize(Exo__getAverageScale(state->xform), 0.01f), 4.0f);
}

/* Pushes the render state into the stash. The stash keeps its own stack; this
renderer's Push and Pop already moved the state, so the stash is resynchronized
before every text call instead of being pushed in step. */
void Exo__setFontState(RenderContext* context, RenderState* state, FPC scale) {
  FontStash* fs = context->fs;
  if (fs == _NilP) return;
  fs->SetFont(state->font_id);
  fs->SetSize(state->font_size * scale);
  fs->SetSpacing(state->letter_spacing * scale);
  fs->SetBlur(state->font_blur * scale);
  fs->SetAlign(TextAlign(state->text_align));
}

void Exo__flushTextTexture(RenderContext* context) {
  if (context->fs == _NilP || context->font_image == 0) return;
  ISC dirty[4];
  if (!context->fs->ValidateTexture(dirty)) return;
  ISC iw, ih;
  const IUA* data = context->fs->Texture(iw, ih);
  /* The backend takes the atlas base pointer and the changed rectangle; its
  row pitch is the atlas width, which is what the texture was created at. */
  context->params.renderUpdateTexture(context->params.userPtr,
    context->font_image, dirty[0], dirty[1], dirty[2], dirty[3], data);
}

/* Grows the atlas and the texture that mirrors it, up to the block the Wall
took at creation. Returns false when it is already at the ceiling. */
ISC Exo__growTextAtlas(RenderContext* context) {
  if (context->fs == _NilP || context->font_image == 0) return 0;
  ISC iw, ih;
  context->fs->Texture(iw, ih);
  ISC nw = iw, nh = ih;
  if (iw > ih) nh = ih * 2;
  else         nw = iw * 2;
  if (nw > context->atlas_max) nw = context->atlas_max;
  if (nh > context->atlas_max) nh = context->atlas_max;
  if (nw == iw && nh == ih) return 0;
  if (!context->fs->ResetAtlas(nw, nh)) return 0;
  context->params.renderDeleteTexture(context->params.userPtr,
                                      context->font_image);
  context->font_image = context->params.renderCreateTexture(
    context->params.userPtr, TextureAlpha, nw, nh, 0, _NilP);
  return context->font_image != 0;
}

void Exo__renderText(RenderContext* context, VertexFPC* verts, ISC nverts) {
  RenderState* state = Exo__getState(context);
  VectorPaint paint = state->fill;
  paint.image = context->font_image;
  paint.inner_color.a *= state->alpha;
  paint.outer_color.a *= state->alpha;
  context->params.renderTriangles(context->params.userPtr, &paint, state->composite_operation, &state->scissor, verts, nverts, context->fringe_width);
  context->draw_call_count++;
  context->text_tri_count += nverts/3;
}

ISC Exo__isTransformFlipped(const FPC *xform) {
  FPC det = xform[0] * xform[3] - xform[2] * xform[1];
  return(det < 0);
}

/* Turns one glyph quad into the two triangles, applying the transform per
vertex so a rotation or a scale lands on the atlas sprite intact. */
void Exo__emitQuad(RenderState* state, VertexFPC* verts, ISC& nverts,
                   ISC cverts, const StashQuad& q, FPC invscale, ISC flipped) {
  FPC c[4*2];
  FPC s0 = q.s0, t0 = q.t0, s1 = q.s1, t1 = q.t1;
  FPC y0 = q.y0, y1 = q.y1;
  if (flipped) {
    FPC tmp = y0; y0 = y1; y1 = tmp;
    tmp = t0; t0 = t1; t1 = tmp;
  }
  ExoTransformPoint(&c[0],&c[1], state->xform, q.x0*invscale, y0*invscale);
  ExoTransformPoint(&c[2],&c[3], state->xform, q.x1*invscale, y0*invscale);
  ExoTransformPoint(&c[4],&c[5], state->xform, q.x1*invscale, y1*invscale);
  ExoTransformPoint(&c[6],&c[7], state->xform, q.x0*invscale, y1*invscale);
  if (nverts + 6 > cverts) return;
  Exo__vset(&verts[nverts], c[0], c[1], s0, t0); nverts++;
  Exo__vset(&verts[nverts], c[4], c[5], s1, t1); nverts++;
  Exo__vset(&verts[nverts], c[2], c[3], s1, t0); nverts++;
  Exo__vset(&verts[nverts], c[0], c[1], s0, t0); nverts++;
  Exo__vset(&verts[nverts], c[6], c[7], s0, t1); nverts++;
  Exo__vset(&verts[nverts], c[4], c[5], s1, t1); nverts++;
}

FPC ExoText(RenderContext* context, FPC x, FPC y, const CHR* string,
  const CHR* end) {
  RenderState* state = Exo__getState(context);
  if (context->fs == _NilP || state->font_id < 0) return x;
  FPC scale = Exo__getFontScale(state) * context->device_pixel_ratio;
  FPC invscale = 1.0f / scale;
  ISC flipped = Exo__isTransformFlipped(state->xform);
  if (end == _NilP)
    end = string + Exo__cstrlen(string);
  ISC cverts = Max2(2, (ISC)(end - string)) * 6;
  VertexFPC* verts = Exo__allocTempVerts(context, cverts);
  if (verts == _NilP) return x;
  Exo__setFontState(context, state, scale);

  ISC nverts = 0;
  FPC pen_x = x;
  TextIter iter;
  StashQuad quad;
  /* A glyph that does not fit the atlas is not drawn, but the pen still moves,
  so the pass restarts at the larger atlas instead of losing the line. Four
  attempts is the whole growth ladder from the start size to the ceiling. */
  for (ISC attempt = 0; attempt < 4; ++attempt) {
    nverts = 0;
    if (!context->fs->TextIterInit(iter, x*scale, y*scale, string, end,
                                   BitmapMode::Required))
      return x;
    context->fs->TakeMissing();
    BOL full = false;
    while (context->fs->TextIterNext(iter, quad)) {
      if (context->fs->TakeMissing() != 0) { full = true; break; }
      Exo__emitQuad(state, verts, nverts, cverts, quad, invscale, flipped);
    }
    pen_x = iter.next_x * invscale;
    if (!full) break;
    if (!Exo__growTextAtlas(context)) break;
  }
  Exo__flushTextTexture(context);
  Exo__renderText(context, verts, nverts);
  return pen_x;
}

void ExoTextBox(RenderContext* context, FPC x, FPC y, FPC breakRowWidth, const CHR* string, const CHR* end) {
  RenderState* state = Exo__getState(context);
  TextRow rows[2];
  ISC nrows = 0, i;
  ISC oldAlign = state->text_align;
  ISC halign = state->text_align & (AlignLeft | AlignCenter | AlignRight);
  ISC valign = state->text_align & (AlignTop | AlignMiddle | AlignBottom | AlignBaseline);
  FPC lineh = 0;
  if (state->font_id < 0) return;
  ExoTextMetrics(context, _NilP, _NilP, &lineh);
  state->text_align = AlignLeft | valign;
  while ((nrows = ExoTextBreakLines(context, string, end, breakRowWidth, rows, 2))) {
    for (i = 0; i < nrows; i++) {
      TextRow* row = &rows[i];
      if (halign & AlignLeft)
        ExoText(context, x, y, row->start, row->end);
      else if (halign & AlignCenter)
        ExoText(context, x + breakRowWidth*0.5f - row->width*0.5f, y, row->start, row->end);
      else if (halign & AlignRight)
        ExoText(context, x + breakRowWidth - row->width, y, row->start, row->end);
      y += lineh * state->line_height;
    }
    string = rows[nrows-1].next;
  }
  state->text_align = oldAlign;
}

ISC ExoTextGlyphPositions(RenderContext* context, FPC x, FPC y, const CHR* string, const CHR* end, GlyphPosition* positions, ISC maxPositions) {
  RenderState* state = Exo__getState(context);
  if (context->fs == _NilP || state->font_id < 0) return 0;
  if (positions == _NilP || maxPositions <= 0) return 0;
  FPC scale = Exo__getFontScale(state) * context->device_pixel_ratio;
  FPC invscale = 1.0f / scale;
  if (end == _NilP)
    end = string + Exo__cstrlen(string);
  if (string == end)
    return 0;
  Exo__setFontState(context, state, scale);
  ISC npos = 0;
  TextIter iter;
  StashQuad q;
  if (!context->fs->TextIterInit(iter, x*scale, y*scale, string, end,
                                 BitmapMode::Optional))
    return 0;
  while (iter.cursor < end) {
    const CHR* start = iter.cursor;
    if (!context->fs->TextIterNext(iter, q)) break;
    positions[npos].str = start;
    positions[npos].x = iter.x * invscale;
    positions[npos].min_x = Minf(iter.x, q.x0) * invscale;
    positions[npos].max_x = Maxf(iter.next_x, q.x1) * invscale;
    npos++;
    if (npos >= maxPositions)
      break;
  }
  return npos;
}

enum CodepointType {
  CTSpace,
  CTNewLine,
  CTCharacter,
  CTCJKCharacter,
};

/* Breaks a string into rows. The walk is the stash's codepoint iterator; the
break rule is the one a UI expects: break on newline always, break after a run
of spaces that follows a word, and break mid word only when a single word is
wider than the row. */
ISC ExoTextBreakLines(RenderContext* context, const CHR* string, const CHR* end, FPC breakRowWidth, TextRow* rows, ISC maxRows) {
  RenderState* state = Exo__getState(context);
  if (context->fs == _NilP || state->font_id < 0) return 0;
  if (maxRows == 0 || rows == _NilP) return 0;
  FPC scale = Exo__getFontScale(state) * context->device_pixel_ratio;
  FPC invscale = 1.0f / scale;
  ISC nrows = 0;
  FPC rowStartX = 0;
  FPC rowWidth = 0;
  FPC rowMinX = 0;
  FPC rowMaxX = 0;
  const CHR* rowStart = _NilP;
  const CHR* rowEnd = _NilP;
  const CHR* wordStart = _NilP;
  FPC wordStartX = 0;
  FPC wordMinX = 0;
  const CHR* breakEnd = _NilP;
  FPC breakWidth = 0;
  FPC breakMaxX = 0;
  ISC type = CTSpace, ptype = CTSpace;
  IUC pcodepoint = 0;
  if (end == _NilP)
    end = string + Exo__cstrlen(string);
  if (string == end) return 0;
  Exo__setFontState(context, state, scale);
  breakRowWidth *= scale;
  TextIter iter;
  StashQuad q;
  if (!context->fs->TextIterInit(iter, 0, 0, string, end,
                                 BitmapMode::Optional))
    return 0;
  while (iter.cursor < end) {
    const CHR* glyph_start = iter.cursor;
    if (!context->fs->TextIterNext(iter, q)) break;
    const CHR* glyph_next = iter.cursor;
    switch (iter.code_point) {
      case 9:
      case 11:
      case 12:
      case 32:
      case 0x00a0:
        type = CTSpace;
        break;
      case 10:
        type = pcodepoint == 13 ? CTSpace : CTNewLine;
        break;
      case 13:
        type = pcodepoint == 10 ? CTSpace : CTNewLine;
        break;
      case 0x0085:
        type = CTNewLine;
        break;
      default:
        if ((iter.code_point >= 0x4E00 && iter.code_point <= 0x9FFF) ||
          (iter.code_point >= 0x3000 && iter.code_point <= 0x30FF) ||
          (iter.code_point >= 0xFF00 && iter.code_point <= 0xFFEF) ||
          (iter.code_point >= 0x1100 && iter.code_point <= 0x11FF) ||
          (iter.code_point >= 0x3130 && iter.code_point <= 0x318F) ||
          (iter.code_point >= 0xAC00 && iter.code_point <= 0xD7AF))
          type = CTCJKCharacter;
        else
          type = CTCharacter;
        break;
    }
    if (type == CTNewLine) {
      rows[nrows].start = rowStart != _NilP ? rowStart : glyph_start;
      rows[nrows].end = rowEnd != _NilP ? rowEnd : glyph_start;
      rows[nrows].width = rowWidth * invscale;
      rows[nrows].min_x = rowMinX * invscale;
      rows[nrows].max_x = rowMaxX * invscale;
      rows[nrows].next = glyph_next;
      nrows++;
      if (nrows >= maxRows)
        return nrows;
      breakEnd = rowStart;
      breakWidth = 0.0;
      breakMaxX = 0.0;
      rowStart = _NilP;
      rowEnd = _NilP;
      rowWidth = 0;
      rowMinX = rowMaxX = 0;
    } else {
      if (rowStart == _NilP) {
        if (type == CTCharacter || type == CTCJKCharacter) {
          rowStartX = iter.x;
          rowStart = glyph_start;
          rowEnd = glyph_next;
          rowWidth = iter.next_x - rowStartX;
          rowMinX = q.x0 - rowStartX;
          rowMaxX = q.x1 - rowStartX;
          wordStart = glyph_start;
          wordStartX = iter.x;
          wordMinX = q.x0 - rowStartX;
          breakEnd = rowStart;
          breakWidth = 0.0;
          breakMaxX = 0.0;
        }
      } else {
        FPC nextWidth = iter.next_x - rowStartX;
        if (type == CTCharacter || type == CTCJKCharacter) {
          rowEnd = glyph_next;
          rowWidth = iter.next_x - rowStartX;
          rowMaxX = q.x1 - rowStartX;
        }
        if (((ptype == CTCharacter ||
          ptype == CTCJKCharacter) && type == CTSpace) || type == CTCJKCharacter) {
          breakEnd = glyph_start;
          breakWidth = rowWidth;
          breakMaxX = rowMaxX;
        }
        if ((ptype == CTSpace && (type == CTCharacter ||
            type == CTCJKCharacter)) || type == CTCJKCharacter) {
          wordStart = glyph_start;
          wordStartX = iter.x;
          wordMinX = q.x0;
        }
        if ((type == CTCharacter || type == CTCJKCharacter) && nextWidth > breakRowWidth) {
          if (breakEnd == rowStart) {
            rows[nrows].start = rowStart;
            rows[nrows].end = glyph_start;
            rows[nrows].width = rowWidth * invscale;
            rows[nrows].min_x = rowMinX * invscale;
            rows[nrows].max_x = rowMaxX * invscale;
            rows[nrows].next = glyph_start;
            nrows++;
            if (nrows >= maxRows)
              return nrows;
            rowStartX = iter.x;
            rowStart = glyph_start;
            rowEnd = glyph_next;
            rowWidth = iter.next_x - rowStartX;
            rowMinX = q.x0 - rowStartX;
            rowMaxX = q.x1 - rowStartX;
            wordStart = glyph_start;
            wordStartX = iter.x;
            wordMinX = q.x0 - rowStartX;
          } else {
            rows[nrows].start = rowStart;
            rows[nrows].end = breakEnd;
            rows[nrows].width = breakWidth * invscale;
            rows[nrows].min_x = rowMinX * invscale;
            rows[nrows].max_x = breakMaxX * invscale;
            rows[nrows].next = wordStart;
            nrows++;
            if (nrows >= maxRows)
              return nrows;
            rowStartX = wordStartX;
            rowStart = wordStart;
            rowEnd = glyph_next;
            rowWidth = iter.next_x - rowStartX;
            rowMinX = wordMinX - rowStartX;
            rowMaxX = q.x1 - rowStartX;
          }
          breakEnd = rowStart;
          breakWidth = 0.0;
          breakMaxX = 0.0;
        }
      }
    }
    pcodepoint = IUC(iter.code_point);
    ptype = type;
  }
  if (rowStart != _NilP) {
    rows[nrows].start = rowStart;
    rows[nrows].end = rowEnd;
    rows[nrows].width = rowWidth * invscale;
    rows[nrows].min_x = rowMinX * invscale;
    rows[nrows].max_x = rowMaxX * invscale;
    rows[nrows].next = end;
    nrows++;
  }
  return nrows;
}

FPC ExoTextBounds(RenderContext* context, FPC x, FPC y, const CHR* string,
  const CHR* end, FPC* bounds) {
  RenderState* state = Exo__getState(context);
  if (context->fs == _NilP || state->font_id < 0) return 0;
  FPC scale = Exo__getFontScale(state) * context->device_pixel_ratio;
  FPC invscale = 1.0f / scale;
  Exo__setFontState(context, state, scale);
  FPC width = context->fs->TextBounds(x*scale, y*scale, string, end, bounds);
  if (bounds != _NilP) {
    /* The line box replaces the vertical extent: a string of wide letters and
    one of narrow ones sit on the same baseline range. */
    FPC min_y = 0, max_y = 0;
    context->fs->LineBounds(y*scale, min_y, max_y);
    bounds[0] *= invscale;
    bounds[1] = min_y * invscale;
    bounds[2] *= invscale;
    bounds[3] = max_y * invscale;
  }
  return width * invscale;
}

void ExoTextBoxBounds(RenderContext* context, FPC x, FPC y, FPC breakRowWidth,
  const CHR* string, const CHR* end, FPC* bounds) {
  RenderState* state = Exo__getState(context);
  TextRow rows[2];
  FPC scale = Exo__getFontScale(state) * context->device_pixel_ratio;
  FPC invscale = 1.0f / scale;
  ISC nrows = 0, i;
  ISC oldAlign = state->text_align;
  ISC halign = state->text_align & (AlignLeft | AlignCenter | AlignRight);
  ISC valign = state->text_align & (AlignTop | AlignMiddle | AlignBottom | AlignBaseline);
  FPC lineh = 0, rminy = 0, rmaxy = 0;
  FPC min_x, miny, max_x, maxy;
  if (context->fs == _NilP || state->font_id < 0) {
    if (bounds != _NilP)
      bounds[0] = bounds[1] = bounds[2] = bounds[3] = 0.0f;
    return;
  }
  ExoTextMetrics(context, _NilP, _NilP, &lineh);
  state->text_align = AlignLeft | valign;
  min_x = max_x = x;
  miny = maxy = y;
  Exo__setFontState(context, state, scale);
  context->fs->LineBounds(0, rminy, rmaxy);
  rminy *= invscale;
  rmaxy *= invscale;
  while ((nrows = ExoTextBreakLines(context, string, end, breakRowWidth,
    rows, 2))) {
    for (i = 0; i < nrows; i++) {
      TextRow* row = &rows[i];
      FPC rminx, rmaxx, dx = 0;
      if (halign & AlignLeft)
        dx = 0;
      else if (halign & AlignCenter)
        dx = breakRowWidth*0.5f - row->width*0.5f;
      else if (halign & AlignRight)
        dx = breakRowWidth - row->width;
      rminx = x + row->min_x + dx;
      rmaxx = x + row->max_x + dx;
      min_x = Minf(min_x, rminx);
      max_x = Maxf(max_x, rmaxx);
      miny = Minf(miny, y + rminy);
      maxy = Maxf(maxy, y + rmaxy);
      y += lineh * state->line_height;
    }
    string = rows[nrows-1].next;
  }
  state->text_align = oldAlign;
  if (bounds != _NilP) {
    bounds[0] = min_x;
    bounds[1] = miny;
    bounds[2] = max_x;
    bounds[3] = maxy;
  }
}

void ExoTextMetrics(RenderContext* context, FPC* ascender, FPC* descender,
  FPC* lineh) {
  RenderState* state = Exo__getState(context);
  if (context->fs == _NilP || state->font_id < 0) return;
  FPC scale = Exo__getFontScale(state) * context->device_pixel_ratio;
  FPC invscale = 1.0f / scale;
  Exo__setFontState(context, state, scale);
  FPC asc = 0, desc = 0, line = 0;
  context->fs->VerticalMetrics(asc, desc, line);
  if (ascender != _NilP)  *ascender = asc * invscale;
  if (descender != _NilP) *descender = desc * invscale;
  if (lineh != _NilP)     *lineh = line * invscale;
}

}  //< namespace Exo
}  //< namespace CTK
