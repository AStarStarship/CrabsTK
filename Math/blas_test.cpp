// Copyright AStarship <https://astarship.net>.
// Self-test for CrabsTK/Math/Blas.h: verify each row-major product op against a
// naive FP64 reference. Build:
//   g++ -std=c++23 -O2 -I<CrabsTK>/Math blas_test.cpp -lmkl_rt
// Exit 0 = all ops match reference to < 1e-9.
#include "Blas.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace _;
using namespace _::Math;

static double MaxErr(const std::vector<double>& a, const std::vector<double>& b) {
  double e = 0.0;
  for (size_t i = 0; i < a.size(); i++) e = std::max(e, std::fabs(a[i] - b[i]));
  return e;
}

int main() {
  const int m = 7, k = 5, n = 9;
  std::vector<double> A(m * k), B(k * n), At(k * m), Bt(n * k), x(n);
  // deterministic fill
  auto fill = [](std::vector<double>& v, int seed) {
    unsigned s = (unsigned)seed;
    for (auto& d : v) { s = s * 1103515245u + 12345u; d = (double)(s & 0xffff) / 32768.0 - 1.0; }
  };
  fill(A, 1); fill(B, 2); fill(At, 3); fill(Bt, 4); fill(x, 5);
  for (auto& d : x) d *= 0.5;

  int fails = 0;
  auto check = [&](const char* name, double err) {
    if (err < 1e-9) printf("PASS: %-8s max_err=%.3e\n", name, err);
    else { printf("FAIL: %-8s max_err=%.3e\n", name, err); fails++; }
  };

  // 1) GemmRR: C = A * B  (m x k)(k x n) -> (m x n)
  {
    std::vector<double> C(m * n, 0.0);
    GemmRR(1.0, A.data(), k, m, k, B.data(), n, n, 0.0, C.data(), n);
    std::vector<double> ref(m * n, 0.0);
    for (int i = 0; i < m; i++) for (int p = 0; p < k; p++)
      for (int j = 0; j < n; j++) ref[i * n + j] += A[i * k + p] * B[p * n + j];
    check("RR", MaxErr(C, ref));
  }
  // 2) GemmTNR: C = At^T * B  where At is (k x m) row-major, At^T (m x k)
  {
    std::vector<double> C(m * n, 0.0);
    GemmTNR(1.0, At.data(), m, k, m, B.data(), n, n, 0.0, C.data(), n);
    std::vector<double> ref(m * n, 0.0);
    // At^T[i][p] = At[p][i]
    for (int i = 0; i < m; i++) for (int p = 0; p < k; p++)
      for (int j = 0; j < n; j++) ref[i * n + j] += At[p * m + i] * B[p * n + j];
    check("TNR", MaxErr(C, ref));
  }
  // 3) GemmRNT: C = A * Bt^T  where Bt is (n x k) row-major, Bt^T (k x n)
  {
    std::vector<double> C(m * n, 0.0);
    GemmRNT(1.0, A.data(), k, m, k, Bt.data(), k, n, 0.0, C.data(), n);
    std::vector<double> ref(m * n, 0.0);
    // Bt^T[p][j] = Bt[j][p]
    for (int i = 0; i < m; i++) for (int p = 0; p < k; p++)
      for (int j = 0; j < n; j++) ref[i * n + j] += A[i * k + p] * Bt[j * k + p];
    check("RNT", MaxErr(C, ref));
  }
  // 4) GemmTTT: C = At^T * Bt^T  (m x k)(k x n) -> (m x n)
  {
    std::vector<double> C(m * n, 0.0);
    GemmTTT(1.0, At.data(), m, k, m, Bt.data(), k, n, 0.0, C.data(), n);
    std::vector<double> ref(m * n, 0.0);
    for (int i = 0; i < m; i++) for (int p = 0; p < k; p++)
      for (int j = 0; j < n; j++) ref[i * n + j] += At[p * m + i] * Bt[j * k + p];
    check("TTT", MaxErr(C, ref));
  }
  // 5) GemvR: y = Ag * x  where Ag is (m x n) row-major, x (n) -> y (m)
  {
    std::vector<double> Ag(m * n);
    { unsigned s = 7; for (auto& d : Ag) { s = s * 1103515245u + 12345u; d = (double)(s & 0xffff) / 32768.0 - 1.0; } }
    std::vector<double> y(m, 0.0);
    GemvR(1.0, Ag.data(), n, m, n, x.data(), 0.0, y.data());
    std::vector<double> ref(m, 0.0);
    for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) ref[i] += Ag[i * n + j] * x[j];
    check("gemv", MaxErr(y, ref));
  }

  // 6) beta accumulation: C = alpha*A*B + beta*C0
  {
    std::vector<double> C0(m * n); fill(C0, 9);
    std::vector<double> C = C0;
    GemmRR(0.5, A.data(), k, m, k, B.data(), n, n, 2.0, C.data(), n);
    std::vector<double> ref(m * n);
    for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) {
      ref[i * n + j] = 2.0 * C0[i * n + j];
      for (int p = 0; p < k; p++) ref[i * n + j] += 0.5 * A[i * k + p] * B[p * n + j];
    }
    check("alpha/beta", MaxErr(C, ref));
  }

  printf(fails ? "BLAS SELF-TEST: %d FAILED\n" : "BLAS SELF-TEST: ALL PASS\n", fails);
  return fails ? 1 : 0;
}
