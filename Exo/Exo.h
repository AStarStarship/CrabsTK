// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_EXOH
#define CRABSTK_EXOH
#include "../ASCIICrabs/Math.hxx"
#include "../ASCIICrabs/Homotuples.h"
#include "../Image/Image.h"
#include "../Image/IDraw.h"
namespace CTK {
namespace Exo {

// Exo.h — the exoskeleton: a self-contained software 3D rasterizer.
// "Exo" = outside. This is the rendering layer that lives on the outside
// of the MUSE math: it takes 3D vertices (projected from spectral
// embeddings, point clouds, or arbitrary geometry) and rasterizes them
// into a CrabsTK CImage RGBA frame, with a z-buffer, perspective
// projection, and per-vertex attribute interpolation.
//
// Design (modeled on the SDL3 renderer API + Black Art of 3D concepts,
// re-implemented clean):
//   - No stdlib. Types from ASCIICrabs (_ConfigHeader.h), math from
//     ASCIICrabs Math.hxx (self-contained, no libm), pixels into the
//     CrabsTK Image layer (CImage / ImageDraw).
//   - No window, no GPU, no driver. A frame is an RGBA buffer the caller
//     owns. Render a frame, ship the bytes. Deterministic, testable.
//   - Forward rasterization (NOT ray tracing): transform vertices ->
//     perspective project -> fill triangles with edge functions ->
//     per-pixel barycentric interpolation of z and color -> depth test.
//
// The vertex/attribute model mirrors SDL_Vertex: position + color +
// texcoord, drawn as indexed triangle batches.

/* A 4x4 column-major matrix (row 0 = m00,m01,m02,m03 ... row 3 = m30..m33).
   We store it as FPD[16] with m[col*4+row] so a vertex v' = M * v is a
   straight 16-multiply dot product per component. */
struct TMat4 {
  FPD m[16];

  /* Identity. */
  static TMat4 Identity() {
    TMat4 r;
    for (int i = 0; i < 16; ++i) r.m[i] = 0.0;
    r.m[0] = r.m[5] = r.m[10] = r.m[15] = 1.0;
    return r;
  }
};

/* Build a perspective projection matrix.
   fovY in radians, aspect = width/height, near/far > 0, near < far.
   Maps the view frustum (camera looks down -Z) to NDC [-1,1]^3. */
TMat4 Perspective(FPD fovY, FPD aspect, FPD near_, FPD far_);

/* Build a view matrix from a camera: eye position, look target, up vector.
   (Standard look-at; the camera looks along normalize(target-eye).) */
TMat4 LookAt(const FPD3& eye, const FPD3& target, const FPD3& up);

/* Axis rotation matrices (radians). */
TMat4 RotateX(FPD rad);
TMat4 RotateY(FPD rad);
TMat4 RotateZ(FPD rad);

/* Translation matrix. */
TMat4 Translate(FPD x, FPD y, FPD z);

/* Uniform + non-uniform scale. */
TMat4 Scale(FPD sx, FPD sy, FPD sz);

/* Multiply two matrices: result = a * b (apply b first, then a). */
TMat4 Multiply(const TMat4& a, const TMat4& b);

/* Transform a 3D point by a 4x4 matrix (w=1, perspective divide applied
   if the matrix carries a w term). Returns the projected FPD3. */
FPD3 TransformPoint(const TMat4& M, const FPD3& p);

/* A vertex: position (world space) + color + texture coordinate.
   Mirrors SDL_Vertex {position, color, tex_coord}. */
struct TVertex {
  FPD3 p;     //< world-space position
  ISB4 c;     //< RGBA color
  FPD2 t;     //< normalized texture coordinate (0..1)
};

/* A frame: an RGBA image buffer plus a floating z-buffer for the depth
   test. The caller owns both buffers (or uses the stack-allocated helper
   below). z is cleared to a far value by TFrame::Begin. */
struct TFrame {
  CImage* img;     //< RGBA destination (caller-owned)
  FPD* zbuf;       //< depth buffer, width*height (caller-owned)
  ISN width;
  ISN height;
  FPD zfar;        //< "far" depth (z-buffer cleared to this; canonical far = -1)

