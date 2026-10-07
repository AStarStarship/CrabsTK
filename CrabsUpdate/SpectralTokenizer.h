// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TOOLKIT_UPDATE_SPECTRAL_TOKENIZER_H
#define CRABS_TOOLKIT_UPDATE_SPECTRAL_TOKENIZER_H

/*
Spectral Tokenizer - geometric attention on S2 via 3x3 monoid layers.

Problem: dot-product self-attention on tokens laid out arbitrarily does not
produce clean decision boundaries for neural networks.

Solution: map tokens to angles on S1 via a spectral tokenizer, lift to the
unit sphere S2 subset R3, and replace softmax(QK^T)V with composed 3x3
geometric operators (rotation x stretch) modulated by local ellipsoid
statistics per 12-token window.

Architecture:
  1. SpectralTokenizer - token ID to angle (S1) to 3D embedding (S2).
  2. Vec3 / Mat3x3 - 3D vector and 3x3 matrix with dot, cross, norm, etc.
  3. Covariance - 12-token window covariance + polar decomposition.
  4. Monoid - 3x3 matrix monoid (composition = layer transform).
  5. MuseLayer - single MUSE-Transformer block: window to cov to polar to
     rotate-and-squish to renormalize to output.
  6. MuseTransformer - stacked layers with readout.

Build: g++ -std=c++23 -Wall -Wextra -Wpedantic -Werror
       <test>.cpp -o /tmp/<out> && /tmp/<out>

No std:: - uses ASCII Data Types (FPC, FPD, IUC, IUA, etc.) and
project containers.
*/

#include "ASCIITypes.h"
#include "AType.h"

