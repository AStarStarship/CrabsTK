// Copyright AStarship <https://astarship.net>.
// Include the adapter before C/C++ system headers: namespace leaks must fail.
#include "Blas.h"
#include "../../ASCIICrabs/_Config.h"
#include "../../ASCIICrabs/Math.hxx"

using namespace _::Math;

static_assert(__is_same(FPD, double));
static_assert(__is_same(ISC, int));
static_assert(sizeof(FPD) == 8 && sizeof(ISC) == 4);
static_assert(sizeof(ISW) == sizeof(void*));

static ISC failures = 0;
static ISC checks = 0;

static void Check(BOL pass, const CHA* name) {
  ++checks;
  if (!pass) {
    ++failures;
    printf("FAIL: %s\n", name);
  }
}

static void Product(ISC op, FPD alpha, const FPD* a, ISC lda, ISC m,
                    ISC k, const FPD* b, ISC ldb, ISC n, FPD beta,
                    FPD* c, ISC ldc) {
  switch (op) {
    case 0: GemmRR(alpha, a, lda, m, k, b, ldb, n, beta, c, ldc); break;
    case 1: GemmTNR(alpha, a, lda, k, m, b, ldb, n, beta, c, ldc); break;
    case 2: GemmRNT(alpha, a, lda, m, k, b, ldb, n, beta, c, ldc); break;
    case 3: GemmTTT(alpha, a, lda, k, m, b, ldb, n, beta, c, ldc); break;
  }
}

static void MatrixCases() {
  const ISC shapes[][3] = {{3, 5, 2}, {1, 4, 7}, {6, 1, 3}, {7, 5, 9}};
  const FPD factors[][2] = {{1, 0}, {-0.75, 0.5}, {0, 0}, {0, -2}, {1, 1}};
  for (const auto& shape : shapes) {
    const ISC m = shape[0], k = shape[1], n = shape[2];
    for (ISC op = 0; op < 4; ++op) {
      const BOL ta = (op & 1) != 0, tb = (op & 2) != 0;
      for (ISC pad = 0; pad < 2; ++pad) {
        const ISC lda = (ta ? m : k) + pad * 3;
        const ISC ldb = (tb ? k : n) + pad * 2;
        const ISC ldc = n + pad * 4;
        FPD a[256], b[256], c[256], expected[256];
        for (ISC i = 0; i < 256; ++i) {
          a[i] = FPD((i * 7) % 17 - 8) / 8;
          b[i] = FPD((i * 3) % 19 - 9) / 16;
        }
        for (const auto& factor : factors) {
          const FPD alpha = factor[0], beta = factor[1];
          for (ISC i = 0; i < 256; ++i) c[i] = expected[i] = 9123.25;
          // Offset outputs and preserve canaries/padding. beta=0 must not read NaN.
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
          Product(op, alpha, a + pad, lda, m, k, b + pad * 2, ldb,
                  n, beta, c + 1, ldc);
          BOL pass = true;
          for (ISC i = 0; i < 256; ++i) {
            if (!isfinite(c[i]) || fabs(c[i] - expected[i]) > 1e-12) pass = false;
            if (a[i] != FPD((i * 7) % 17 - 8) / 8 ||
                b[i] != FPD((i * 3) % 19 - 9) / 16) pass = false;
          }
          Check(pass, "GEMM rectangular/transpose/stride/alpha/beta/canaries");
        }
      }
    }
  }
}

