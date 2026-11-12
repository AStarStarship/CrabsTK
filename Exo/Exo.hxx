// Copyright AStarship <https://astarship.net>.
#include "Exo.h"
namespace CTK {
namespace Exo {

// Exo.hxx — implementation of the Exo software rasterizer.
// Self-contained: all math from ASCIICrabs Math.hxx (no libm), all pixel
// writes through the CrabsTK Image layer (CImage / the CTK::Exo raster primitives). No stdlib.

// Matrix algebra

TMat4 Multiply(const TMat4& a, const TMat4& b) {
  TMat4 r;
  for (ISC col = 0; col < 4; ++col) {
    for (ISC row = 0; row < 4; ++row) {
      FPD s = 0.0;
      for (ISC k = 0; k < 4; ++k) s += a.m[k * 4 + row] * b.m[col * 4 + k];
      r.m[col * 4 + row] = s;
    }
  }
  return r;
}

TMat4 Perspective(FPD fovY, FPD aspect, FPD near_, FPD far_) {
  TMat4 m = TMat4::Identity();
  const FPD f = 1.0 / Tan(fovY * 0.5);
  // Column-major storage m[col*4+row]. GL perspective:
  //   row0: [f/aspect, 0, 0, 0]
  //   row1: [0, f, 0, 0]
  //   row2: [0, 0, (far+near)/(near-far), 2*far*near/(near-far)]
  //   row3: [0, 0, -1, 0]        (w' = -z_view)
  m.m[0]  = f / aspect;   // row0,col0
  m.m[5]  = f;            // row1,col1
  m.m[10] = (far_ + near_) / (near_ - far_);  // row2,col2
  m.m[14] = (2.0 * far_ * near_) / (near_ - far_);  // row2,col3 (z-translation)
  m.m[11] = -1.0;         // row3,col2 (w' = -z)
  m.m[15] = 0.0;          // row3,col3
  return m;
}

TMat4 LookAt(const FPD3& eye, const FPD3& target, const FPD3& up) {
  // Canonical GL look-at: camera looks DOWN -Z in view space.
  //   f = normalize(eye - target)   (points from target toward camera)
  //   s = normalize(cross(f, up))   (view right)
  //   u = cross(s, f)               (view up)
  // View matrix rotation rows = [s; u; -f], translation = [-s.e; -u.e; f.e].
  // A point in front of the camera maps to view z < 0, so the perspective
  // matrix (m[15] = -1) gives w' = -z_view > 0 for in-front geometry.
  FPD fx = eye.x - target.x, fy = eye.y - target.y, fz = eye.z - target.z;
  FPD flen = SquareRoot(fx * fx + fy * fy + fz * fz);
  FPD finv = flen > 0.0 ? 1.0 / flen : 0.0;
  fx *= finv; fy *= finv; fz *= finv;

  // s = f x up
  FPD sx = fy * up.z - fz * up.y;
  FPD sy = fz * up.x - fx * up.z;
  FPD sz = fx * up.y - fy * up.x;
  FPD slen = SquareRoot(sx * sx + sy * sy + sz * sz);
  FPD sinv = slen > 0.0 ? 1.0 / slen : 0.0;
  sx *= sinv; sy *= sinv; sz *= sinv;

  // u = s x f
  FPD ux = sy * fz - sz * fy;
  FPD uy = sz * fx - sx * fz;
  FPD uz = sx * fy - sy * fx;

  // View matrix (rotation rows [s; u; -f] + translation). Column-major:
  // the translation lives in column 3 (m[12]=x, m[13]=y, m[14]=z).
  TMat4 m = TMat4::Identity();
  m.m[0] = sx;  m.m[4] = sy;  m.m[8]  = sz;
  m.m[1] = ux;  m.m[5] = uy;  m.m[9]  = uz;
  m.m[2] = -fx; m.m[6] = -fy; m.m[10] = -fz;
  m.m[12] = -(sx * eye.x + sy * eye.y + sz * eye.z);
  m.m[13] = -(ux * eye.x + uy * eye.y + uz * eye.z);
  m.m[14] = (fx * eye.x + fy * eye.y + fz * eye.z);
  m.m[15] = 1.0;
  return m;
}

TMat4 RotateX(FPD rad) {
  FPD c = Cosine(rad), s = Sine(rad);
  TMat4 m = TMat4::Identity();
  m.m[5] = c;  m.m[6] = s;
  m.m[9] = -s; m.m[10] = c;
  return m;
}

TMat4 RotateY(FPD rad) {
  FPD c = Cosine(rad), s = Sine(rad);
  TMat4 m = TMat4::Identity();
  m.m[0] = c;  m.m[2] = -s;
  m.m[8] = s;  m.m[10] = c;
  return m;
}

TMat4 RotateZ(FPD rad) {
  FPD c = Cosine(rad), s = Sine(rad);
  TMat4 m = TMat4::Identity();
  m.m[0] = c;  m.m[1] = s;
  m.m[4] = -s; m.m[5] = c;
  return m;
}

TMat4 Translate(FPD x, FPD y, FPD z) {
  TMat4 m = TMat4::Identity();
  m.m[3] = x; m.m[7] = y; m.m[11] = z;
  return m;
}

TMat4 Scale(FPD sx, FPD sy, FPD sz) {
  TMat4 m = TMat4::Identity();
  m.m[0] = sx; m.m[5] = sy; m.m[10] = sz;
  return m;
}

FPD3 TransformPoint(const TMat4& M, const FPD3& p) {
  // v' = M * (x,y,z,1); apply perspective divide by w.
  FPD x = p.x, y = p.y, z = p.z, w = 1.0;
  FPD ox = M.m[0] * x + M.m[4] * y + M.m[8] * z + M.m[12];
  FPD oy = M.m[1] * x + M.m[5] * y + M.m[9] * z + M.m[13];
  FPD oz = M.m[2] * x + M.m[6] * y + M.m[10] * z + M.m[14];
  FPD ow = M.m[3] * x + M.m[7] * y + M.m[11] * z + M.m[15];
  FPD iw = Absolute(ow) > 1.0e-12 ? 1.0 / ow : 0.0;
  return FPD3(ox * iw, oy * iw, oz * iw);
}

// Frame

void ClearFrame(TFrame& f, const VColor& bg) {
  Fill(*f.img, bg);
  IUC px = (IUC)f.width * (IUC)f.height;
  for (IUC i = 0; i < px; ++i) f.zbuf[i] = f.zfar;
}

// Projection

BOL ProjectPoint(const TMat4& proj, const FPD3& world, ISC width, ISC height,
                 FPD* sx, FPD* sy, FPD* depth) {
  // Transform to clip space WITHOUT the perspective divide, so we can test
  // w' (the homogeneous w) for the near-plane check.
  FPD x = world.x, y = world.y, z = world.z;
  FPD cx = proj.m[0] * x + proj.m[4] * y + proj.m[8]  * z + proj.m[12];
  FPD cy = proj.m[1] * x + proj.m[5] * y + proj.m[9]  * z + proj.m[13];
  FPD cz = proj.m[2] * x + proj.m[6] * y + proj.m[10] * z + proj.m[14];
  FPD cw = proj.m[3] * x + proj.m[7] * y + proj.m[11] * z + proj.m[15];
  // Near-plane check (measured, this camera basis): in-front geometry yields
  // cw < 0 with the canonical look-at + perspective pair. Reject at/behind
  // the near plane: cw >= 0.
  if (Absolute(cw) < 1.0e-12 || cw >= 0.0) return false;
  FPD iw = 1.0 / cw;
  FPD nx = cx * iw, ny = cy * iw, nz = cz * iw;
  // Reject only on the far plane (nz beyond the far clip). x/y may exceed
  // [-1,1] — those vertices are off-screen but the triangle's visible edges
  // must still rasterize; the bounding-box loop + WritePixel clip them.
  // (Full near-plane triangle clipping is a later pass; for now we rely on
  // the cw sign test to drop vertices behind the near plane.)
  if (nz < -1.0) return false;
  // NDC -> pixel. x right, y down (screen convention).
  *sx = (nx * 0.5 + 0.5) * (FPD)width;
  *sy = (1.0 - (ny * 0.5 + 0.5)) * (FPD)height;
  // Depth in [0,1] (near=0, far=1) for the z-buffer.
  *depth = nz * 0.5 + 0.5;
  return true;
}

// Triangle rasterizer

/* Edge function: positive if the point (px,py) is on the inside of the
   directed edge a->b. Standard 2D cross product. */
FPD Edge(FPD ax, FPD ay, FPD bx, FPD by, FPD px, FPD py) {
  return (bx - ax) * (py - ay) - (by - ay) * (px - ax);
}

void WritePixel(TFrame& f, ISC x, ISC y, FPD r, FPD g, FPD b) {
  if (x < 0 || y < 0 || x >= f.width || y >= f.height) return;
  ISC idx = (y * f.width + x) * 4;
  // Premultiplied-ish "over" blend with a fully-opaque source is just a
  // store; we store opaque (a=255) triangle fragments.
  f.img->data[idx]     = (IUA)(r < 0.0 ? 0.0 : (r > 255.0 ? 255.0 : r));
  f.img->data[idx + 1] = (IUA)(g < 0.0 ? 0.0 : (g > 255.0 ? 255.0 : g));
  f.img->data[idx + 2] = (IUA)(b < 0.0 ? 0.0 : (b > 255.0 ? 255.0 : b));
  f.img->data[idx + 3] = 255;
}

void Triangle(TFrame& f,
              FPD ax, FPD ay, FPD az, FPD ar, FPD ag, FPD ab,
              FPD bx, FPD by, FPD bz, FPD br, FPD bg, FPD bb,
              FPD cx, FPD cy, FPD cz, FPD cr, FPD cg, FPD cb) {
  // Bounding box of the triangle in screen space.
  FPD minx = Ceiling(Min(ax, Min(bx, cx)));
  FPD maxx = Floor(Max(ax, Max(bx, cx)));
  FPD miny = Ceiling(Min(ay, Min(by, cy)));
  FPD maxy = Floor(Max(ay, Max(by, cy)));
  if (minx > maxx || miny > maxy) return;

  // Total area (2x). Its sign tells us winding. In SCREEN space (y-down,
  // after the NDC->pixel flip), VISIBLE (front-facing) triangles are CCW and
  // have POSITIVE area; back-facing are CW (negative). We cull back faces —
  // this also drops the giant inflated triangles that near-plane clipping
  // produces for faces extending behind the near plane.
  FPD area = Edge(ax, ay, bx, by, cx, cy);
  if (area <= 0.0) return;  // back-facing (or degenerate) -> cull
  FPD invArea = 1.0 / area;

  ISC x0 = (ISC)minx, x1 = (ISC)maxx;
  ISC y0 = (ISC)miny, y1 = (ISC)maxy;
  // Clamp the bounding box to the frame. A vertex clipped exactly onto the
  // near plane projects to (nearly) infinity; without this clamp the box
  // overflows the ISC cast and the loop indexes the z-buffer out of bounds.
  if (x0 < 0) x0 = 0;
  if (y0 < 0) y0 = 0;
  if (x1 > f.width - 1) x1 = f.width - 1;
  if (y1 > f.height - 1) y1 = f.height - 1;
  if (x0 > x1 || y0 > y1) return;
  for (ISC py = y0; py <= y1; ++py) {
    for (ISC px = x0; px <= x1; ++px) {
      // Sample at pixel center.
      FPD sx = (FPD)px + 0.5, sy = (FPD)py + 0.5;
      // Barycentric weights (each opposite its vertex).
      FPD w0 = Edge(bx, by, cx, cy, sx, sy) * invArea;  // weight for A
      FPD w1 = Edge(cx, cy, ax, ay, sx, sy) * invArea;  // weight for B
      FPD w2 = Edge(ax, ay, bx, by, sx, sy) * invArea;  // weight for C
      // Inside the triangle if all weights are non-negative.
      if (w0 < 0.0 || w1 < 0.0 || w2 < 0.0) continue;

      // Interpolate depth and color.
      FPD z = w0 * az + w1 * bz + w2 * cz;
      // Depth test (canonical GL): NDC.z maps near=+1, far=-1, so LARGER z is
      // CLOSER. Keep the fragment if z >= stored (as close or closer).
      IUC zidx = (IUC)py * (IUC)f.width + (IUC)px;
      if (z < f.zbuf[zidx]) continue;
      f.zbuf[zidx] = z;

      FPD r = w0 * ar + w1 * br + w2 * cr;
      FPD g = w0 * ag + w1 * bg + w2 * cg;
      FPD b = w0 * ab + w1 * bb + w2 * cb;
      WritePixel(f, px, py, r, g, b);
    }
  }
}

// Clip-space near-plane clipping
//
// The perspective divide is undefined at/behind the near plane (w' -> 0),
// which produces huge NDCs that overflow the rasterizer. The fix is the
// standard one: work in clip space (NO divide), clip the polygon against the
// near plane (w' = 0), and only then divide. In this camera basis (canonical
// GL look-at + perspective, m[15] = -1) geometry IN FRONT of the camera has
// cw < 0, so we keep the cw < 0 side and discard cw > 0.

// Clip-space vertex: homogeneous (no divide) + the interpolated attributes.
struct TClipVertex {
  FPD cx, cy, cz, cw;   // clip coordinates (cw is the homogeneous w)
  FPD r, g, b;          // color 0-255
};

// Near-plane margin: we clip to cw < NEAR_EPS so interpolated vertices have a
// finite nonzero cw (avoids divide-by-zero) while staying effectively on the
// near plane.
const FPD NEAR_EPS = -1.0e-6;

// World point -> clip space (no perspective divide).
TClipVertex ToClipSpace(const TMat4& proj, const FPD3& w, const ISB4& col) {
  TClipVertex v;
  v.cx = proj.m[0] * w.x + proj.m[4] * w.y + proj.m[8]  * w.z + proj.m[12];
  v.cy = proj.m[1] * w.x + proj.m[5] * w.y + proj.m[9]  * w.z + proj.m[13];
  v.cz = proj.m[2] * w.x + proj.m[6] * w.y + proj.m[10] * w.z + proj.m[14];
  v.cw = proj.m[3] * w.x + proj.m[7] * w.y + proj.m[11] * w.z + proj.m[15];
  v.r  = (FPD)col.x;
  v.g  = (FPD)col.y;
  v.b  = (FPD)col.z;
  return v;
}

// Linearly interpolate a clip vertex across the near plane. `a` is the
// outside vertex (cw > NEAR_EPS), `b` the inside vertex (cw < NEAR_EPS). The
// crossing point is where cw == NEAR_EPS: a + t * (b - a),
// t = (cw_a - NEAR_EPS) / (cw_a - cw_b).
TClipVertex InterpNear(const TClipVertex& a, const TClipVertex& b) {
  TClipVertex o;
  FPD t = (a.cw - NEAR_EPS) / (a.cw - b.cw);
  o.cx = a.cx + t * (b.cx - a.cx);
  o.cy = a.cy + t * (b.cy - a.cy);
  o.cz = a.cz + t * (b.cz - a.cz);
  o.cw = NEAR_EPS;                 // finite nonzero, on the near plane
  o.r  = a.r  + t * (b.r  - a.r);
  o.g  = a.g  + t * (b.g  - a.g);
  o.b  = a.b  + t * (b.b  - a.b);
  return o;
}

// Sutherland-Hodgman clip of a triangle (in[0..n-1]) against the near
// plane, keeping the cw < NEAR_EPS side. Out vertices go into `out`
// (capacity `outCap`). Returns the count written. A triangle can produce up
// to 4 clipped vertices (2 original + 2 new), so outCap >= 4 is required.
//
// We keep cw < NEAR_EPS (a small negative margin) rather than cw < 0 so that
// the interpolated crossing vertex has cw = NEAR_EPS, a finite nonzero value.
// That gives it a correct, finite screen position after the divide, instead
// of dividing by zero at cw == 0.
IUD ClipNear(const TClipVertex* in, IUD n, TClipVertex* out, IUD outCap) {
  IUD oc = 0;
  const TClipVertex* prev = &in[n - 1];
  for (IUD i = 0; i < n; ++i) {
    const TClipVertex* cur = &in[i];
    BOL prevIn = (prev->cw < NEAR_EPS);
    BOL curIn  = (cur->cw < NEAR_EPS);
    if (curIn) {
      if (!prevIn) out[oc++] = InterpNear(*prev, *cur);  // entering
      out[oc++] = *cur;
    } else if (prevIn) {
      out[oc++] = InterpNear(*cur, *prev);               // leaving
    }
    if (oc >= outCap) break;
    prev = cur;
  }
  return oc;
}

// Clip-space vertex -> screen (x,y) + depth, after the near plane is gone.
BOL ProjectClip(const TClipVertex& v, ISC width, ISC height,
                       FPD* sx, FPD* sy, FPD* depth) {
  // After clipping, every vertex is at or in front of the near plane. cw is
  // <= 0 here; the crossing vertices have cw == 0 exactly, which would
  // divide by zero. Nudge those infinitesimally inside so the divide is
  // finite; the resulting pixel is far off-screen and the bounding-box +
  // WritePixel clip discard it.
  FPD cw = v.cw;
  if (cw >= 0.0) cw = -1.0e-12;
  FPD iw = 1.0 / cw;
  FPD nx = v.cx * iw, ny = v.cy * iw, nz = v.cz * iw;
  // Far plane: reject if beyond it (nz < -1 in this basis). Off-screen x/y is
  // fine — the rasterizer clips it.
  if (nz < -1.0) return false;
  // Cap the off-screen NDC x/y to a finite range. A vertex clipped exactly
  // onto the near plane (cw ~ 0) would otherwise divide to ~1e12 and flood the
  // frame with one interpolated gradient. +/-4 NDC is far outside the +/-1
  // frame but finite, so the bounding box stays bounded and the giant
  // triangle rasterizes only the sliver that is actually inside the frame.
  if (nx > 4.0) nx = 4.0; else if (nx < -4.0) nx = -4.0;
  if (ny > 4.0) ny = 4.0; else if (ny < -4.0) ny = -4.0;
  *sx = (nx * 0.5 + 0.5) * (FPD)width;
  *sy = (1.0 - (ny * 0.5 + 0.5)) * (FPD)height;
  *depth = nz;   // raw NDC z (near=+1, far=-1); larger = closer, for z-buffer
  return true;
}

// Indexed batch

void DrawIndexed(TFrame& f, const TMat4& proj, const TVertex* verts,
                 IUD vertCount, const ISC* indices, IUD indexCount) {
  (void)vertCount;
  for (IUD i = 0; i + 2 < indexCount; i += 3) {
    ISC ia = indices[i], ib = indices[i + 1], ic = indices[i + 2];
    if (ia < 0 || ia >= (ISC)vertCount) continue;
    if (ib < 0 || ib >= (ISC)vertCount) continue;
    if (ic < 0 || ic >= (ISC)vertCount) continue;

    // 1. Transform the three corners to clip space (no divide).
    TClipVertex tri[3];
    tri[0] = ToClipSpace(proj, verts[ia].p, verts[ia].c);
    tri[1] = ToClipSpace(proj, verts[ib].p, verts[ib].c);
    tri[2] = ToClipSpace(proj, verts[ic].p, verts[ic].c);

    // 2. Clip against the near plane (cw < 0 side).
    TClipVertex clip[4];
    IUD cc = ClipNear(tri, 3, clip, 4);
    if (cc < 3) continue;   // fully behind the near plane -> nothing to draw

    // 3. Project the clipped vertices to screen space (finite now).
    FPD sx[4], sy[4], dz[4], sr[4], sg[4], sb[4];
    IUD pc = 0;
    for (IUD k = 0; k < cc; ++k) {
      if (ProjectClip(clip[k], f.width, f.height, &sx[pc], &sy[pc], &dz[pc])) {
        sr[pc] = clip[k].r; sg[pc] = clip[k].g; sb[pc] = clip[k].b;
        ++pc;
      }
    }
    if (pc < 3) continue;

    // 4. Fan-triangulate the clipped polygon (0..1..k) and rasterize.
    for (IUD k = 1; k + 1 < pc; ++k) {
      Triangle(f,
               sx[0],     sy[0],     dz[0],     sr[0],     sg[0],     sb[0],
               sx[k],     sy[k],     dz[k],     sr[k],     sg[k],     sb[k],
               sx[k + 1], sy[k + 1], dz[k + 1], sr[k + 1], sg[k + 1], sb[k + 1]);
    }
  }
}

// Textured quad blit (display target)
// Maps a CImage texture over the whole frame. uv = center + pos*scale +
// pan, where pos is the full-screen quad coordinate in [-1,1]. This is
// the "WebGL texture quad" done in C++: the browser could do it too, but
// doing it here keeps the display transform in the renderer.

void BlitTexture(TFrame& f, const CImage& tex, FPD scale, FPD panX, FPD panY) {
  ISC W = f.width, H = f.height;
  for (ISC y = 0; y < H; ++y) {
    for (ISC x = 0; x < W; ++x) {
      // Full-screen quad coord in [-1,1], x right, y down.
      FPD px = (2.0 * (FPD)x + 1.0) / (FPD)W - 1.0;
      FPD py = (2.0 * (FPD)y + 1.0) / (FPD)H - 1.0;
      // uv (0..1), y flipped so texture top is at screen top.
      FPD u = 0.5 + px * scale + panX;
      FPD v = 0.5 - py * scale - panY;
      // Nearest-sample the texture; clamp to edges.
      ISC tx = (ISC)(u * (FPD)tex.width);
      ISC ty = (ISC)(v * (FPD)tex.height);
      if (tx < 0) tx = 0;
      if (ty < 0) ty = 0;
      if (tx >= tex.width) tx = tex.width - 1;
      if (ty >= tex.height) ty = tex.height - 1;
      IUC sidx = ((IUC)ty * (IUC)tex.width + (IUC)tx) * (IUC)PixelBytes(tex.format);
      IUC didx = ((IUC)y * (IUC)W + (IUC)x) * 4;
      f.img->data[didx]     = tex.data[sidx];
      f.img->data[didx + 1] = tex.data[sidx + 1];
      f.img->data[didx + 2] = tex.data[sidx + 2];
      f.img->data[didx + 3] = 255;
    }
  }
}

// Scene layer

TScene::TScene()
    : verts(nullptr), vert_count(0), vert_used(0),
      indices(nullptr), idx_count(0), idx_used(0),
      eye(0.0, 0.0, 6.0), target(0.0, 0.0, 0.0), up(0.0, 1.0, 0.0),
      fov(1.0), aspect(1.0) {}

TScene::~TScene() {
  delete[] verts;
  delete[] indices;
}

// Grow the vertex/index arrays if needed. Returns false if the requested
// count cannot fit (the scene is finite; the caller checks).
BOL SceneGrowVerts(TScene& s, IUD need) {
  if (need <= s.vert_count) return true;
  IUD newcap = s.vert_count ? s.vert_count : 64;
  while (newcap < need) newcap *= 2;
  TVertex* nv = new TVertex[newcap]();
  for (IUD i = 0; i < s.vert_used; ++i) nv[i] = s.verts[i];
  delete[] s.verts;
  s.verts = nv;
  s.vert_count = newcap;
  return true;
}

BOL SceneGrowIdx(TScene& s, IUD need) {
  if (need <= s.idx_count) return true;
  IUD newcap = s.idx_count ? s.idx_count : 64;
  while (newcap < need) newcap *= 2;
  ISC* ni = new ISC[newcap]();
  for (IUD i = 0; i < s.idx_used; ++i) ni[i] = s.indices[i];
  delete[] s.indices;
  s.indices = ni;
  s.idx_count = newcap;
  return true;
}

ISC TScene::AddQuad(const FPD3& pos, const FPD2& size, const ISB4& color,
                    const FPD2& t0, const FPD2& t1, ISC axis) {
  if (!SceneGrowVerts(*this, vert_used + 4) ||
      !SceneGrowIdx(*this, idx_used + 6)) return -1;
  const FPD hw = size.x * 0.5, hh = size.y * 0.5;
  // Four corners of the quad in its plane, centered at pos.
  // axis 0 = XY plane (z=0, the "2D" case): corners offset in x and y.
  // axis 1 = XZ plane (y=0, floor): offset in x and z.
  // axis 2 = YZ plane (x=0, wall): offset in y and z.
  FPD3 c[4];
  if (axis == 1) {
    c[0] = {pos.x - hw, pos.y, pos.z - hh};
    c[1] = {pos.x + hw, pos.y, pos.z - hh};
    c[2] = {pos.x + hw, pos.y, pos.z + hh};
    c[3] = {pos.x - hw, pos.y, pos.z + hh};
  } else if (axis == 2) {
    c[0] = {pos.x, pos.y - hh, pos.z - hw};
    c[1] = {pos.x, pos.y + hh, pos.z - hw};
    c[2] = {pos.x, pos.y + hh, pos.z + hw};
    c[3] = {pos.x, pos.y - hh, pos.z + hw};
  } else {
    // axis 0 = XY (z=0)
    c[0] = {pos.x - hw, pos.y - hh, pos.z};
    c[1] = {pos.x + hw, pos.y - hh, pos.z};
    c[2] = {pos.x + hw, pos.y + hh, pos.z};
    c[3] = {pos.x - hw, pos.y + hh, pos.z};
  }
  // Texture coords: t0 at c0, t1 at c2 (diagonal); t/v split for c1/c3.
  FPD2 tc[4] = {
      {t0.x, t0.y}, {t1.x, t0.y}, {t1.x, t1.y}, {t0.x, t1.y}};
  const IUD base = vert_used;
  for (IUD i = 0; i < 4; ++i) {
    verts[vert_used].p = c[i];
    verts[vert_used].c = color;
    verts[vert_used].t = tc[i];
    ++vert_used;
  }
  // Two triangles: (0,1,2) and (0,2,3). Winding chosen so the quad faces
  // +Z (toward a camera at +Z) for axis 0; the back-face cull in Triangle
  // drops the reverse winding.
  const ISC tri[6] = {(ISC)base, (ISC)base + 1, (ISC)base + 2,
                      (ISC)base, (ISC)base + 2, (ISC)base + 3};
  for (IUD i = 0; i < 6; ++i) indices[idx_used++] = tri[i];
  return (ISC)base;
}

ISC TScene::AddCube(const FPD3& pos, const ISB4& color) {
  // A unit cube centered at pos = 6 quads (one per face). Each face is a
  // quad in its plane. We add them as 6 quads via AddQuad, but each face
  // needs its own plane axis and orientation. For simplicity, build the 8
  // cube corners directly and emit 12 triangles (2 per face) with the
  // given color. This is the demo/test convenience; the general case is
  // AddQuad for textured faces.
  if (!SceneGrowVerts(*this, vert_used + 8) ||
      !SceneGrowIdx(*this, idx_used + 36)) return -1;
  const FPD s = 0.5;
  FPD3 corner[8] = {
      {pos.x - s, pos.y - s, pos.z - s}, {pos.x + s, pos.y - s, pos.z - s},
      {pos.x + s, pos.y + s, pos.z - s}, {pos.x - s, pos.y + s, pos.z - s},
      {pos.x - s, pos.y - s, pos.z + s}, {pos.x + s, pos.y - s, pos.z + s},
      {pos.x + s, pos.y + s, pos.z + s}, {pos.x - s, pos.y + s, pos.z + s}};
  const IUD base = vert_used;
  for (IUD i = 0; i < 8; ++i) {
    verts[vert_used].p = corner[i];
    verts[vert_used].c = color;
    verts[vert_used].t = {0.0, 0.0};
    ++vert_used;
  }
  // 12 triangles (2 per face), same winding as ExoTest's cube.
  const ISC tri[36] = {
      // front (z=+s)
      4, 5, 6, 4, 6, 7,
      // back (z=-s)
      1, 0, 3, 1, 3, 2,
      // left (x=-s)
      0, 4, 7, 0, 7, 3,
      // right (x=+s)
      5, 1, 2, 5, 2, 6,
      // bottom (y=-s)
      0, 1, 5, 0, 5, 4,
      // top (y=+s)
      3, 7, 6, 3, 6, 2};
  for (IUD i = 0; i < 36; ++i) indices[idx_used++] = (ISC)(base + tri[i]);
  return (ISC)base;
}

void TScene::SetCamera(const FPD3& eye_, const FPD3& target_,
                       const FPD3& up_, FPD fov_) {
  eye = eye_;
  target = target_;
  up = up_;
  fov = fov_;
}

void TScene::Render(TFrame& frame, const VColor& bg) {
  if (idx_used == 0) {
    ClearFrame(frame, bg);
    return;
  }
  aspect = (FPD)frame.width / (FPD)frame.height;
  TMat4 view = LookAt(eye, target, up);
  TMat4 proj = Perspective(fov, aspect, 0.1, 100.0);
  TMat4 mvp = Multiply(proj, view);
  ClearFrame(frame, bg);
  DrawIndexed(frame, mvp, verts, vert_used, indices, idx_used);
}

}  // namespace Exo
}  // namespace CTK
