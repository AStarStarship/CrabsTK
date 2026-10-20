# CrabsTK Math — host and device BLAS

Current implementation: FP64 row-major GEMM (all four transpose combinations)
and GEMV, with an explicit scalar or Intel MKL host backend. `CudaBlas.h`
provides a separate cuBLAS device-buffer API in `_::Math::Cuda`; see
[CudaBlas.md](CudaBlas.md) for the handle/stream/status/lifetime contract.

## Build and test

Run from the CrabsTK root. Keep generated files outside the source tree.
ASCIICrabs must remain the sibling repository; Math uses its canonical
configuration/types, not a second set of typedefs.

Scalar only (no BLAS SDK or library required):

    cmake -S Math -B "$TMPDIR/CrabsTKMath/ScalarOnly" -DCMAKE_BUILD_TYPE=Release
    cmake --build "$TMPDIR/CrabsTKMath/ScalarOnly" -j 4
    ctest --test-dir "$TMPDIR/CrabsTKMath/ScalarOnly" --output-on-failure

MKL, using an existing Intel oneAPI installation or an isolated SDK prefix:

    cmake -S Math -B "$TMPDIR/CrabsTKMath/Release" \
      -DCMAKE_BUILD_TYPE=Release -DCRABSTK_MATH_ENABLE_MKL=ON \
      -DCMAKE_PREFIX_PATH=/absolute/path/to/mkl/prefix
    cmake --build "$TMPDIR/CrabsTKMath/Release" -j 4
    ctest --test-dir "$TMPDIR/CrabsTKMath/Release" --output-on-failure

For host-only ASan/UBSan, choose a fresh build directory and add
`-DCRABSTK_MATH_SANITIZE=ON -DCMAKE_BUILD_TYPE=Debug`. CUDA device checking and
the tested CUDA/host-ASan incompatibility are documented in CudaBlas.md.

CUDA, on a machine with an existing SDK and accessible NVIDIA devices:

    cmake -S Math -B "$TMPDIR/CrabsTKMath/CudaRelease" \
      -DCMAKE_BUILD_TYPE=Release -DCRABSTK_MATH_ENABLE_CUDA=ON \
      -DCUDAToolkit_ROOT=/usr/local/cuda-13.0 \
      '-DCRABSTK_MATH_CUDA_TEST_DEVICES=0;1'
    cmake --build "$TMPDIR/CrabsTKMath/CudaRelease" -j 4
    ctest --test-dir "$TMPDIR/CrabsTKMath/CudaRelease" -V --output-on-failure

Select only device 0 on a single-GPU host. CUDA is OFF by default; when ON,
CMake requires the real CUDA toolkit/cuBLAS rather than falling back silently.
Link `CrabsTK::MathCuda` and include `CudaBlas.h`. No custom CUDA kernel or nvcc
compilation is required by this C++ adapter; CMake locates the SDK libraries.
MKL and CUDA options are independent and need not be enabled together.

The MKL option is OFF by default. When ON it requires Intel's MKL CMake
package, headers and libraries and fails configuration if missing. It never
reports a scalar result as an MKL run. MKL's package is explicitly configured
for LP64 integers, sequential threading and dynamic component linking
(`mkl_intel_lp64`, `mkl_sequential`, `mkl_core`). This avoids a runtime-selected
ILP64 interface and surprise nested OpenMP/TBB pools. A compile-time check
rejects `MKL_ILP64` on the MKL header path.

CMake consumers can add this directory and link `CrabsTK::MathScalar` or,
when enabled, `CrabsTK::MathMkl`. Include `Blas.h`. Backend definitions must
be consistent across all translation units in one executable.

Without CMake, include the sibling ASCIICrabs `_Seams` directory in compiler
include paths. The default needs no BLAS library. Define
`CRABSTK_MATH_MKL=1` and link the correctly configured MKL LP64 libraries for
MKL. Merely linking a library does not select the accelerated backend.

The older `CBLAS_H_AVAILABLE=1` switch now calls real CBLAS; `=0` selects the
scalar path. It includes `<cblas.h>` by default. For a provider with a different
header, define `CRABSTK_MATH_CBLAS_HEADER` to its quoted header name, e.g.
`"mkl_cblas.h"`. Supply that provider's include paths and libraries yourself.
The compatibility CTest uses real MKL CBLAS, not a fake provider. OpenBLAS and
other providers have not been independently verified here.

## API and buffer contract

