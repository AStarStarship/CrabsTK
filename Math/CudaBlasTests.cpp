// Copyright AStarship <https://astarship.net>.
#include "CudaBlas.h"
#include <cuda_runtime.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

namespace Cuda = _::Math::Cuda;
static ISC failures = 0;
static ISC cases = 0;

static BOL Check(BOL pass, const CHA* name) {
  ++cases;
  if (!pass) { ++failures; printf("FAIL: %s\n", name); }
  return pass;
}
static BOL Runtime(cudaError_t code, const CHA* name) {
  if (code == cudaSuccess) return true;
  ++failures;
  printf("CUDA FAIL: %s: %s\n", name, cudaGetErrorString(code));
  return false;
}
static BOL Blas(cublasStatus_t code, const CHA* name) {
  if (code == CUBLAS_STATUS_SUCCESS) return true;
  ++failures;
  printf("cuBLAS FAIL: %s: %d\n", name, ISC(code));
  return false;
}

// Setup/transfer/teardown are explicit TEST responsibilities, not adapter work.
struct TCudaTest {
  cudaStream_t stream = nullptr;
  cublasHandle_t handle = nullptr;
  FPD* a = nullptr;
  FPD* b = nullptr;
  FPD* c = nullptr;
  FPD* x = nullptr;
  ~TCudaTest() { Close(); }
  BOL Init(ISC device) {
    return Runtime(cudaSetDevice(device), "device") &&
      Runtime(cudaStreamCreateWithFlags(&stream, cudaStreamNonBlocking), "stream") &&
      Blas(cublasCreate(&handle), "create") &&
      Blas(cublasSetStream(handle, stream), "set stream") &&
      Blas(cublasSetPointerMode(handle, CUBLAS_POINTER_MODE_HOST), "host scalars") &&
      Runtime(cudaMalloc(&a, 256 * sizeof(FPD)), "allocate A") &&
      Runtime(cudaMalloc(&b, 256 * sizeof(FPD)), "allocate B") &&
      Runtime(cudaMalloc(&c, 256 * sizeof(FPD)), "allocate C/y") &&
      Runtime(cudaMalloc(&x, 256 * sizeof(FPD)), "allocate x");
  }
  void Close() {
    if (stream) Runtime(cudaStreamSynchronize(stream), "close completion");
    if (handle) { Blas(cublasDestroy(handle), "destroy handle"); handle = nullptr; }
    if (a) { Runtime(cudaFree(a), "free A"); a = nullptr; }
    if (b) { Runtime(cudaFree(b), "free B"); b = nullptr; }
    if (c) { Runtime(cudaFree(c), "free C"); c = nullptr; }
    if (x) { Runtime(cudaFree(x), "free x"); x = nullptr; }
    if (stream) { Runtime(cudaStreamDestroy(stream), "destroy stream"); stream = nullptr; }
  }
  BOL Put(FPD* dst, const FPD* src) {
    return Runtime(cudaMemcpyAsync(dst, src, 256 * sizeof(FPD),
      cudaMemcpyHostToDevice, stream), "upload fixture");
  }
  BOL Get(FPD* dst, const FPD* src) {
    return Runtime(cudaMemcpyAsync(dst, src, 256 * sizeof(FPD),
      cudaMemcpyDeviceToHost, stream), "download fixture") &&
      Runtime(cudaStreamSynchronize(stream), "execution completion");
  }
};

static cublasStatus_t Product(TCudaTest& gpu, ISC op, FPD alpha, const FPD* a,
                              ISC lda, ISC m, ISC k, const FPD* b, ISC ldb,
                              ISC n, FPD beta, FPD* c, ISC ldc) {
  switch (op) {
    case 0: return Cuda::GemmRR(gpu.handle, alpha, a, lda, m, k, b, ldb, n, beta, c, ldc);
    case 1: return Cuda::GemmTNR(gpu.handle, alpha, a, lda, k, m, b, ldb, n, beta, c, ldc);
    case 2: return Cuda::GemmRNT(gpu.handle, alpha, a, lda, m, k, b, ldb, n, beta, c, ldc);
    default: return Cuda::GemmTTT(gpu.handle, alpha, a, lda, k, m, b, ldb, n, beta, c, ldc);
  }
}

static BOL Equal(const FPD* a, const FPD* b) {
  for (ISC i = 0; i < 256; ++i)
    if (!isfinite(a[i]) || fabs(a[i] - b[i]) > 1e-12) return false;
  return true;
}