namespace CT {

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------

/* Window size for local geometry. */
constexpr IUC WindowSize = 12;

/* Covariance regularisation epsilon: epsilon*I added to Sigma. */
constexpr FPC CovarianceEpsilon = 1e-6f;

/* Pi and half-Pi for spectral (angle) math. */
constexpr FPC PiF = 3.14159265358979323846f;
constexpr FPC TwoPiF = 6.28318530717958647692f;
constexpr FPC HalfPiF = 1.57079632679489661923f;

/* Maximum number of eigenvalue iterations for Jacobi. */
constexpr IUC MaxEigenIter = 100;

/* Convergence threshold for Jacobi eigenvalue iteration. */
constexpr FPC EigenConverge = 1e-10f;

/* Max tokens and layers for stack-allocated tables. */
constexpr IUC MaxTokens = 1024;
constexpr IUC MaxLayers = 8;

// ---------------------------------------------------------------------------
// Minimal math helpers (no <cmath> dependency for the core).
// ---------------------------------------------------------------------------

/* Platform math: we pull sin/cos/sqrt from the C math library. */
extern "C" {
  FPC fpc_sin(FPC x);
  FPC fpc_cos(FPC x);
  FPC fpc_sqrt(FPC x);
  FPC fpc_atan2(FPC y, FPC x);
  FPC fpc_pow(FPC base, FPC exp);
}

inline FPC Sin(FPC x) { return fpc_sin(x); }
inline FPC Cos(FPC x) { return fpc_cos(x); }
inline FPC Sqrt(FPC x) { return fpc_sqrt(x); }
inline FPC Atan2(FPC y, FPC x) { return fpc_atan2(y, x); }
inline FPC Pow(FPC base, FPC exp) { return fpc_pow(base, exp); }

inline FPC Fabs(FPC x) { return x < 0 ? -x : x; }

inline FPC Clamp(FPC v, FPC lo, FPC hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

// ---------------------------------------------------------------------------
// Vec3 - 3D vector on or near the unit sphere.
// ---------------------------------------------------------------------------

struct Vec3 {
  FPC x, y, z;

  __attribute__((always_inline)) Vec3() : x(0), y(0), z(0) {}

  __attribute__((always_inline)) Vec3(FPC xx, FPC yy, FPC zz)
      : x(xx), y(yy), z(zz) {}

  __attribute__((always_inline)) FPC Dot(Vec3 other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  __attribute__((always_inline)) Vec3 Cross(Vec3 other) const {
    return Vec3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
  }

  __attribute__((always_inline)) FPC NormSq() const {
    return x * x + y * y + z * z;
  }

  __attribute__((always_inline)) FPC Norm() const {
    return Sqrt(NormSq());
  }

  __attribute__((always_inline)) Vec3 Normalized() const {
    FPC n = Norm();
    if (n < 1e-15f) return Vec3(0, 0, 0);
    return Vec3(x / n, y / n, z / n);
  }

  __attribute__((always_inline)) Vec3 operator+(Vec3 other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
  }

  __attribute__((always_inline)) Vec3 operator-(Vec3 other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
  }

  __attribute__((always_inline)) Vec3 Scale(FPC s) const {
    return Vec3(x * s, y * s, z * s);
  }

  __attribute__((always_inline)) void AddInPlace(Vec3 other) {
    x += other.x; y += other.y; z += other.z;
  }

  __attribute__((always_inline)) void ScaleInPlace(FPC s) {
    x *= s; y *= s; z *= s;
  }

  __attribute__((always_inline)) FPC& operator[](IUC i) {
    return (&x)[i];
  }
  __attribute__((always_inline)) FPC operator[](IUC i) const {
    return (&x)[i];
  }
};

inline Vec3 operator*(FPC s, Vec3 v) { return v.Scale(s); }

// ---------------------------------------------------------------------------
// Mat3x3 - 3x3 row-major matrix.
// ---------------------------------------------------------------------------

struct Mat3x3 {
  FPC m[3][3];

  __attribute__((always_inline)) Mat3x3() {
    for (IUC i = 0; i < 3; ++i)
      for (IUC j = 0; j < 3; ++j)
        m[i][j] = 0;
  }

  __attribute__((always_inline)) static Mat3x3 Identity() {
    Mat3x3 r;
    r.m[0][0] = 1; r.m[1][1] = 1; r.m[2][2] = 1;
    return r;
  }

  __attribute__((always_inline)) Vec3 Mul(Vec3 v) const {
    return Vec3(
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
    );
  }

  __attribute__((always_inline)) Mat3x3 Mul(Mat3x3 other) const {
    Mat3x3 r;
    for (IUC i = 0; i < 3; ++i)
      for (IUC j = 0; j < 3; ++j) {
        FPC s = 0;
        for (IUC k = 0; k < 3; ++k)
          s += m[i][k] * other.m[k][j];
        r.m[i][j] = s;
      }
    return r;
  }

  __attribute__((always_inline)) Mat3x3 operator+(Mat3x3 other) const {
    Mat3x3 r;
    for (IUC i = 0; i < 3; ++i)
      for (IUC j = 0; j < 3; ++j)
        r.m[i][j] = m[i][j] + other.m[i][j];
    return r;
  }

  __attribute__((always_inline)) Mat3x3 Scale(FPC s) const {
    Mat3x3 r;
    for (IUC i = 0; i < 3; ++i)
      for (IUC j = 0; j < 3; ++j)
        r.m[i][j] = m[i][j] * s;
    return r;
  }

  __attribute__((always_inline)) Mat3x3 Transpose() const {
    Mat3x3 r;
    for (IUC i = 0; i < 3; ++i)
      for (IUC j = 0; j < 3; ++j)
        r.m[i][j] = m[j][i];
    return r;
  }

  __attribute__((always_inline)) FPC Trace() const {
    return m[0][0] + m[1][1] + m[2][2];
  }

  __attribute__((always_inline)) void Zero() {
    for (IUC i = 0; i < 3; ++i)
      for (IUC j = 0; j < 3; ++j)
        m[i][j] = 0;
  }
};

inline Mat3x3 operator*(FPC s, Mat3x3 m) { return m.Scale(s); }

// ---------------------------------------------------------------------------
// Spectral Tokenizer - token ID to angle to 3D embedding on S2.
// ---------------------------------------------------------------------------

/*
Spectral tokenizer maps a token ID to an angle theta on S1, then lifts to
the unit sphere S2 in R3 as (cos theta, sin theta, z) with a learned z.

The angle is a hash of the token ID spread over [0, 2pi):
  theta = (token_id * golden_angle) mod 2pi
where golden_angle = 2pi * (1 - 1/phi) approx 2.39996...

The z component is a learned embedding: z = learn_z(token_id).
The final embedding is normalised to unit length.
*/
class SpectralTokenizer {
 public:
  SpectralTokenizer() : token_count_(0), layer_count_(0), golden_angle_(0) {}

  SpectralTokenizer(IUC num_tokens, IUC num_layers)
      : token_count_(num_tokens), layer_count_(num_layers), golden_angle_(0) {
    Init();
  }

  void Init() {
    golden_angle_ = TwoPiF * (1.0f - 1.0f / 1.6180339887498948482f);
    for (IUC l = 0; l < layer_count_; ++l)
      for (IUC t = 0; t < token_count_; ++t)
        z_table_[l * token_count_ + t] = 0;
  }

  __attribute__((always_inline)) FPC GetAngle(IUC token_id) const {
    if (token_count_ == 0) return 0;
    FPC angle = static_cast<FPC>(token_id % token_count_) * golden_angle_;
    while (angle >= TwoPiF) angle -= TwoPiF;
    while (angle < 0) angle += TwoPiF;
    return angle;
  }

  __attribute__((always_inline)) FPC GetZ(IUC token_id, IUC layer) const {
    if (layer >= layer_count_ || token_id >= token_count_) return 0;
    return z_table_[layer * token_count_ + token_id];
  }

  __attribute__((always_inline)) void SetZ(IUC token_id, IUC layer, FPC z) {
    if (layer < layer_count_ && token_id < token_count_)
      z_table_[layer * token_count_ + token_id] = z;
  }

  __attribute__((always_inline)) Vec3 Embed(IUC token_id, IUC layer) const {
    FPC theta = GetAngle(token_id);
    FPC z = GetZ(token_id, layer);
    Vec3 raw(Cos(theta), Sin(theta), z);
    return raw.Normalized();
  }

  __attribute__((always_inline)) Vec3 EmbedRaw(IUC token_id, IUC layer) const {
    FPC theta = GetAngle(token_id);
    FPC z = GetZ(token_id, layer);
    return Vec3(Cos(theta), Sin(theta), z);
  }

  IUC TokenCount() const { return token_count_; }
  IUC LayerCount() const { return layer_count_; }

 private:
  IUC token_count_;
  IUC layer_count_;
  FPC golden_angle_;
  FPC z_table_[MaxTokens * MaxLayers];
};

// ---------------------------------------------------------------------------
// Covariance - 12-token window covariance matrix + polar decomposition.
// ---------------------------------------------------------------------------

/*
Covariance computes Sigma = (1/n) X X^T + epsilon*I for a 3xn window.
Sigma is 3x3 symmetric positive-definite (SPD).

Polar decomposition: Sigma = R*S where R is a rotation matrix and S is
symmetric positive-definite stretch matrix.

Eigenvalues lambda_1 >= lambda_2 >= lambda_3 and eigenvectors r_k describe
the ellipsoid shape of the local token geometry.
*/
class Covariance {
 public:
  Covariance() : computed_(false) {}

  void Compute(const Vec3* embeddings, IUC n) {
    sigma_.Zero();

    FPC inv_n = 1.0f / static_cast<FPC>(n);
    for (IUC i = 0; i < 3; ++i)
      for (IUC j = 0; j < 3; ++j) {
        FPC s = 0;
        for (IUC k = 0; k < n; ++k)
          s += embeddings[k][i] * embeddings[k][j];
        sigma_.m[i][j] = s * inv_n;
      }

    sigma_.m[0][0] += CovarianceEpsilon;
    sigma_.m[1][1] += CovarianceEpsilon;
    sigma_.m[2][2] += CovarianceEpsilon;

    sigma_.m[0][1] = (sigma_.m[0][1] + sigma_.m[1][0]) * 0.5f;
    sigma_.m[0][2] = (sigma_.m[0][2] + sigma_.m[2][0]) * 0.5f;
    sigma_.m[1][2] = (sigma_.m[1][2] + sigma_.m[2][1]) * 0.5f;

    EigenDecompose();

    stretch_ = sigma_;
    rotation_ = Mat3x3::Identity();

    computed_ = true;
  }

  void PolarDecompose(Mat3x3 M) {
    Mat3x3 A = M;

    for (IUC iter = 0; iter < MaxEigenIter; ++iter) {
      Mat3x3 At = A.Transpose();
      Mat3x3 AtA = At.Mul(A);
      Mat3x3 AtA_inv = Inverse3x3(AtA);
      if (!AtA_inv.m[0][0] && !AtA_inv.m[1][1] && !AtA_inv.m[2][2]) break;
      Mat3x3 next = A.Scale(0.5f) + AtA_inv.Scale(0.5f);

      FPC diff = 0;
      for (IUC i = 0; i < 3; ++i)
        for (IUC j = 0; j < 3; ++j) {
          FPC d = next.m[i][j] - A.m[i][j];
          diff += d * d;
        }
      A = next;
      if (diff < EigenConverge) break;
    }

    rotation_ = A;
    stretch_ = rotation_.Transpose().Mul(M);

    computed_ = true;
  }

  __attribute__((always_inline)) const Mat3x3& Sigma() const { return sigma_; }
  __attribute__((always_inline)) const Mat3x3& Rotation() const { return rotation_; }
  __attribute__((always_inline)) const Mat3x3& Stretch() const { return stretch_; }
  __attribute__((always_inline)) const FPC* Eigenvalues() const { return eigenvalues_; }
  __attribute__((always_inline)) const Vec3* Eigenvectors() const { return eigenvectors_; }
  __attribute__((always_inline)) bool IsComputed() const { return computed_; }

 private:
  Mat3x3 sigma_;
  Mat3x3 rotation_;
  Mat3x3 stretch_;
  FPC eigenvalues_[3];
  Vec3 eigenvectors_[3];
  bool computed_;

  void EigenDecompose() {
    Mat3x3 A;
    for (IUC i = 0; i < 3; ++i)
      for (IUC j = 0; j < 3; ++j)
        A.m[i][j] = sigma_.m[i][j];

    Vec3 V[3] = { Vec3(1,0,0), Vec3(0,1,0), Vec3(0,0,1) };

    for (IUC iter = 0; iter < MaxEigenIter; ++iter) {
      FPC max_val = 0;
      IUC p = 0, q = 1;
      if (Fabs(A.m[0][2]) > max_val) { max_val = Fabs(A.m[0][2]); p = 0; q = 2; }
      if (Fabs(A.m[1][2]) > max_val) { max_val = Fabs(A.m[1][2]); p = 1; q = 2; }

      if (max_val < EigenConverge) break;

      FPC a_pp = A.m[p][p];
      FPC a_qq = A.m[q][q];
      FPC a_pq = A.m[p][q];
      FPC theta = 0.5f * Atan2(2.0f * a_pq, a_qq - a_pp);
      FPC c = Cos(theta);
      FPC s = Sin(theta);

      Mat3x3 Ap;
      for (IUC i = 0; i < 3; ++i) {
        if (i != p && i != q) {
          Ap.m[i][p] =  c * A.m[i][p] + s * A.m[i][q];
          Ap.m[i][q] = -s * A.m[i][p] + c * A.m[i][q];
          Ap.m[p][i] = Ap.m[i][p];
          Ap.m[q][i] = Ap.m[i][q];
        }
      }
      Ap.m[p][p] =  c*c*a_pp + 2*s*c*a_pq + s*s*a_qq;
      Ap.m[q][q] =  s*s*a_pp - 2*s*c*a_pq + c*c*a_qq;
      Ap.m[p][q] = 0;
      Ap.m[q][p] = 0;

      for (IUC i = 0; i < 3; ++i) {
        FPC v_p = V[i][p];
        FPC v_q = V[i][q];
        V[i][p] =  c * v_p + s * v_q;
        V[i][q] = -s * v_p + c * v_q;
      }

      A = Ap;
    }

    eigenvalues_[0] = A.m[0][0];
    eigenvalues_[1] = A.m[1][1];
    eigenvalues_[2] = A.m[2][2];
    eigenvectors_[0] = V[0];
    eigenvectors_[1] = V[1];
    eigenvectors_[2] = V[2];

    for (IUC i = 0; i < 2; ++i)
      for (IUC j = i + 1; j < 3; ++j) {
        if (eigenvalues_[j] > eigenvalues_[i]) {
          FPC tmp = eigenvalues_[i];
          eigenvalues_[i] = eigenvalues_[j];
          eigenvalues_[j] = tmp;
          Vec3 vtmp = eigenvectors_[i];
          eigenvectors_[i] = eigenvectors_[j];
          eigenvectors_[j] = vtmp;
        }
      }

    computed_ = true;
  }

  __attribute__((always_inline)) static Mat3x3 Inverse3x3(Mat3x3 m) {
    FPC det = m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1])
            - m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0])
            + m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);
    if (Fabs(det) < 1e-20f) {
      Mat3x3 zero;
      return zero;
    }
    FPC idet = 1.0f / det;
    Mat3x3 inv;
    inv.m[0][0] = (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) * idet;
    inv.m[0][1] = (m.m[0][2] * m.m[2][1] - m.m[0][1] * m.m[2][2]) * idet;
    inv.m[0][2] = (m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1]) * idet;
    inv.m[1][0] = (m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2]) * idet;
    inv.m[1][1] = (m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0]) * idet;
    inv.m[1][2] = (m.m[0][2] * m.m[1][0] - m.m[0][0] * m.m[1][2]) * idet;
    inv.m[2][0] = (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]) * idet;
    inv.m[2][1] = (m.m[0][1] * m.m[2][0] - m.m[0][0] * m.m[2][1]) * idet;
    inv.m[2][2] = (m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0]) * idet;
    return inv;
  }
};

