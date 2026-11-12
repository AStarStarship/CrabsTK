// Copyright AStarship <https://astarship.net>.
#pragma once
// AVCanvas implementation. The headless backend is a software rasterizer for
// nanovg's Exo::RenderParams vtable: it fills/strokes paths as anti-aliased triangles
// into a CImage. The GL backend reuses nanovg's own GL3 renderer (nanovg_gl.h)
// when a context is current — the path calls are identical either way.
#include "Canvas.h"
#include <ASCIICrabs/Array.hxx>  // ArrayFill

namespace CTK {
namespace Exo {

// Headless software rasterizer for Exo::RenderParams

namespace {

/* A filled or stroked path arrives as a triangle soup from nanovg's tessellator.
We render each triangle with barycentric coverage (8x8 supersampling) into the
RGBA8 image. SHeadless (the backend state) is declared in Canvas.h so it can 
live with the canvas. */

void HViewport(void* uptr, FPC, FPC, FPC) {
  SHeadless* s = (SHeadless*)uptr;
  /* The image is already sized; nothing to do on viewport changes. */
  (void)s;
}

void HCancel(void*) {}

void HFlush(void*) {}

/* Tessellate a simple polygon (verts, n) with orientation-aware ear-clipping and rasterize the triangles into the RGBA8 buffer with 8x8 supersampled coverage.
Used by both HFill (fill[]) and HStroke (stroke[]). */
void HRasterizePoly(IUA* pixels, ISC w, ISC h, const Exo::VertexFPC* verts,
                           ISC n, FPC r, FPC g, FPC b, FPC a,
                           ISC* idx, ISC capacity) {
  if (n < 3 || n > capacity) return;
  ISC rem = n;
  for (ISC i = 0; i < n; ++i) idx[i] = i;
  FPC area2 = 0.0f;
  for (ISC i = 0; i < n; ++i) {
    ISC j = (i + 1) % n;
    area2 += verts[i].x * verts[j].y - verts[j].x * verts[i].y;
  }
  ISC sgn = (area2 >= 0.0f) ? 1 : -1;
  ISC guard = 0;
  while (rem >= 3 && guard++ < 4096) {
    BOL ear_cut = false;
    for (ISC e = 0; e < rem && !ear_cut; ++e) {
      ISC a0 = (e + rem - 1) % rem, b0 = e, c0 = (e + 1) % rem;
      ISC ia = idx[a0], ib = idx[b0], ic = idx[c0];
      FPC ax = verts[ia].x, ay = verts[ia].y;
      FPC bx = verts[ib].x, by = verts[ib].y;
      FPC cx = verts[ic].x, cy = verts[ic].y;
      FPC cross = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
      if (sgn * cross <= 1e-6f) continue;  // reflex corner, not an ear
      BOL contains = false;
      for (ISC k = 0; k < rem; ++k) {
        if (k == a0 || k == b0 || k == c0) continue;
        ISC ik = idx[k];
        FPC px = verts[ik].x, py = verts[ik].y;
        FPC d1 = (px - ax) * (by - ay) - (py - ay) * (bx - ax);
        FPC d2 = (px - bx) * (cy - by) - (py - by) * (cx - bx);
        FPC d3 = (px - cx) * (ay - cy) - (py - cy) * (ax - cx);
        BOL neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        BOL pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
        if (!(neg && pos)) { contains = true; break; }
      }
      if (contains) continue;
      // Rasterize triangle (ia, ib, ic).
      {
        FPC x0 = ax, y0 = ay, x1 = bx, y1 = by, x2 = cx, y2 = cy;
        FPC mnx = x0, mxx = x0, mny = y0, mxy = y0;
        if (x1 < mnx) mnx = x1; if (x1 > mxx) mxx = x1;
        if (x2 < mnx) mnx = x2; if (x2 > mxx) mxx = x2;
        if (y1 < mny) mny = y1; if (y1 > mxy) mxy = y1;
        if (y2 < mny) mny = y2; if (y2 > mxy) mxy = y2;
        ISC ix0 = (ISC)mnx, iy0 = (ISC)mny;
        ISC ix1 = (ISC)mxx + 1, iy1 = (ISC)mxy + 1;
        if (ix0 < 0) ix0 = 0; if (iy0 < 0) iy0 = 0;
        if (ix1 > w) ix1 = w; if (iy1 > h) iy1 = h;
        for (ISC py = iy0; py < iy1; ++py) {
          for (ISC px = ix0; px < ix1; ++px) {
            ISC hits = 0;
            for (ISC sy = 0; sy < 8; ++sy) {
              for (ISC sx = 0; sx < 8; ++sx) {
                FPC fx = (FPC)px + (sx + 0.5f) / 8.0f;
                FPC fy = (FPC)py + (sy + 0.5f) / 8.0f;
                FPC d = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
                if (d == 0.0f) continue;
                FPC a1 = ((y1 - y2) * (fx - x2) + (x2 - x1) * (fy - y2)) / d;
                FPC a2 = ((y2 - y0) * (fx - x2) + (x0 - x2) * (fy - y2)) / d;
                FPC a3 = 1.0f - a1 - a2;
                if (a1 >= 0 && a2 >= 0 && a3 >= 0) ++hits;
              }
            }
            if (hits == 0) continue;
            FPC cov = (FPC)hits / 64.0f;
            IUA* dst = &pixels[((IUC)py * (IUC)w + (IUC)px) * 4];
            FPC ia2 = 1.0f - a * cov;
            dst[0] = (IUA)(r * a * cov * 255.0f + (dst[0] / 255.0f) * ia2 * 255.0f);
            dst[1] = (IUA)(g * a * cov * 255.0f + (dst[1] / 255.0f) * ia2 * 255.0f);
            dst[2] = (IUA)(b * a * cov * 255.0f + (dst[2] / 255.0f) * ia2 * 255.0f);
            dst[3] = (IUA)255;
          }
        }
      }
      for (ISC k = e; k + 1 < rem; ++k) idx[k] = idx[k + 1];
      --rem;
      ear_cut = true;
    }
    if (!ear_cut) break;
  }

}

void HFill(void* uptr, Exo::VectorPaint* paint, Exo::CompositeOperationState,
                  Exo::Scissor*, FPC, const FPC*, const Exo::RenderPath* paths,
                  ISC npaths) {
  SHeadless* s = (SHeadless*)uptr;
  FPC r = paint->inner_color.r, g = paint->inner_color.g,
        b = paint->inner_color.b, a = paint->inner_color.a;
  for (ISC p = 0; p < npaths; ++p)
    HRasterizePoly(s->pixels, s->w, s->h, paths[p].fill, paths[p].nfill, r, g, b, a,
                   s->indices, s->index_count);
}

/* Supersampled coverage fill of one triangle: 8x8 subsamples per pixel give the
edge antialiasing the seam tests look for. */
void HRasterizeTriangle(Exo::VertexFPC* pixels_unused, ISC w, ISC h,
                               IUA* pixels, const Exo::VertexFPC& v0,
                               const Exo::VertexFPC& v1,
                               const Exo::VertexFPC& v2,
                               FPC r, FPC g, FPC b, FPC a) {
  FPC min_x = v0.x, max_x = v0.x, miny = v0.y, maxy = v0.y;
  if (v1.x < min_x) min_x = v1.x;
  if (v1.x > max_x) max_x = v1.x;
  if (v2.x < min_x) min_x = v2.x;
  if (v2.x > max_x) max_x = v2.x;
  if (v1.y < miny) miny = v1.y;
  if (v1.y > maxy) maxy = v1.y;
  if (v2.y < miny) miny = v2.y;
  if (v2.y > maxy) maxy = v2.y;

  const FPC d = (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y);
  if (d == 0.0F) return;

  ISC ix0 = ISC(min_x), iy0 = ISC(miny);
  ISC ix1 = ISC(max_x) + 1, iy1 = ISC(maxy) + 1;
  if (ix0 < 0) ix0 = 0;
  if (iy0 < 0) iy0 = 0;
  if (ix1 > w) ix1 = w;
  if (iy1 > h) iy1 = h;

  for (ISC py = iy0; py < iy1; ++py) {
    for (ISC px = ix0; px < ix1; ++px) {
      ISC hits = 0;
      for (ISC sy = 0; sy < 8; ++sy) {
        for (ISC sx = 0; sx < 8; ++sx) {
          const FPC fx = FPC(px) + (FPC(sx) + 0.5F) / 8.0F;
          const FPC fy = FPC(py) + (FPC(sy) + 0.5F) / 8.0F;
          const FPC a1 =
              ((v1.y - v2.y) * (fx - v2.x) + (v2.x - v1.x) * (fy - v2.y)) / d;
          const FPC a2 =
              ((v2.y - v0.y) * (fx - v2.x) + (v0.x - v2.x) * (fy - v2.y)) / d;
          const FPC a3 = 1.0F - a1 - a2;
          if (a1 >= 0.0F && a2 >= 0.0F && a3 >= 0.0F) ++hits;
        }
      }
      if (hits == 0) continue;
      const FPC cov = FPC(hits) / 64.0F;
      IUA* dst = &pixels[((IUC)py * (IUC)w + (IUC)px) * 4];
      const FPC ia = 1.0F - a * cov;
      dst[0] = IUA(r * a * cov * 255.0F + (FPC(dst[0]) / 255.0F) * ia * 255.0F);
      dst[1] = IUA(g * a * cov * 255.0F + (FPC(dst[1]) / 255.0F) * ia * 255.0F);
      dst[2] = IUA(b * a * cov * 255.0F + (FPC(dst[2]) / 255.0F) * ia * 255.0F);
      dst[3] = 255;
    }
  }
}

void HStroke(void* uptr, Exo::VectorPaint* paint, Exo::CompositeOperationState,
                    Exo::Scissor*, FPC, FPC, const Exo::RenderPath* paths,
                    ISC npaths) {
  /* nanovg expands a stroke into a TRIANGLE STRIP: two vertices per segment,
  the outer edge then the inner edge (Exo__expandStroke emits them in pairs).
  The GL backend draws that strip with GL_TRIANGLE_STRIP. Rasterising it as a
  polygon instead turns every stroke into a filled fan across its vertices,
  which is exactly what the arm links looked like. Walk the strip. */
  SHeadless* s = (SHeadless*)uptr;
  FPC r = paint->inner_color.r, g = paint->inner_color.g,
      b = paint->inner_color.b, a = paint->inner_color.a;
  for (ISC p = 0; p < npaths; ++p) {
    const Exo::VertexFPC* strip = paths[p].stroke;
    const ISC count = paths[p].nstroke;
    for (ISC i = 0; i + 3 < count; i += 2) {
      HRasterizeTriangle(_NilP, s->w, s->h, s->pixels, strip[i], strip[i + 1],
                         strip[i + 2], r, g, b, a);
      HRasterizeTriangle(_NilP, s->w, s->h, s->pixels, strip[i + 1],
                         strip[i + 2], strip[i + 3], r, g, b, a);
    }
  }
}

void HTriangles(void* uptr, Exo::VectorPaint* paint,
                       Exo::CompositeOperationState, Exo::Scissor*,
                       const Exo::VertexFPC* verts, ISC nverts, FPC) {
  // RenderTriangles hands us tessellated triangles as (x, y) triples.
  SHeadless* s = (SHeadless*)uptr;
  FPC r = paint->inner_color.r, g = paint->inner_color.g,
      b = paint->inner_color.b, a = paint->inner_color.a;
  for (ISC i = 0; i + 2 < nverts; i += 3) {
    HRasterizeTriangle(_NilP, s->w, s->h, s->pixels, verts[i], verts[i + 1],
                       verts[i + 2], r, g, b, a);
  }
}

ISC HCreate(void* uptr) {
  SHeadless* s = (SHeadless*)uptr;
  (void)s;
  return 1;
}
ISC HCreateTexture(void*, ISC, ISC, ISC, ISC, const IUA*) { return -1; }
ISC HDeleteTexture(void*, ISC) { return 0; }
ISC HUpdateTexture(void*, ISC, ISC, ISC, ISC, ISC, const IUA*) { return 0; }
ISC HGetTextureSize(void*, ISC, ISC* w, ISC* h) { *w = 0; *h = 0; return 0; }
void HDelete(void* uptr) {
  SHeadless* s = (SHeadless*)uptr;
  s->pixels = nullptr;  /* AVCanvas owns the pixel buffer; it deletes it. */
}

}  //< namespace

// AVCanvas

BOL AVCanvas::Init(ARoom& room, ISC width, ISC height, Exo::VColor bg) {
  if (Ready() || room_ != _NilP) return false;
  if (room.WallCount() >= room.WallCountMax) return false;
  if (!Init_(width, height, bg)) return false;
  if (room.AddWall(&wall_) == _NilP) {
    Release();
    return false;
  }
  room_ = &room;
  return true;
}

BOL AVCanvas::Init_(ISC width, ISC height, Exo::VColor bg) {
  // Bound before multiplying, including the Wall's signed byte count.
  if (width <= 0 || height <= 0 || width > 16384 || height > 16384)
    return false;
  ISW pixels_bytes = ISW(width) * ISW(height) * 4;
  ISW scratch_bytes = ISW(RenderConfigDefault.verts) * sizeof(ISC);
  _::TWallInit<>(&wall_, ExoWallBytesMin + pixels_bytes + scratch_bytes,
                ExoWallBlocks() + 34);
  auto* root = _::TWallRoot<>(&wall_);
  ISR pixels = _::TListAlloc<ISS, ISR, DTB>(root, DTB(_::_IUD),
                                         ISS(pixels_bytes));
  ISR scratch = _::TListAlloc<ISS, ISR, DTB>(root, DTB(_::_IUD),
                                          ISS(scratch_bytes));
  if (pixels < 0 || scratch < 0) {
    Release();
    return false;
  }
  img_ = {static_cast<IUA*>(_::TListValue<ISS, ISR, DTB>(root, pixels)),
          width, height, PixelFormat::RGBA8};
  headless_.indices = static_cast<ISC*>(
      _::TListValue<ISS, ISR, DTB>(root, scratch));
  headless_.index_count = RenderConfigDefault.verts;
  width_ = width;
  height_ = height;
  Exo::Fill(img_, bg);
  headless_.img = &img_;
  headless_.pixels = img_.data;
  headless_.w = width;
  headless_.h = height;
  Exo::RenderParams p;
  ::_::ArrayFill(&p, (ISW)sizeof(p));
  p.userPtr = &headless_;
  p.edgeAntiAlias = 1;
  p.renderCreate = HCreate;
  p.renderCreateTexture = HCreateTexture;
  p.renderDeleteTexture = HDeleteTexture;
  p.renderUpdateTexture = HUpdateTexture;
  p.renderGetTextureSize = HGetTextureSize;
  p.renderViewport = HViewport;
  p.renderCancel = HCancel;
  p.renderFlush = HFlush;
  p.renderFill = HFill;
  p.renderStroke = HStroke;
  p.renderTriangles = HTriangles;
  p.renderDelete = HDelete;
  ctx_ = Exo::ExoCreateInternal(&p, &wall_);
  if (!Ready()) {
    Release();
    return false;
  }
  return true;
}

AVCanvas::~AVCanvas() { Release(); }

void AVCanvas::Release() {
  if (ctx_ != _NilP) Exo::ExoDeleteInternal(ctx_);
  ctx_ = _NilP;
  if (room_ != _NilP) {
    for (ISN i = 0; i < room_->WallCount(); ++i) {
      if (room_->GetWall(i) == &wall_) {
        room_->RemoveWall(i);
        break;
      }
    }
  }
  room_ = _NilP;
  _::TWallRelease<>(&wall_);
  img_ = {};
  headless_ = {};
  width_ = height_ = 0;
}

void AVCanvas::BeginFrame() {
  Exo::ExoBeginFrame(ctx_, (FPC)width_, (FPC)height_, 1.0f);
}

void AVCanvas::EndFrame() {
  Exo::ExoEndFrame(ctx_);
}

void AVCanvas::BeginPath() { Exo::ExoBeginPath(ctx_); }
void AVCanvas::MoveTo(FPD x, FPD y) { Exo::ExoMoveTo(ctx_, (FPC)x, (FPC)y); }
void AVCanvas::LineTo(FPD x, FPD y) { Exo::ExoLineTo(ctx_, (FPC)x, (FPC)y); }
void AVCanvas::Circle(FPD cx, FPD cy, FPD r) {
  Exo::ExoCircle(ctx_, (FPC)cx, (FPC)cy, (FPC)r);
}
void AVCanvas::ClosePath() { Exo::ExoClosePath(ctx_); }

void AVCanvas::Fill(Exo::VColor c) {
  Exo::ExoFillColor(ctx_, Exo::ExoRGBA(c.r, c.g, c.b, c.a));
  Exo::ExoFill(ctx_);
}

void AVCanvas::Stroke(Exo::VColor c, FPD width) {
  Exo::ExoStrokeColor(ctx_, Exo::ExoRGBA(c.r, c.g, c.b, c.a));
  Exo::ExoStrokeWidth(ctx_, (FPC)width);
  Exo::ExoStroke(ctx_);
}

void AVCanvas::Clear(Exo::VColor c) {
  for (ISC i = 0; i < (ISC)width_ * (ISC)height_; ++i) {
    img_.data[i * 4] = c.r;
    img_.data[i * 4 + 1] = c.g;
    img_.data[i * 4 + 2] = c.b;
    img_.data[i * 4 + 3] = c.a;
  }
}

}  //< namespace Exo
}  //< namespace CTK