static void UninitializedOutputs(TCudaTest& gpu) {
  FPD a[256];
  for (ISC i = 0; i < 256; ++i) a[i] = FPD(i % 4 + 1);
  FPD actual[4], expected[4] = {};
  for (ISC i = 0; i < 2; ++i)
    for (ISC j = 0; j < 2; ++j)
      for (ISC p = 0; p < 2; ++p) expected[i * 2 + j] += a[i * 2 + p] * a[p * 2 + j];
  if (!gpu.Put(gpu.a, a) ||
      !Blas(Cuda::GemmRR(gpu.handle, 1, gpu.a, 2, 2, 2, gpu.a, 2,
                         2, 0, gpu.c, 2), "uninitialized output GEMM") ||
      !Runtime(cudaMemcpyAsync(actual, gpu.c, sizeof(actual),
                               cudaMemcpyDeviceToHost, gpu.stream), "read GEMM result") ||
      !Runtime(cudaStreamSynchronize(gpu.stream), "GEMM completion")) return;
  BOL pass = true;
  for (ISC i = 0; i < 4; ++i) if (actual[i] != expected[i]) pass = false;
  Check(pass,
        "beta-zero GEMM replaces uninitialized device output");
  if (!Blas(Cuda::GemvR(gpu.handle, 1, nullptr, 1, 2, 0, nullptr,
                        0, gpu.c + 8), "uninitialized scale output") ||
      !Runtime(cudaMemcpyAsync(actual, gpu.c + 8, 2 * sizeof(FPD),
                               cudaMemcpyDeviceToHost, gpu.stream), "read scale result") ||
      !Runtime(cudaStreamSynchronize(gpu.stream), "scale completion")) return;
  Check(actual[0] == 0 && actual[1] == 0,
        "empty contraction replaces uninitialized device output");
}

static void Matrices(TCudaTest& gpu) {
  const ISC shapes[][3] = {{3, 5, 2}, {1, 4, 7}, {6, 1, 3}, {7, 5, 9}};
  const FPD factors[][2] = {{1, 0}, {-0.75, 0.5}, {0, 0}, {0, -2}, {1, 1}};
  FPD a[256], b[256], c[256], expected[256], actual[256], after_a[256], after_b[256];
  for (ISC i = 0; i < 256; ++i) {
    a[i] = FPD((i * 7) % 17 - 8) / 8;
    b[i] = FPD((i * 3) % 19 - 9) / 16;
  }
  if (!gpu.Put(gpu.a, a) || !gpu.Put(gpu.b, b)) return;
  for (const auto& shape : shapes) {
    const ISC m = shape[0], k = shape[1], n = shape[2];
    for (ISC op = 0; op < 4; ++op) {
      const BOL ta = (op & 1) != 0, tb = (op & 2) != 0;
      for (ISC pad = 0; pad < 2; ++pad) {
        const ISC lda = (ta ? m : k) + pad * 3;
        const ISC ldb = (tb ? k : n) + pad * 2;
        const ISC ldc = n + pad * 4;
        for (const auto& factor : factors) {
          const FPD alpha = factor[0], beta = factor[1];
          for (ISC i = 0; i < 256; ++i) c[i] = expected[i] = 9123.25;
          for (ISC i = 0; i < m; ++i) {
            for (ISC j = 0; j < n; ++j) {
              const ISC index = 1 + i * ldc + j;
              c[index] = beta == 0 ? NAN : FPD(i - j) / 4;
              FPD sum = 0;
              for (ISC p = 0; p < k; ++p)
                sum += a[pad + (ta ? p * lda + i : i * lda + p)] *
                       b[pad * 2 + (tb ? j * ldb + p : p * ldb + j)];
              expected[index] = alpha * sum + (beta == 0 ? 0 : beta * c[index]);
            }
          }
          if (!gpu.Put(gpu.c, c) ||
              !Blas(Product(gpu, op, alpha, gpu.a + pad, lda, m, k,
                            gpu.b + pad * 2, ldb,
                            n, beta, gpu.c + 1, ldc), "GEMM submit") ||
              !gpu.Get(actual, gpu.c)) return;
          Check(Equal(actual, expected), "GEMM transpose/rectangular/stride/beta/canaries");
        }
      }
    }
  }
  if (!gpu.Get(after_a, gpu.a) || !gpu.Get(after_b, gpu.b)) return;
  Check(Equal(a, after_a) && Equal(b, after_b), "GEMM inputs unchanged");
}