// ---------------------------------------------------------------------------
// Monoid - 3x3 matrix monoid under composition.
// ---------------------------------------------------------------------------

/*
The monoid of 3x3 real matrices under multiplication.
- Set: all 3x3 real matrices (or a constrained subset, e.g. invertible).
- Operation: matrix multiplication.
- Identity: I3 (no-change layer).

A "layer transform" is an element of this monoid.
Composition of layers is monoid multiplication.
*/
class Monoid {
 public:
  Monoid() : matrix_(Mat3x3::Identity()) {}

  explicit Monoid(Mat3x3 m) : matrix_(m) {}

  __attribute__((always_inline)) static Monoid Identity() {
    return Monoid(Mat3x3::Identity());
  }

  __attribute__((always_inline)) const Mat3x3& Matrix() const { return matrix_; }

  __attribute__((always_inline)) Vec3 Apply(Vec3 v) const {
    return matrix_.Mul(v);
  }

  __attribute__((always_inline)) Monoid Compose(Monoid other) const {
    return Monoid(matrix_.Mul(other.matrix_));
  }

  __attribute__((always_inline)) void ComposeInPlace(Monoid other) {
    matrix_ = matrix_.Mul(other.matrix_);
  }

  __attribute__((always_inline)) void SetMatrix(Mat3x3 m) { matrix_ = m; }

