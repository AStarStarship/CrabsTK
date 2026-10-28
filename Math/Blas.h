#pragma once
#ifndef CRABSTK_MATH_BLAS
#define CRABSTK_MATH_BLAS

// Copyright AStarship <https://astarship.net>.
// Host FP64 row-major BLAS. Backend selection is explicit, never header-only
// autodetection: installed headers do not guarantee a linked library.
// Default: scalar. CRABSTK_MATH_MKL=1: MKL LP64. CBLAS_H_AVAILABLE=1: CBLAS.
// See README.md for buffer, size, alias and threading contracts.
#include "../../ASCIICrabs/_Config.h"
#include "../../ASCIICrabs/Math.hxx"

#ifndef CRABSTK_MATH_MKL
#define CRABSTK_MATH_MKL 0
#endif
#ifndef CBLAS_H_AVAILABLE
#define CBLAS_H_AVAILABLE 0
#endif

#if CRABSTK_MATH_MKL
#include <mkl.h>
static_assert(sizeof(MKL_INT) == sizeof(ISC), "Math requires MKL LP64, not ILP64");
#elif CBLAS_H_AVAILABLE
#ifndef CRABSTK_MATH_CBLAS_HEADER
#define CRABSTK_MATH_CBLAS_HEADER <cblas.h>
#endif
#include CRABSTK_MATH_CBLAS_HEADER
#endif