static void VectorCases() {
  for (ISC pad = 0; pad < 2; ++pad) {
    const ISC m = 3, n = 5, lda = n + pad * 2;
    FPD a[32], x[5] = {1, -2, 3, -4, 5};
    for (ISC i = 0; i < 32; ++i) a[i] = FPD(i - 8) / 8;
    for (ISC mode = 0; mode < 3; ++mode) {
      const FPD alpha = mode == 2 ? 0 : -0.5;
      const FPD beta = mode == 0 ? 0 : 2;
      FPD y[5] = {99, NAN, NAN, NAN, 99};
      FPD expected[3];
      for (ISC i = 0; i < m; ++i) {
        if (beta != 0) y[i + 1] = FPD(i - 1);
        FPD sum = 0;
        for (ISC j = 0; j < n; ++j) sum += a[i * lda + j] * x[j];
        expected[i] = alpha * sum + (beta == 0 ? 0 : beta * y[i + 1]);
      }
      GemvR(alpha, a, lda, m, n, x, beta, y + 1);
      BOL pass = y[0] == 99 && y[4] == 99;
      for (ISC i = 0; i < m; ++i)
        if (!isfinite(y[i + 1]) || fabs(y[i + 1] - expected[i]) > 1e-12) pass = false;
      Check(pass, "GEMV rectangular/stride/alpha/beta/canaries");
    }
  }
}

static void EdgeCases() {
  for (ISC op = 0; op < 4; ++op) {
    // Empty outputs never touch pointers, including nil pointers.
    Product(op, 1, nullptr, 1, 0, 3, nullptr, 1, 2, 0, nullptr, 2);
    Product(op, 1, nullptr, 1, 3, 2, nullptr, 1, 0, 0, nullptr, 1);
    Check(true, "empty GEMM outputs");
    FPD c[8] = {99, NAN, NAN, 99, NAN, NAN, 99, 99};
    Product(op, 1, nullptr, 2, 2, 0, nullptr, 2, 2, 0, c + 1, 3);
    Check(c[0] == 99 && c[1] == 0 && c[2] == 0 && c[3] == 99 &&
          c[4] == 0 && c[5] == 0 && c[6] == 99, "empty contraction scales only output");
    Product(op, 0, nullptr, 2, 2, 3, nullptr, 2, 2, 1, c + 1, 3);
    Check(c[1] == 0 && c[5] == 0, "alpha zero does not read inputs");
    const FPD a[4] = {1, 2, 3, 4};
    FPD expected[4] = {};
    for (ISC i = 0; i < 2; ++i)
      for (ISC j = 0; j < 2; ++j)
        for (ISC p = 0; p < 2; ++p)
          expected[i * 2 + j] += a[(op & 1) ? p * 2 + i : i * 2 + p] *
                                 a[(op & 2) ? j * 2 + p : p * 2 + j];
    Product(op, 1, a, 2, 2, 2, a, 2, 2, 0, c, 2);
    Check(MaxErr(c, expected, 4) == 0, "read-only input alias allowed");
  }
  GemvR(1, nullptr, 1, 0, 2, nullptr, 0, nullptr);
  FPD y[3] = {NAN, NAN, 99};
  GemvR(1, nullptr, 1, 2, 0, nullptr, 0, y);
  Check(y[0] == 0 && y[1] == 0 && y[2] == 99, "empty GEMV contraction");
  GemvR(0, nullptr, 1, 2, 3, nullptr, 1, y);
  Check(y[0] == 0 && y[1] == 0, "alpha-zero GEMV does not read inputs");
  FPD data[4] = {99, 1, 2, 99};
  ZeroMatrix(data + 1, 2);
  Check(data[0] == 99 && data[1] == 0 && data[2] == 0 && data[3] == 99,
        "ZeroMatrix bounds");
  const FPD a[2] = {1, 4}, b[2] = {-1, 5}, invalid[2] = {NAN, 5};
  Check(MaxErr(a, b, 2) == 2, "MaxErr finite values");
  Check(isinf(MaxErr(a, invalid, 2)), "MaxErr must not hide NaNs");
  Check(MaxErr(nullptr, nullptr, 0) == 0, "empty MaxErr");
  ZeroMatrix(nullptr, 0);
  BlasInit();
}

int main() {
  MatrixCases();
  VectorCases();
  EdgeCases();
  printf("BlasTests: checks=%d failures=%d\n", checks, failures);
  return failures != 0;
}