  void Begin(FPD background_far = -1.0) {
    zfar = background_far;
  }
};

/* Clear the frame's color to a solid color and reset the z-buffer to far. */
void ClearFrame(TFrame& f, const ImageDraw::TColor& bg);

/* Rasterize a triangle given three screen-space vertices with depth and
   color. Uses edge functions for coverage, barycentric interpolation for
   z and RGB, and the z-buffer for the depth test. Alpha is taken from the
   average vertex alpha.
   a,b,c: screen x,y (pixels), z (view depth), r,g,b (0..255). */
void Triangle(TFrame& f,
              FPD ax, FPD ay, FPD az, FPD ar, FPD ag, FPD ab,
              FPD bx, FPD by, FPD bz, FPD br, FPD bg, FPD bb,
              FPD cx, FPD cy, FPD cz, FPD cr, FPD cg, FPD cb);

/* Draw a triangle batch from a vertex array + index list.
   proj: the combined model*view*projection matrix.
   verts: world-space vertices. indices: triangle indices (count/3 tris). */
void DrawIndexed(TFrame& f, const TMat4& proj, const TVertex* verts,
                 IUD vertCount, const ISN* indices, IUD indexCount);

/* Convenience: fill a full-frame textured quad (for the WebGL-style
   texture display) — maps a CImage texture over the whole frame with a
   pan/zoom transform. uv = center + pos*scale + pan. */
void BlitTexture(TFrame& f, const CImage& tex, FPD scale, FPD panX, FPD panY);

/* Project a single world-space point to screen pixels + depth using the
   combined matrix. Returns false if the point is behind the near plane. */
BOL ProjectPoint(const TMat4& proj, const FPD3& world, ISN width, ISN height,
                 FPD* sx, FPD* sy, FPD* depth);

/* ── Scene layer (the "machine's display") ──────────────────────
   A TScene is a collection of textured quads + a camera. This is the
   unified 2D/3D primitive: a quad at z=0 with an orthographic-down-Z
   camera is a "2D tile/sprite"; the same quad in 3D with a perspective
   camera is a billboard or a placed face. The agent's terminal output
   renders into a TScene; TScene::Render rasterizes it to a CImage frame.

   No stdlib: the vertex/index arrays are new[]/delete[] (the established
   no-stdlib dynamic pattern). The scene owns its buffers. */
struct TScene {
  TVertex* verts;      //< dynamic vertex array (owned)
  IUD vert_count;      //< number of vertices allocated
  IUD vert_used;       //< number of vertices used
  ISN* indices;        //< dynamic index array (owned)
  IUD idx_count;       //< number of indices allocated
  IUD idx_used;        //< number of indices used

  // Camera (look-at). eye/target/up; the camera looks at target from eye.
  FPD3 eye;
  FPD3 target;
  FPD3 up;
  FPD fov;             //< vertical field of view (radians)
  FPD aspect;          //< width/height (set by Render from the frame)

  TScene();
  ~TScene();

  /* Add a textured quad. `pos` = center (world), `size` = (w,h) extent,
     `color` = the quad's RGBA. `t0`/`t1` = the texture sub-rect (0..1);
     pass {0,0}/{1,1} for the full texture (a solid-color quad when no
     texture is bound). `axis` selects the quad's plane: 0=XY (z=0, the
     "2D" case), 1=XZ (y=0, floor), 2=YZ (x=0, wall). Returns the quad's
     first vertex index, or -1 on overflow. */
  ISN AddQuad(const FPD3& pos, const FPD2& size, const ISB4& color,
              const FPD2& t0, const FPD2& t1, ISC axis = 0);

  /* Add a unit cube at `pos` with per-face `color`. Convenience for the
     test/demo. Returns the cube's first vertex index, or -1 on overflow. */
  ISN AddCube(const FPD3& pos, const ISB4& color);

  /* Set the camera (look-at). */
  void SetCamera(const FPD3& eye_, const FPD3& target_, const FPD3& up_,
                 FPD fov_);

  /* Render the scene into `frame`. Builds the model (per-quad, identity
     for now — quads carry their world position in their vertices) * view
     (camera) * projection, then DrawIndexed over the whole batch. The
     frame's aspect sets the projection aspect. */
  void Render(TFrame& frame, const ImageDraw::TColor& bg);
};

}  // namespace Exo
}  // namespace CTK

#endif  //< CRABSTK_EXOH