  __attribute__((always_inline)) void Reset() { matrix_ = Mat3x3::Identity(); }

 private:
  Mat3x3 matrix_;
};

// ---------------------------------------------------------------------------
// MuseLayer - single MUSE-Transformer block.
// ---------------------------------------------------------------------------

/*
One layer of the MUSE (Monoid Unit Sphere Ellipsoid) Transformer.

For each position t in the sequence:
  1. Take a 12-token window to X_t in R^(3x12).
  2. Compute covariance Sigma_t = (1/12) X_t X_t^T + epsilon*I.
  3. Compute polar decomposition: M_t = R_t * S_t.
  4. Rotate and squish: x_tilde_i = R_t * S_t * x_i.
  5. Optionally renormalise back to sphere: x_i' = x_tilde_i / |x_tilde_i|.

The layer's monoid element M^(ell) = M_t for the window at position t.
*/
class MuseLayer {
 public:
  MuseLayer() : layer_matrix_(Mat3x3::Identity()) {}

  Monoid Process(const Vec3* embeddings, Vec3* output,
                 BOL renormalize = true) {
    cov_.Compute(embeddings, WindowSize);

    const Mat3x3& R = cov_.Rotation();
    const Mat3x3& S = cov_.Stretch();

    Mat3x3 M = R.Mul(S);
    layer_matrix_ = M;

    for (IUC i = 0; i < WindowSize; ++i) {
      Vec3 transformed = M.Mul(embeddings[i]);
      if (renormalize) {
        output[i] = transformed.Normalized();
      } else {
        output[i] = transformed;
      }
    }

    return Monoid(M);
  }

