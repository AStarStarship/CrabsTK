// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_MATH_CUDA_BLAS
#define CRABSTK_MATH_CUDA_BLAS
#include "../../ASCIICrabs/_Config.h"
#include <cublas_v2.h>

// Borrowed device buffers and handle; asynchronous on the caller's stream.
// No implicit transfers, allocation, synchronization or handle-policy changes.
// See CudaBlas.md for ownership, validation and completion/error contracts.
namespace _ {
namespace Math {
namespace Cuda {
namespace Detail {

inline BOL MatrixFits(ISC rows, ISC cols, ISC stride) {
  if (rows < 0 || cols < 0 || stride < 1 || stride < cols) return false;
  if (rows == 0 || cols == 0) return true;
  const IUD elements = IUD(rows - 1) * IUD(stride) + IUD(cols);
  return elements <= IUD(IUW(-1) >> 1) / sizeof(FPD);
}

inline cublasStatus_t HostMode(cublasHandle_t handle) {
  if (!handle) return CUBLAS_STATUS_NOT_INITIALIZED;
  cublasPointerMode_t mode;
  const cublasStatus_t status = cublasGetPointerMode(handle, &mode);
  if (status != CUBLAS_STATUS_SUCCESS) return status;
  return mode == CUBLAS_POINTER_MODE_HOST ? CUBLAS_STATUS_SUCCESS :
                                          CUBLAS_STATUS_INVALID_VALUE;
}

inline cublasStatus_t Scale(cublasHandle_t handle, ISC m, ISC n,
                            FPD beta, FPD* c, ISC ldc) {
  if (beta == 1) return CUBLAS_STATUS_SUCCESS;
  // In-place GEAM requires op=N and matching leading dimensions. Its zero
  // coefficients omit input reads, unlike SCAL(0), which propagates old NaNs.
  const FPD zero = 0;
  return cublasDgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, n, m, &beta, c, ldc,
                     &zero, nullptr, ldc, c, ldc);
}

inline cublasStatus_t Gemm(cublasHandle_t handle, BOL ta, BOL tb,
                           FPD alpha, const FPD* a, ISC lda, ISC m, ISC k,
                           const FPD* b, ISC ldb, ISC n, FPD beta,
                           FPD* c, ISC ldc) {
  if (!handle) return CUBLAS_STATUS_NOT_INITIALIZED;
  if (!MatrixFits(ta ? k : m, ta ? m : k, lda) ||
      !MatrixFits(tb ? n : k, tb ? k : n, ldb) || !MatrixFits(m, n, ldc))
    return CUBLAS_STATUS_INVALID_VALUE;
  const cublasStatus_t status = HostMode(handle);
  if (status != CUBLAS_STATUS_SUCCESS) return status;
  if (m == 0 || n == 0) return CUBLAS_STATUS_SUCCESS;
  if (!c || (alpha != 0 && k != 0 && (!a || !b)))
    return CUBLAS_STATUS_INVALID_VALUE;
  if (alpha == 0 || k == 0) return Scale(handle, m, n, beta, c, ldc);
  // C_rm^T = op(B_rm)^T * op(A_rm)^T. Swap operands/dimensions and
  // their transpose flags; row strides become column leading dimensions.
  return cublasDgemm(handle, tb ? CUBLAS_OP_T : CUBLAS_OP_N,
                     ta ? CUBLAS_OP_T : CUBLAS_OP_N, n, m, k, &alpha,
                     b, ldb, a, lda, &beta, c, ldc);
}

}  // namespace Detail

inline cublasStatus_t GemmRR(cublasHandle_t handle, FPD alpha, const FPD* a,
                             ISC lda, ISC m, ISC k, const FPD* b, ISC ldb,
                             ISC n, FPD beta, FPD* c, ISC ldc) {
  return Detail::Gemm(handle, false, false, alpha, a, lda, m, k, b, ldb,
                       n, beta, c, ldc);
}
inline cublasStatus_t GemmTNR(cublasHandle_t handle, FPD alpha, const FPD* a,
                              ISC lda, ISC k, ISC m, const FPD* b, ISC ldb,
                              ISC n, FPD beta, FPD* c, ISC ldc) {
  return Detail::Gemm(handle, true, false, alpha, a, lda, m, k, b, ldb,
                       n, beta, c, ldc);
}
inline cublasStatus_t GemmRNT(cublasHandle_t handle, FPD alpha, const FPD* a,
                              ISC lda, ISC m, ISC k, const FPD* b, ISC ldb,
                              ISC n, FPD beta, FPD* c, ISC ldc) {
  return Detail::Gemm(handle, false, true, alpha, a, lda, m, k, b, ldb,
                       n, beta, c, ldc);
}
inline cublasStatus_t GemmTTT(cublasHandle_t handle, FPD alpha, const FPD* a,
                              ISC lda, ISC k, ISC m, const FPD* b, ISC ldb,
                              ISC n, FPD beta, FPD* c, ISC ldc) {
  return Detail::Gemm(handle, true, true, alpha, a, lda, m, k, b, ldb,
                       n, beta, c, ldc);
}

inline cublasStatus_t GemvR(cublasHandle_t handle, FPD alpha, const FPD* a,
                            ISC lda, ISC m, ISC n, const FPD* x,
                            FPD beta, FPD* y) {
  if (!handle) return CUBLAS_STATUS_NOT_INITIALIZED;
  if (!Detail::MatrixFits(m, n, lda) || !Detail::MatrixFits(m, 1, 1) ||
      !Detail::MatrixFits(n, 1, 1)) return CUBLAS_STATUS_INVALID_VALUE;
  const cublasStatus_t status = Detail::HostMode(handle);
  if (status != CUBLAS_STATUS_SUCCESS) return status;
  if (m == 0) return CUBLAS_STATUS_SUCCESS;
  if (!y || (alpha != 0 && n != 0 && (!a || !x)))
    return CUBLAS_STATUS_INVALID_VALUE;
  if (alpha == 0 || n == 0) return Detail::Scale(handle, m, 1, beta, y, 1);
  // Row-major A(m,n) is a column-major A^T(n,m); transpose it back.
  return cublasDgemv(handle, CUBLAS_OP_T, n, m, &alpha, a, lda,
                     x, 1, &beta, y, 1);
}

}  // namespace Cuda
}  // namespace Math
}  // namespace _
#endif