static void Vectors(TCudaTest& gpu) {
  FPD a[256], x[256], y[256], expected[256], actual[256];
  for (ISC i = 0; i < 256; ++i) { a[i] = FPD(i - 8) / 8; x[i] = FPD(i % 5 - 2); }
  if (!gpu.Put(gpu.a, a) || !gpu.Put(gpu.x, x)) return;
  for (ISC pad = 0; pad < 2; ++pad) {
    const ISC m = 3, n = 5, lda = n + pad * 2;
    for (ISC mode = 0; mode < 3; ++mode) {
      const FPD alpha = mode == 2 ? 0 : -0.5, beta = mode == 0 ? 0 : 2;
      for (ISC i = 0; i < 256; ++i) y[i] = expected[i] = 99;
      for (ISC i = 0; i < m; ++i) {
        y[i + 1] = beta == 0 ? NAN : FPD(i - 1);
        FPD sum = 0;
        for (ISC j = 0; j < n; ++j) sum += a[i * lda + j] * x[j];
        expected[i + 1] = alpha * sum + (beta == 0 ? 0 : beta * y[i + 1]);
      }
      if (!gpu.Put(gpu.c, y) ||
          !Blas(Cuda::GemvR(gpu.handle, alpha, gpu.a, lda, m, n,
                            gpu.x, beta, gpu.c + 1), "GEMV submit") ||
          !gpu.Get(actual, gpu.c)) return;
      Check(Equal(actual, expected), "GEMV rectangular/stride/alpha/beta/canaries");
    }
  }
}