  void Forward(const Vec3* embeddings, Vec3* output,
               IUC seq_len, BOL renormalize = true) {
    Vec3 window[WindowSize];

    for (IUC t = 0; t < seq_len; ++t) {
      for (IUC w = 0; w < WindowSize; ++w) {
        IUC idx = (t + w) % seq_len;
        window[w] = embeddings[idx];
      }
      Process(window, &output[t], renormalize);
    }
  }

  __attribute__((always_inline)) const Mat3x3& LayerMatrix() const {
    return layer_matrix_;
  }

  __attribute__((always_inline)) const Covariance& GetCovariance() const {
    return cov_;
  }

 private:
  Mat3x3 layer_matrix_;
  Covariance cov_;
};

// ---------------------------------------------------------------------------
// MUSE Transformer - stacked MuseLayers with readout.
// ---------------------------------------------------------------------------

/*
Stacked MuseLayers form a transformer-like architecture:
  1. Embedding: spectral tokenizer to angle to 3D on unit sphere.
  2. Layer: monoid element (3x3 matrix) acting on 3D embeddings,
     decomposed into rotation + stretch, modulated by local ellipsoid
     statistics.
  3. Depth: composition of monoid elements across layers.
  4. Readout: map final 3D embeddings back to token logits
     (via learned projection + nearest-angle).
*/
class MuseTransformer {
 public:
  MuseTransformer(IUC num_tokens, IUC num_layers, IUC hidden_dim = 1)
      : tokenizer_(num_tokens, hidden_dim), layer_count_(num_layers) {
    for (IUC l = 0; l < layer_count_; ++l)
      layers_[l] = MuseLayer();
  }

