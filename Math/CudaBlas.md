# CUDA BLAS contract

`CudaBlas.h` exposes `_::Math::Cuda::GemmRR`, `GemmTNR`, `GemmRNT`,
`GemmTTT` and `GemvR`. Each takes a borrowed `cublasHandle_t` first,
followed by the same scalars, dimensions and element strides as the host API,
and returns `cublasStatus_t`. This is an explicit DEVICE-buffer interface;
it never replaces host calls or uploads host matrices implicitly.

The caller creates/configures/destroys the handle and its stream outside a
control tick. The current CUDA device must match the handle/buffers. Configure
CUBLAS_POINTER_MODE_HOST: alpha/beta are value arguments copied through host
scalars to cuBLAS. Device pointer mode is rejected, not silently changed. The
adapter does not create a context/stream, change pointer/math/stream policy,
allocate buffers, transfer data or synchronize. Vendor initialization/workspace
allocation can still occur; no hard-real-time guarantee is made.

Matrices/vectors must be live, aligned, sufficiently large device allocations
with row-major matrix storage and positive row strides in FPD elements. Vectors
have unit stride. Buffers must remain live and inputs unchanged until the
caller's stream/event indicates completion. Outputs must not overlap inputs
that are read; input/input aliasing is allowed. No cross-device pointers,
concurrent handle reconfiguration, or overlapping-output mutation is supported.

Negative dimensions, undersized/nonpositive strides, overflowing address spans,
and required nil pointers return CUBLAS_STATUS_INVALID_VALUE before submission.
A nil handle returns CUBLAS_STATUS_NOT_INITIALIZED. Device pointer mode returns
CUBLAS_STATUS_INVALID_VALUE. This does not validate allocation capacities,
device identity, stale non-nil handles or arbitrary pointer accessibility.

Empty output dimensions are a no-op after handle/shape/pointer-mode validation.
A zero contraction or alpha=0 scales output only, and unused input pointers may
be nil. beta=0 overwrites without reading old NaNs/uninitialized output; beta=1
leaves scale-only output untouched. Padding is preserved. Scaling uses cuBLAS
GEAM's documented in-place/no-transpose mode and zero-factor input omission,
not SCAL's 0*NaN behavior. GEMM swaps operands/dimensions for the column-major
cuBLAS view; GEMV transposes that view.

SUCCESS means cuBLAS accepted the call; it does NOT mean the GPU completed it.
Check both returned cuBLAS status and the caller-owned stream/event completion
status. The adapter returns vendor submission errors unchanged. Subsequent CUDA
execution faults must be handled at the application's explicit synchronization
boundary. No errors are converted into successful scalar fallback results.

Reference: NVIDIA cuBLAS documentation, scalar parameters and GEAM:
https://docs.nvidia.com/cuda/cublas/index.html#scalar-parameters
https://docs.nvidia.com/cuda/cublas/index.html#cublas-t-geam

## Build and verification

See README.md for the CUDA-enabled Release build. The CTest GPU executable
accepts one CUDA device ordinal (default 0). The CMake cache variable
CRABSTK_MATH_CUDA_TEST_DEVICES selects a semicolon-separated list for CTest.
Missing devices are failures, never successful skips.

For device memory errors and leaks, use an installed NVIDIA tool:

    compute-sanitizer --tool memcheck --leak-check full --error-exitcode 99 \
      /absolute/build/path/CudaBlasTests 0
    compute-sanitizer --tool initcheck --error-exitcode 99 \
      /absolute/build/path/CudaBlasTests 0

Repeat for every selected device. Run a separate host UBSan build with
CRABSTK_MATH_ENABLE_CUDA=ON, CRABSTK_MATH_SANITIZE=OFF and explicit flags:

    -DCMAKE_BUILD_TYPE=Debug
    '-DCMAKE_CXX_FLAGS=-O1 -g -fsanitize=undefined -fno-sanitize-recover=all -fno-omit-frame-pointer'
    -DCMAKE_EXE_LINKER_FLAGS=-fsanitize=undefined

This is explicitly UBSan-only, not a claimed ASan pass. On the tested remote
host (driver 595.99.02, CUDA toolkit 13.0), ASan executables return CUDA error 2,
out of memory, at cudaSetDevice before any Math/cuBLAS operation. A minimal
program containing only cudaSetDevice reproduces the failure with
`-fsanitize=address`, while unsanitized and UBSan versions succeed with idle
GPUs. The exact driver/address-space mechanism has not been established.
Do not disable ASan silently or blame GEMM for that initialization failure.
The CMake CRABSTK_MATH_SANITIZE option still genuinely enables ASan/UBSan;
CUDA builds using it can expose this host/runtime limitation. Device memcheck
and initcheck are separate checks, not substitutes labeled as host ASan.