static void Edges(TCudaTest& gpu) {
  FPD c[256], expected[256], actual[256], a[256];
  const ISC logical[] = {1, 2, 4, 5};
  for (ISC op = 0; op < 4; ++op) {
    // Valid leading dimensions for every stored transpose shape, no buffers read.
    Check(Product(gpu, op, 1, nullptr, 3, 0, 3, nullptr, 3, 2, 0, nullptr, 2)
          == CUBLAS_STATUS_SUCCESS, "empty GEMM output");
    for (ISC i = 0; i < 256; ++i) c[i] = expected[i] = 99;
    for (ISC i : logical) { c[i] = NAN; expected[i] = 0; }
    if (!gpu.Put(gpu.c, c) ||
        !Blas(Product(gpu, op, 1, nullptr, 2, 2, 0, nullptr, 2, 2,
                      0, gpu.c + 1, 3), "empty contraction") ||
        !gpu.Get(actual, gpu.c)) return;
    Check(Equal(actual, expected), "empty GEMM contraction overwrites NaNs");
    if (!Blas(Product(gpu, op, 0, nullptr, 3, 2, 3, nullptr, 3, 2,
                      1, gpu.c + 1, 3), "zero alpha nil inputs") ||
        !gpu.Get(actual, gpu.c)) return;
    Check(Equal(actual, expected), "scale-only beta one");
    for (ISC i = 0; i < 256; ++i) a[i] = FPD(i % 4 + 1);
    for (ISC i = 0; i < 256; ++i) expected[i] = 99;
    for (ISC i = 0; i < 2; ++i)
      for (ISC j = 0; j < 2; ++j) {
        expected[i * 2 + j] = 0;
        for (ISC p = 0; p < 2; ++p)
          expected[i * 2 + j] += a[(op & 1) ? p * 2 + i : i * 2 + p] *
                                 a[(op & 2) ? j * 2 + p : p * 2 + j];
      }
    for (ISC i = 0; i < 256; ++i) c[i] = 99;
    if (!gpu.Put(gpu.a, a) || !gpu.Put(gpu.c, c) ||
        !Blas(Product(gpu, op, 1, gpu.a, 2, 2, 2, gpu.a, 2, 2,
                      0, gpu.c, 2), "aliased inputs") ||
        !gpu.Get(actual, gpu.c)) return;
    Check(Equal(actual, expected), "read-only input alias allowed");
  }
  for (ISC i = 0; i < 256; ++i) c[i] = expected[i] = 99;
  c[1] = c[2] = NAN; expected[1] = expected[2] = 0;
  if (!gpu.Put(gpu.c, c) ||
      !Blas(Cuda::GemvR(gpu.handle, 1, nullptr, 1, 2, 0, nullptr,
                        0, gpu.c + 1), "empty GEMV contraction") ||
      !gpu.Get(actual, gpu.c)) return;
  Check(Equal(actual, expected), "empty GEMV contraction overwrites NaNs");
  Check(Cuda::GemvR(gpu.handle, 1, nullptr, 2, 0, 2, nullptr, 0, nullptr)
        == CUBLAS_STATUS_SUCCESS, "empty GEMV output");
  Check(Cuda::GemmRR(nullptr, 1, gpu.a, 2, 2, 2, gpu.b, 2, 2, 0, gpu.c, 2)
        == CUBLAS_STATUS_NOT_INITIALIZED, "nil handle rejected");
  Check(Product(gpu, 0, 1, gpu.a, 1, 2, 3, gpu.b, 2, 2, 0, gpu.c, 2)
        == CUBLAS_STATUS_INVALID_VALUE, "short stride rejected");
  Check(Product(gpu, 0, 1, gpu.a, 2, -1, 2, gpu.b, 2, 2, 0, gpu.c, 2)
        == CUBLAS_STATUS_INVALID_VALUE, "negative size rejected");
  Check(Product(gpu, 0, 1, gpu.a, 2147483647, 2147483647, 2147483647,
                gpu.b, 2147483647, 2147483647, 0, gpu.c, 2147483647)
        == CUBLAS_STATUS_INVALID_VALUE, "overflowing byte span rejected");
  Check(Product(gpu, 0, 1, nullptr, 2, 2, 2, gpu.b, 2, 2, 0, gpu.c, 2)
        == CUBLAS_STATUS_INVALID_VALUE, "required nil input rejected");
  Check(Product(gpu, 0, 1, gpu.a, 2, 2, 2, gpu.b, 2, 2, 0, nullptr, 2)
        == CUBLAS_STATUS_INVALID_VALUE, "required nil output rejected");
  Check(Cuda::GemvR(nullptr, 1, gpu.a, 2, 2, 2, gpu.x, 0, gpu.c)
        == CUBLAS_STATUS_NOT_INITIALIZED, "GEMV nil handle rejected");
  Check(Cuda::GemvR(gpu.handle, 1, gpu.a, 1, 2, 2, gpu.x, 0, gpu.c)
        == CUBLAS_STATUS_INVALID_VALUE, "GEMV short stride rejected");
  Check(Cuda::GemvR(gpu.handle, 1, gpu.a, 2, -1, 2, gpu.x, 0, gpu.c)
        == CUBLAS_STATUS_INVALID_VALUE, "GEMV negative size rejected");
  Check(Cuda::GemvR(gpu.handle, 1, gpu.a, 2, 2, 2, nullptr, 0, gpu.c)
        == CUBLAS_STATUS_INVALID_VALUE, "GEMV nil vector rejected");
  Check(Cuda::GemvR(gpu.handle, 1, gpu.a, 2, 2, 2, gpu.x, 0, nullptr)
        == CUBLAS_STATUS_INVALID_VALUE, "GEMV nil output rejected");
  if (!Blas(cublasSetPointerMode(gpu.handle, CUBLAS_POINTER_MODE_DEVICE), "test device mode")) return;
  Check(Product(gpu, 0, 1, gpu.a, 2, 2, 2, gpu.b, 2, 2, 0, gpu.c, 2)
        == CUBLAS_STATUS_INVALID_VALUE, "device scalar mode rejected");
  Check(Cuda::GemvR(gpu.handle, 1, gpu.a, 2, 2, 2, gpu.x, 0, gpu.c)
        == CUBLAS_STATUS_INVALID_VALUE, "GEMV device scalar mode rejected");
  cublasPointerMode_t mode;
  if (!Blas(cublasGetPointerMode(gpu.handle, &mode), "get mode")) return;
  Check(mode == CUBLAS_POINTER_MODE_DEVICE, "adapter did not change pointer mode");
  if (!Blas(cublasSetPointerMode(gpu.handle, CUBLAS_POINTER_MODE_HOST), "restore host mode")) return;
  cudaStream_t stream;
  if (!Blas(cublasGetStream(gpu.handle, &stream), "get stream")) return;
  Check(stream == gpu.stream, "adapter did not change stream");
  if (!gpu.Get(actual, gpu.c)) return;
  Check(Equal(actual, expected), "rejected requests did not alter output");
}

int main(int argc, char** argv) {
  const ISC device = argc > 1 ? ISC(atoi(argv[1])) : 0;
  TCudaTest gpu;
  if (!gpu.Init(device)) return 1;
  UninitializedOutputs(gpu);
  Matrices(gpu);
  Vectors(gpu);
  Edges(gpu);
  gpu.Close();
  gpu.Close();
  printf("CudaBlasTests: device=%d cases=%d failures=%d\n", device, cases, failures);
  return failures != 0;
}
