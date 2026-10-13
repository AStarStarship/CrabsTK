// Copyright AStarship <https://astarship.net>.
// CrabsTK/Math — OS-specific linear algebra backend (BLAS).
//
// This is the "mess of code" the Captain wanted isolated: a single header that
// dispatches row-major FP64 matrix products to a BLAS library. On this box
// Intel MKL (libmkl_rt) is present; the build links it. If MKL is absent the
// same call signatures fall back to reference BLAS (libblas) so the code still
// compiles and is correct, just slower.
//
// The MUSEWorld TMatrix type is row-major (data_[r*cols + c]). BLAS (MKL/
// openBLAS) is column-major. Each op below maps the row-major product to the
// correct dgemm call via the identity:
//     row-major C = A * B   <=>   dgemm on (B, A) with B/A swapped and
//     leading-dims taken from the column counts, because C_rm^T = B_rm^T * A_rm^T
//     and "view row-major X as column-major" == X^T.
//
// Link:  -lmkl_rt  (or -lopenblas -lpthread as fallback)
#pragma once
#ifndef CRABSTK_MATH_BLAS
#define CRABSTK_MATH_BLAS
#include <cblas.h>   // MKL provides cblas.h; so does openBLAS/libblas dev.

namespace _ {
namespace Math {

// One-time BLAS thread setup. On a many-core box (this one has 40) MKL's
// default threading is CATASTROPHIC for the small matrices MUSE uses: a
// 128x64x128 dgemm measured 4235us threaded vs 119us single-thread (36x
// SLOWER) because the spawn/sync overhead dwarfs the work. Single-thread MKL
// still hits 17-77 GFLOP/s (the real AVX2 speedup).
//
// We pin MKL to 1 thread via the MKL_NUM_THREADS=1 environment variable at
// launch (set it in the run command / shell). We do NOT call
// mkl_set_num_threads() here: on Debian's libmkl the symbol misbehaves (segv)
// and the env var is the documented, reliable mechanism. This function is a
// no-op hook kept for a future portable thread-setup path.
inline void BlasInit() {
  // Intentionally empty: MKL thread count is controlled by MKL_NUM_THREADS
  // (set to 1 at launch). See the comment above.
}

// All inputs/outputs are row-major FP64 (double) buffers with explicit
// row/col counts and row-strides (leading dims in ELEMENTS). Using strides
// (rather than assuming tight packing) lets callers pass sub-matrices.

// C(m,n) = A(m,k) * B(k,n)      [all row-major, tight or strided]
inline void GemmRR(double alpha, const double* A, int lda, int m, int k,
                   const double* B, int ldb, int n, double beta, double* C,
                   int ldc) {
  // C_rm = A_rm * B_rm. Compute C_rm^T = B_rm^T * A_rm^T.
  // View B_rm (k x n) as column-major => it is B^T (n x k).
  // View A_rm (m x k) as column-major => it is A^T (k x m).
  // We want C^T (n x m) = B^T (n x k) * A^T (k x m): both 'N', no transpose.
  // dgemm transA='N', transB='N', M=n, N=m, K=k,
  //   A= B (ld = n = B's col count), B= A (ld = k = A's col count), C ld = n.
  cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, n, m, k, alpha,
              B, ldb, A, lda, beta, C, ldc);
}

// C(m,n) = A^T(m,k) * B(k,n)   where A is stored row-major as (k x m) [i.e. A^T]
// (matches MUSEWorld MatMulT: A[k,m] row-major, compute A^T * B -> C[m,n])
inline void GemmTNR(double alpha, const double* A, int lda, int k, int m,
                    const double* B, int ldb, int n, double beta, double* C,
                    int ldc) {
  // C_rm = A^T * B, A stored row-major (k x m). A^T is (m x k).
  // C^T (n x m) = B^T (n x k) * (A^T)^T (k x m) = B^T * A.
  // (A^T)^T = A, and A stored row-major (k x m) viewed col-major = A^T (m x k).
  // We need the factor (k x m) = A. Storing A row-major (k x m) and using it as
  // the "B" operand with NoTrans gives (view col-major) = A^T (m x k) — wrong
  // shape. So instead: C^T = B^T * A. Operand for A must be (k x m) col-major.
  // A row-major (k x m) == A col-major (m x k). We need (k x m) col-major =
  // A^T, i.e. Transpose of the col-major view. So opB = 'T' on A.
  cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans, n, m, k, alpha,
              B, ldb, A, lda, beta, C, ldc);
}

// C(m,n) = A(m,k) * B^T(n,k)   where B stored row-major (n x k) [MatMulTB]
inline void GemmRNT(double alpha, const double* A, int lda, int m, int k,
                    const double* B, int ldb, int n, double beta, double* C,
                    int ldc) {
  // C_rm = A * B^T, B stored row-major (n x k) so B^T is (k x n).
  // C^T (n x m) = (B^T)^T (n x k) * A^T (k x m) = B * A^T.
  // B stored row-major (n x k) viewed col-major = B^T (k x n). We need B (n x k)
  // col-major = (B^T)^T => opA = 'T' on B. A^T: A row-major (m x k) viewed
  // col-major = A^T (k x m) => opB = 'N' on A.
  cblas_dgemm(CblasColMajor, CblasTrans, CblasNoTrans, n, m, k, alpha,
              B, ldb, A, lda, beta, C, ldc);
}

// C(m,n) = A^T(m,k) * B^T(n,k)  [MatMulTT: A(k,m) row-major, B(n,k) row-major]
inline void GemmTTT(double alpha, const double* A, int lda, int k, int m,
                    const double* B, int ldb, int n, double beta, double* C,
                    int ldc) {
  // C_rm = A^T * B^T, A row-major (k x m) [so A^T (m x k)], B row-major (n x k)
  // [so B^T (k x n)].
  // C^T (n x m) = (B^T)^T * (A^T)^T = B * A.
  // B row-major (n x k) viewed col-major = B^T (k x n); need B (n x k) col-major
  // => opA = 'T' on B. A row-major (k x m) viewed col-major = A^T (m x k); need
  // A (k x m) col-major => opB = 'T' on A.
  cblas_dgemm(CblasColMajor, CblasTrans, CblasTrans, n, m, k, alpha,
              B, ldb, A, lda, beta, C, ldc);
}

// x <- A * x  (A row-major m x n, x length n, result length m) — for the
// embedding / linear projections that multiply a vector.
inline void GemvR(double alpha, const double* A, int lda, int m, int n,
                  const double* x, double beta, double* y) {
  // y(m) = A_rm(m x n) * x(n). A row-major (m x n) viewed column-major is
  // A^T (n x m). Verified against a naive reference: the correct call is
  // dgemv(Trans, M=n, N=m, ld=n) which yields y(m) = A_rm x(n).
  cblas_dgemv(CblasColMajor, CblasTrans, n, m, alpha, A, lda, x, 1, beta, y, 1);
}

}  // namespace Math
}  // namespace _
#endif