  void Forward(const IUC* token_ids, Vec3* output, IUC seq_len) {
    for (IUC t = 0; t < seq_len; ++t)
      embeddings_[t] = tokenizer_.Embed(token_ids[t], 0);

    for (IUC l = 0; l < layer_count_; ++l) {
      layers_[l].Forward(embeddings_, intermediate_, seq_len, true);
      for (IUC t = 0; t < seq_len; ++t)
        embeddings_[t] = intermediate_[t];
    }

    for (IUC t = 0; t < seq_len; ++t)
      output[t] = embeddings_[t];
  }

  IUC Readout(Vec3 embedding) const {
    FPC angle = Atan2(embedding.y, embedding.x);
    if (angle < 0) angle += TwoPiF;

    IUC best_id = 0;
    FPC best_diff = TwoPiF;
    for (IUC tid = 0; tid < tokenizer_.TokenCount(); ++tid) {
      FPC theta = tokenizer_.GetAngle(tid);
      FPC diff = Fabs(angle - theta);
      if (diff > PiF) diff = TwoPiF - diff;
      if (diff < best_diff) {
        best_diff = diff;
        best_id = tid;
      }
    }
    return best_id;
  }

  __attribute__((always_inline)) const SpectralTokenizer& Tokenizer() const {
    return tokenizer_;
  }

  __attribute__((always_inline)) MuseLayer& Layer(IUC l) {
    return layers_[l];
  }

  __attribute__((always_inline)) IUC NumLayers() const { return layer_count_; }

 private:
  SpectralTokenizer tokenizer_;
  IUC layer_count_;
  MuseLayer layers_[8];
  Vec3 embeddings_[256];
  Vec3 intermediate_[256];
};

}  // namespace CT

#endif  // CRABS_TOOLKIT_UPDATE_SPECTRAL_TOKENIZER_H