Existing function names, argument ordering and void return types are preserved.
`FPD` and `ISC` are canonical ASCIICrabs aliases for double and 32-bit int on
the tested Linux build. `ISW` is used before stride/index multiplication.

    GemmRR:  C[m,n] = alpha * A[m,k]   * B[k,n]   + beta * C[m,n]
    GemmTNR: C[m,n] = alpha * A[k,m]^T * B[k,n]   + beta * C[m,n]
    GemmRNT: C[m,n] = alpha * A[m,k]   * B[n,k]^T + beta * C[m,n]
    GemmTTT: C[m,n] = alpha * A[k,m]^T * B[n,k]^T + beta * C[m,n]
    GemvR:   y[m]   = alpha * A[m,n]   * x[n]     + beta * y[m]

All matrix buffers are HOST row-major; each leading dimension is a positive
row stride in ELEMENTS of the stored, untransposed matrix. It must be at least
that stored matrix's column count (or 1 for zero columns). Vectors are contiguous
with unit increment. Padding between rows is not read as input or overwritten
as output. An offset pointer may select a submatrix.

These are low-level, unchecked BLAS routines, not shape-owning containers.
Caller preconditions:

- Dimensions/counts are nonnegative ISC values. Strides are positive ISC values.
- Each accessed buffer is a properly aligned, live FPD array of sufficient size.
  The final accessed offset and byte span must fit the host ISW/pointer domain.
  Actual allocation lengths cannot be inferred from a pointer.
- Output must not overlap any input that is read, even partially. No in-place
  GEMM, or GEMV with x/y sharing storage. Read-only A/B aliasing is supported.
- No concurrent mutation of input or shared output. Disjoint caller-owned output
  buffers may be used independently. Scalar code has no shared mutable state.
- Violating shape, capacity, alias or lifetime preconditions is unsupported;
  the void API does not promise validation, an exception or an error status.

Zero output dimensions are a no-op. Zero contraction dimension (k for GEMM,
n for GEMV) or alpha=0 scales only the output, without reading A/B/x; those
unused input pointers may be nil. With beta=0, old output is NOT read, so NaN
or uninitialized output can safely be replaced. beta=1 on a scale-only call
leaves output untouched. Padding remains untouched in all these cases.
`ref_dgemm`/`ref_dgemv` remain scalar compatibility helpers with the same rules.

`ZeroMatrix` writes exactly count values. `MaxErr` returns infinity for a
non-finite difference rather than silently hiding NaNs; its count=0 result is
zero. The finite numerical test tolerance is 1e-12 for exactly representable,
small test fixtures, not a universal error bound for arbitrary BLAS workloads.

## Lifetime and thread policy

Buffers are borrowed only for the duration of these synchronous host calls.
The adapter owns no buffers, threads or library handles and contains no explicit
allocation, file I/O or waits. Vendor libraries can perform lazy initialization
or allocate internally. This is NOT a hard-real-time guarantee; do library setup
and representative warm-up outside a control/physics tick. Scalar headless
physics remains independent of MKL.

`BlasInit()` is preserved as a no-op: it never changes process-wide policy.
The provided MKL CMake target uses the sequential component libraries. If an
application instead links threaded MKL or a different CBLAS provider, it owns
that provider's documented thread controls (e.g. MKL_NUM_THREADS/MKL_DYNAMIC
before startup or an explicitly restored per-thread policy). No unverified
machine-specific timing or speedup claims are part of this API.

## Verification scope

`BlasTests.cpp` covers rectangular/tight/padded/offset matrices, every transpose
combination, alpha/beta including beta=0 over NaNs, empty dimensions, scale-only
calls with nil inputs, input aliasing, output canaries, helpers and type identity.
The original `blas_test.cpp` is also built and run; its old tracked binaries are
not updated. Release and ASan/UBSan tests run the same source against scalar,
macro-zero, MKL and the legacy CBLAS selection. Sanitizers instrument this code
and the harness, not Intel's prebuilt library internals.

`CudaBlasTests.cpp` uses preallocated device buffers, a caller-owned non-default
stream, explicit upload/download/completion and an independent CPU reference.
It exercises both physical GPUs when requested, including actual uninitialized
device output with beta=0, malformed shape/span/pointer-mode rejection and
unchanged handle policy. NVIDIA Compute Sanitizer checks device accesses;
UBSan checks host code. See CudaBlas.md for commands and the host-ASan limitation.
No full CrabsTK/iGeek suite, Windows portability or acceleration speedup is
claimed by these focused numerical tests.

The MUSEWorld consumer currently uses COLUMN-major TMatrix storage and calls
cblas_dgemm directly. Do not pass those buffers to these row-major wrappers
without an explicit conversion/layout-aware integration. That sibling source
has not been modified by this slice.