namespace _ {
namespace Math {
namespace Detail {

// Zero beta must not read old (possibly uninitialized or NaN) output.
inline FPD Accumulate(FPD value, FPD beta, const FPD* old) {
  return beta == 0 ? value : value + beta * *old;
}

inline void Scale(FPD* c, ISC m, ISC n, ISC ldc, FPD beta) {
  if (beta == 1) return;
  for (ISC i = 0; i < m; ++i)
    for (ISC j = 0; j < n; ++j) {
      FPD* cell = c + ISW(i) * ldc + j;
      *cell = beta == 0 ? 0 : beta * *cell;
    }
}

inline void RefGemm(BOL ta, BOL tb, ISC m, ISC n, ISC k, FPD alpha,
                    const FPD* a, ISC lda, const FPD* b, ISC ldb,
                    FPD beta, FPD* c, ISC ldc) {
  if (alpha == 0 || k == 0) {
    Scale(c, m, n, ldc, beta);
    return;
  }
  for (ISC i = 0; i < m; ++i) {
    for (ISC j = 0; j < n; ++j) {
      FPD sum = 0;
      for (ISC p = 0; p < k; ++p)
        sum += a[ta ? ISW(p) * lda + i : ISW(i) * lda + p] *
               b[tb ? ISW(j) * ldb + p : ISW(p) * ldb + j];
      FPD* cell = c + ISW(i) * ldc + j;
      *cell = Accumulate(alpha * sum, beta, cell);
    }
  }
}

inline void Gemm(BOL ta, BOL tb, FPD alpha, const FPD* a, ISC lda,
                 ISC m, ISC k, const FPD* b, ISC ldb, ISC n,
                 FPD beta, FPD* c, ISC ldc) {
  if (m == 0 || n == 0) return;
  if (alpha == 0 || k == 0) {
    Scale(c, m, n, ldc, beta);
    return;
  }
#if CRABSTK_MATH_MKL || CBLAS_H_AVAILABLE
  cblas_dgemm(CblasRowMajor, ta ? CblasTrans : CblasNoTrans,
              tb ? CblasTrans : CblasNoTrans, m, n, k, alpha,
              a, lda, b, ldb, beta, c, ldc);
#else
  RefGemm(ta, tb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
#endif
}

}  // namespace Detail
}  // namespace Math
}  // namespace _

// ============================================================
// Reference BLAS — software fallback when no external BLAS is linked.
// ============================================================

// Reference dgemm: C(m,n) = alpha * A(m,k) * B(k,n) + beta * C(m,n)
// All matrices are row-major with explicit leading dimensions (lda, ldb, ldc).
// This is the naive O(m*k*n) reference algorithm that matches the test
// reference computations in BlasTest.hxx.
inline void ref_dgemm(ISC m, ISC n, ISC k, FPD alpha, const FPD* a, ISC lda,
                      const FPD* b, ISC ldb, FPD beta, FPD* c, ISC ldc) {
  _::Math::Detail::RefGemm(false, false, m, n, k, alpha, a, lda, b, ldb,
                          beta, c, ldc);
}

// Reference dgemv: y(m) = alpha * A(m,n) * x(n) + beta * y(m)
// A is row-major m x n with leading dimension lda.
// x is length n, y is length m.
inline void ref_dgemv(ISC m, ISC n, FPD alpha, const FPD* a, ISC lda,
                      const FPD* x, FPD beta, FPD* y) {
  _::Math::Detail::RefGemm(false, false, m, 1, n, alpha, a, lda, x, 1,
                          beta, y, 1);
}

// ============================================================
// BLAS function definitions — all inside _::Math namespace.
// ============================================================

namespace _ {
namespace Math {

// Compatibility hook: never mutate process-wide thread policy implicitly.
// Use MKL_NUM_THREADS/MKL_DYNAMIC before startup, or an explicit MKL local
// thread scope owned by the application. See README.md.
inline void BlasInit() {}

// All inputs/outputs are row-major FP64 (double) buffers with explicit
// row/col counts and row-strides (leading dims in ELEMENTS). Using strides
// (rather than assuming tight packing) lets callers pass sub-matrices.

// C(m,n) = A(m,k) * B(k,n)      [all row-major, tight or strided]
inline void GemmRR(FPD alpha, const FPD* a, ISC lda, ISC m, ISC k,
                   const FPD* b, ISC ldb, ISC n, FPD beta, FPD* c, ISC ldc) {
  Detail::Gemm(false, false, alpha, a, lda, m, k, b, ldb, n, beta, c, ldc);
}

// C(m,n) = A^T(m,k) * B(k,n)   where A is stored row-major as (k x m) [i.e. A^T]
// (matches TMatrix MatMulT: A[k,m] row-major, compute A^T * B -> C[m,n])
inline void GemmTNR(FPD alpha, const FPD* a, ISC lda, ISC k, ISC m,
                    const FPD* b, ISC ldb, ISC n, FPD beta, FPD* c, ISC ldc) {
  Detail::Gemm(true, false, alpha, a, lda, m, k, b, ldb, n, beta, c, ldc);
}

// C(m,n) = A(m,k) * B^T(n,k)   where B stored row-major (n x k) [MatMulTB]
inline void GemmRNT(FPD alpha, const FPD* a, ISC lda, ISC m, ISC k,
                    const FPD* b, ISC ldb, ISC n, FPD beta, FPD* c, ISC ldc) {
  Detail::Gemm(false, true, alpha, a, lda, m, k, b, ldb, n, beta, c, ldc);
}

// C(m,n) = A^T(m,k) * B^T(n,k)  [MatMulTT: A(k,m) row-major, B(n,k) row-major]
inline void GemmTTT(FPD alpha, const FPD* a, ISC lda, ISC k, ISC m,
                    const FPD* b, ISC ldb, ISC n, FPD beta, FPD* c, ISC ldc) {
  Detail::Gemm(true, true, alpha, a, lda, m, k, b, ldb, n, beta, c, ldc);
}

// y(m) = alpha * A(m,n) * x(n) + beta * y(m). x/y must not overlap.
inline void GemvR(FPD alpha, const FPD* a, ISC lda, ISC m, ISC n,
                  const FPD* x, FPD beta, FPD* y) {
  if (m == 0) return;
  // Like GEMM, an empty contraction scales output; no input reads.
  if (alpha == 0 || n == 0) {
    Detail::Scale(y, m, 1, 1, beta);
    return;
  }
#if CRABSTK_MATH_MKL || CBLAS_H_AVAILABLE
  cblas_dgemv(CblasRowMajor, CblasNoTrans, m, n, alpha, a, lda, x, 1, beta, y, 1);
#else
  ref_dgemv(m, n, alpha, a, lda, x, beta, y);
#endif
}

// ============================================================
// Convenience utilities (reference, no BLAS dependency)
// ============================================================

// Initialize a matrix segment to zero
inline void ZeroMatrix(FPD* data, ISC count) {
  for (ISC i = 0; i < count; ++i) data[i] = 0;
}

// Compute max absolute error between two double arrays
inline FPD MaxErr(const FPD* a, const FPD* b, ISC count) {
  FPD e = 0;
  for (ISC i = 0; i < count; ++i) {
    const FPD err = fabs(a[i] - b[i]);
    if (!isfinite(err)) return INFINITY;
    if (err > e) e = err;
  }
  return e;
}

}  // namespace Math
}  // namespace _

#endif  // CRABSTK_MATH_BLAS
