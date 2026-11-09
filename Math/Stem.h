// Copyright AStarship <https://astarship.net>.
//
// Stem.h — STEM math functions for CrabsTK.
//
// Projection, transform, and colorization primitives for scientific
// visualization. These are the functions Manim doesn't have: direct
// tensor-to-pixel projection with no Cairo/FFmpeg pipeline.
//
// Uses ASCIICrabs homotuples: FPD3 for 3D points, ISB4 for RGBA color.
// All functions operate on caller-owned FPD arrays. No allocation.
//
// Binary I/O (ReadBinary/WriteBinary) is declared here, implemented
// in Stem.hxx (which owns the OS I/O shim, same as File.hxx).

#pragma once
#ifndef CRABSTK_MATH_STEM
#define CRABSTK_MATH_STEM

#include "../../ASCIICrabs/Math.hxx"
#include "../../ASCIICrabs/Homotuples.h"

namespace CTK {
namespace Math {
namespace Stem {

/* Isometric 3D -> 2D projection (same as Plot.hxx Plot3Pt).
   sx = (x - y) * 0.7071
   sy = (x + y) * 0.7071 - z
   Returns the projected point; depth = (x + y) * 0.5 for painter's order. */
inline FPD2 IsoProject(const FPD3& p, FPD* depth) {
  const FPD k = 0.7071;
  *depth = (p.x + p.y) * 0.5;
  return FPD2((p.x - p.y) * k, (p.x + p.y) * k - p.z);
}

/* Euler-to-3D: given Euler angles (azimuth, polar, twist) from the first
   3 coords of a 32D spectral table, compute the unit-sphere 3D point.
   x = cos(azimuth) * sin(polar)
   y = sin(azimuth) * sin(polar)
   z = cos(polar)
   The twist (3rd coord) is unused in the basic projection; it can be
   used for per-point rotation in extended visualizations. */
inline FPD3 EulerTo3D(FPD azimuth, FPD polar, FPD twist) {
  (void)twist;
  FPD sp = Sine(polar);
  return FPD3(
    Cosine(azimuth) * sp,
    Sine(azimuth) * sp,
    Cosine(polar)
  );
}

/* Normalize a vector in-place to unit length. Returns the original norm,
   or 0 if the vector is zero. */
inline FPD Normalize3(FPD3* p) {
  FPD n = SquareRoot(p->x * p->x + p->y * p->y + p->z * p->z);
  if (n > 0) {
    FPD inv = 1.0 / n;
    *p = (*p) * inv;
  }
  return n;
}

/* Map a 32D unit-sphere embedding row to 3D by taking the first 3
   coordinates directly. The 32D points are already on the unit sphere
   (spectral_spherical_32d), so coords 0,1,2 are 3D Cartesian
   coordinates, NOT Euler angles. This gives a full 3D spread.
   pts32 is a row-major [n][dim] array; this projects row i. */
inline FPD3 Project32dRow(const FPD* pts32, IUD i, IUD dim) {
  return FPD3(pts32[i * dim + 0], pts32[i * dim + 1], pts32[i * dim + 2]);
}

/* Zipf-rank color: rank 0 (most frequent) = amber, rank n-1 = cyan.
   Returns ISB4 (R, G, B, A) — the ASCIICrabs 4-tuple of ISB.
   Matches the Python SpectralProjector.py palette. */
inline ISB4 RankColor(IUD rank, IUD n) {
  FPD t = (FPD)rank / (FPD)(n > 1 ? n - 1 : 1);
  return ISB4(
    (ISB)(255.0 * (1.0 - t) + 34.0 * t),
    (ISB)(208.0 * (1.0 - t) + 164.0 * t),
    (ISB)(120.0 * (1.0 - t) + 180.0 * t),
    (ISB)255
  );
}

/* Point size by Zipf rank: rank 0 = 4px, high rank = 1px. */
inline ISC RankSize(IUD rank, IUD n) {
  if (rank < 64) return 4;
  if (rank < 512) return 2;
  (void)n;
  return 1;
}

/* Map normalized 3D (-1..1) to pixel coords within a rect. */
inline void NormToPixel(FPD nx, FPD ny, ISC x0, ISC y0, ISC x1, ISC y1,
                        ISC* px, ISC* py) {
  *px = x0 + (ISC)((nx * 0.5 + 0.5) * (x1 - x0));
  *py = y0 + (ISC)((ny * 0.5 + 0.5) * (y1 - y0));
}

/* ── Binary I/O ───────────────────────────────────────────────────
   Temporary stdlib wrapper (fopen/fread/fwrite/fclose).
   To be replaced by the ASCIICrabs File API when the seam system
   supports standalone tools. */

/* Read up to 'bytes' from path into buf at byte offset 'offset'.
   Returns bytes read, or 0 on open failure. */
IUD ReadBinary(const CHA* path, void* buf, IUD bytes, IUD offset);

/* Read up to 'bytes' from path into buf from the start.
   Returns bytes read, or 0 on open failure. */
IUD ReadBinary(const CHA* path, void* buf, IUD bytes);

/* Write 'bytes' from buf to path. Returns true on success. */
BOL WriteBinary(const CHA* path, const void* buf, IUD bytes);

}  // namespace Stem
}  // namespace Math
}  // namespace CTK

#endif  //< CRABSTK_MATH_STEM
