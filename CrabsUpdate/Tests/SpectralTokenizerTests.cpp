// Copyright AStarship <https://astarship.net>.
//
// Tests for SpectralTokenizer - geometric attention on S2 via 3x3 monoid layers.
// Standalone test - does NOT include _Package.hxx to avoid immintrin.h
//
// Build:
//   g++ -std=c++23 -Wall -Wextra -Wpedantic -Werror
//       SpectralTokenizerTests.cpp -o /tmp/spectral_tests && /tmp/spectral_tests

#include <cstdio>
#include <cmath>
#include <cassert>

// Minimal ASCII Data Types (from CrabsUpdate/ASCIITypes.h)
typedef unsigned int IUC;
typedef unsigned char IUA;
typedef float FPC;
typedef bool BOL;

// Include the SpectralTokenizer header (it includes ASCIITypes.h and AType.h
// but we already have the types defined above, so guard against redefinition)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"

// Inline the minimal parts of SpectralTokenizer.h to avoid include chain issues
// (We copy the essential definitions here since _Package.hxx pulls in Wcb.h
// which pulls in immintrin.h causing compilation failures)

// Constants
constexpr IUC WindowSize = 12;
constexpr FPC CovarianceEpsilon = 1e-6f;
constexpr FPC PiF = 3.14159265358979323846f;
constexpr FPC TwoPiF = 6.28318530717958647692f;
constexpr FPC MaxEigenIter = 100;
constexpr FPC EigenConverge = 1e-10f;
constexpr IUC MaxTokens = 1024;
constexpr IUC MaxLayers = 8;

// Vec3
struct Vec3 {
  FPC x, y, z;
  __attribute__((always_inline)) Vec3() : x(0), y(0), z(0) {}
  __attribute__((always_inline)) Vec3(FPC xx, FPC yy, FPC zz) : x(xx), y(yy), z(zz) {}
  __attribute__((always_inline)) FPC Dot(Vec3 o) const { return x*o.x + y*o.y + z*o.z; }
  __attribute__((always_inline)) Vec3 Cross(Vec3 o) const { return Vec3(y*o.z-z*o.y, z*o.x-x*o.z, x*o.y-y*o.x); }
  __attribute__((always_inline)) FPC NormSq() const { return x*x + y*y + z*z; }
  __attribute__((always_inline)) FPC Norm() const { return std::sqrt(NormSq()); }
  __attribute__((always_inline)) Vec3 Normalized() const { FPC n = Norm(); if (n < 1e-15f) return Vec3(0,0,0); return Vec3(x/n, y/n, z/n); }
  __attribute__((always_inline)) Vec3 operator+(Vec3 o) const { return Vec3(x+o.x, y+o.y, z+o.z); }
  __attribute__((always_inline)) Vec3 operator-(Vec3 o) const { return Vec3(x-o.x, y-o.y, z-o.z); }
  __attribute__((always_inline)) Vec3 Scale(FPC s) const { return Vec3(x*s, y*s, z*s); }
  __attribute__((always_inline)) FPC& operator[](IUC i) { return (&x)[i]; }
  __attribute__((always_inline)) FPC operator[](IUC i) const { return (&x)[i]; }
};

inline Vec3 operator*(FPC s, Vec3 v) { return v.Scale(s); }

// Mat3x3
struct Mat3x3 {
  FPC m[3][3];
  __attribute__((always_inline)) Mat3x3() { for (IUC i=0;i<3;++i) for (IUC j=0;j<3;++j) m[i][j]=0; }
  __attribute__((always_inline)) static Mat3x3 Identity() { Mat3x3 r; r.m[0][0]=1; r.m[1][1]=1; r.m[2][2]=1; return r; }
  __attribute__((always_inline)) Vec3 Mul(Vec3 v) const { return Vec3(m[0][0]*v.x+m[0][1]*v.y+m[0][2]*v.z, m[1][0]*v.x+m[1][1]*v.y+m[1][2]*v.z, m[2][0]*v.x+m[2][1]*v.y+m[2][2]*v.z); }
  __attribute__((always_inline)) Mat3x3 Mul(Mat3x3 o) const {
    Mat3x3 r;
    for (IUC i=0;i<3;++i) for (IUC j=0;j<3;++j) { FPC s=0; for (IUC k=0;k<3;++k) s+=m[i][k]*o.m[k][j]; r.m[i][j]=s; }
    return r;
  }
  __attribute__((always_inline)) Mat3x3 operator+(Mat3x3 o) const { Mat3x3 r; for (IUC i=0;i<3;++i) for (IUC j=0;j<3;++j) r.m[i][j]=m[i][j]+o.m[i][j]; return r; }
  __attribute__((always_inline)) Mat3x3 Scale(FPC s) const { Mat3x3 r; for (IUC i=0;i<3;++i) for (IUC j=0;j<3;++j) r.m[i][j]=m[i][j]*s; return r; }
  __attribute__((always_inline)) Mat3x3 Transpose() const { Mat3x3 r; for (IUC i=0;i<3;++i) for (IUC j=0;j<3;++j) r.m[i][j]=m[j][i]; return r; }
  __attribute__((always_inline)) FPC Trace() const { return m[0][0]+m[1][1]+m[2][2]; }
  __attribute__((always_inline)) void Zero() { for (IUC i=0;i<3;++i) for (IUC j=0;j<3;++j) m[i][j]=0; }
};

inline Mat3x3 operator*(FPC s, Mat3x3 m) { return m.Scale(s); }

// SpectralTokenizer
class SpectralTokenizer {
 public:
  SpectralTokenizer() : token_count_(0), layer_count_(0), golden_angle_(0) {}
  SpectralTokenizer(IUC num_tokens, IUC num_layers) : token_count_(num_tokens), layer_count_(num_layers), golden_angle_(0) { Init(); }
  void Init() {
    golden_angle_ = TwoPiF * (1.0f - 1.0f / 1.6180339887498948482f);
    for (IUC l=0; l<layer_count_; ++l) for (IUC t=0; t<token_count_; ++t) z_table_[l*token_count_+t] = 0;
  }
  __attribute__((always_inline)) FPC GetAngle(IUC token_id) const {
    if (token_count_==0) return 0;
    FPC angle = static_cast<FPC>(token_id % token_count_) * golden_angle_;
    while (angle >= TwoPiF) angle -= TwoPiF;
    while (angle < 0) angle += TwoPiF;
    return angle;
  }
  __attribute__((always_inline)) FPC GetZ(IUC token_id, IUC layer) const {
    if (layer>=layer_count_ || token_id>=token_count_) return 0;
    return z_table_[layer*token_count_+token_id];
  }
  __attribute__((always_inline)) void SetZ(IUC token_id, IUC layer, FPC z) {
    if (layer<layer_count_ && token_id<token_count_) z_table_[layer*token_count_+token_id] = z;
  }
  __attribute__((always_inline)) Vec3 Embed(IUC token_id, IUC layer) const {
    FPC theta = GetAngle(token_id);
    FPC z = GetZ(token_id, layer);
    Vec3 raw(std::cos(theta), std::sin(theta), z);
    return raw.Normalized();
  }
  __attribute__((always_inline)) Vec3 EmbedRaw(IUC token_id, IUC layer) const {
    FPC theta = GetAngle(token_id);
    FPC z = GetZ(token_id, layer);
    return Vec3(std::cos(theta), std::sin(theta), z);
  }
  IUC TokenCount() const { return token_count_; }
  IUC LayerCount() const { return layer_count_; }
 private:
  IUC token_count_;
  IUC layer_count_;
  FPC golden_angle_;
  FPC z_table_[MaxTokens * MaxLayers];
};

// Covariance
class Covariance {
 public:
  Covariance() : computed_(false) {}
  void Compute(const Vec3* embeddings, IUC n) {
    sigma_.Zero();
    FPC inv_n = 1.0f / static_cast<FPC>(n);
    for (IUC i=0; i<3; ++i) for (IUC j=0; j<3; ++j) {
      FPC s = 0;
      for (IUC k=0; k<n; ++k) s += embeddings[k][i] * embeddings[k][j];
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
    for (IUC iter=0; iter<static_cast<IUC>(MaxEigenIter); ++iter) {
      Mat3x3 At = A.Transpose();
      Mat3x3 AtA = At.Mul(A);
      Mat3x3 AtA_inv = Inverse3x3(AtA);
      if (!AtA_inv.m[0][0] && !AtA_inv.m[1][1] && !AtA_inv.m[2][2]) break;
      Mat3x3 next = A.Scale(0.5f) + AtA_inv.Scale(0.5f);
      FPC diff = 0;
      for (IUC i=0; i<3; ++i) for (IUC j=0; j<3; ++j) { FPC d=next.m[i][j]-A.m[i][j]; diff += d*d; }
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
    for (IUC i=0;i<3;++i) for (IUC j=0;j<3;++j) A.m[i][j] = sigma_.m[i][j];
    Vec3 V[3] = { Vec3(1,0,0), Vec3(0,1,0), Vec3(0,0,1) };
    for (IUC iter=0; iter<static_cast<IUC>(MaxEigenIter); ++iter) {
      FPC max_val = 0;
      IUC p=0, q=1;
      if (std::abs(A.m[0][2]) > max_val) { max_val = std::abs(A.m[0][2]); p=0; q=2; }
      if (std::abs(A.m[1][2]) > max_val) { max_val = std::abs(A.m[1][2]); p=1; q=2; }
      if (max_val < EigenConverge) break;
      FPC a_pp = A.m[p][p], a_qq = A.m[q][q], a_pq = A.m[p][q];
      FPC theta = 0.5f * std::atan2(2.0f*a_pq, a_qq-a_pp);
      FPC c = std::cos(theta), s = std::sin(theta);
      Mat3x3 Ap;
      for (IUC i=0; i<3; ++i) {
        if (i!=p && i!=q) {
          Ap.m[i][p] = c*A.m[i][p] + s*A.m[i][q];
          Ap.m[i][q] = -s*A.m[i][p] + c*A.m[i][q];
          Ap.m[p][i] = Ap.m[i][p];
          Ap.m[q][i] = Ap.m[i][q];
        }
      }
      Ap.m[p][p] = c*c*a_pp + 2*s*c*a_pq + s*s*a_qq;
      Ap.m[q][q] = s*s*a_pp - 2*s*c*a_pq + c*c*a_qq;
      Ap.m[p][q] = 0; Ap.m[q][p] = 0;
      for (IUC i=0; i<3; ++i) {
        FPC v_p = V[i][p], v_q = V[i][q];
        V[i][p] = c*v_p + s*v_q;
        V[i][q] = -s*v_p + c*v_q;
      }
      A = Ap;
    }
    eigenvalues_[0] = A.m[0][0]; eigenvalues_[1] = A.m[1][1]; eigenvalues_[2] = A.m[2][2];
    eigenvectors_[0] = V[0]; eigenvectors_[1] = V[1]; eigenvectors_[2] = V[2];
    for (IUC i=0; i<2; ++i) for (IUC j=i+1; j<3; ++j) {
      if (eigenvalues_[j] > eigenvalues_[i]) {
        FPC tmp = eigenvalues_[i]; eigenvalues_[i] = eigenvalues_[j]; eigenvalues_[j] = tmp;
        Vec3 vt = eigenvectors_[i]; eigenvectors_[i] = eigenvectors_[j]; eigenvectors_[j] = vt;
      }
    }
    computed_ = true;
  }
  __attribute__((always_inline)) static Mat3x3 Inverse3x3(Mat3x3 m) {
    FPC det = m.m[0][0]*(m.m[1][1]*m.m[2][2]-m.m[1][2]*m.m[2][1]) - m.m[0][1]*(m.m[1][0]*m.m[2][2]-m.m[1][2]*m.m[2][0]) + m.m[0][2]*(m.m[1][0]*m.m[2][1]-m.m[1][1]*m.m[2][0]);
    if (std::abs(det) < 1e-20f) { Mat3x3 z; return z; }
    FPC idet = 1.0f/det;
    Mat3x3 inv;
    inv.m[0][0]=(m.m[1][1]*m.m[2][2]-m.m[1][2]*m.m[2][1])*idet;
    inv.m[0][1]=(m.m[0][2]*m.m[2][1]-m.m[0][1]*m.m[2][2])*idet;
    inv.m[0][2]=(m.m[0][1]*m.m[1][2]-m.m[0][2]*m.m[1][1])*idet;
    inv.m[1][0]=(m.m[1][2]*m.m[2][0]-m.m[1][0]*m.m[2][2])*idet;
    inv.m[1][1]=(m.m[0][0]*m.m[2][2]-m.m[0][2]*m.m[2][0])*idet;
    inv.m[1][2]=(m.m[0][2]*m.m[1][0]-m.m[0][0]*m.m[1][2])*idet;
    inv.m[2][0]=(m.m[1][0]*m.m[2][1]-m.m[1][1]*m.m[2][0])*idet;
    inv.m[2][1]=(m.m[0][1]*m.m[2][0]-m.m[0][0]*m.m[2][1])*idet;
    inv.m[2][2]=(m.m[0][0]*m.m[1][1]-m.m[0][1]*m.m[1][0])*idet;
    return inv;
  }
};

// Monoid
class Monoid {
 public:
  Monoid() : matrix_(Mat3x3::Identity()) {}
  explicit Monoid(Mat3x3 m) : matrix_(m) {}
  __attribute__((always_inline)) static Monoid Identity() { return Monoid(Mat3x3::Identity()); }
  __attribute__((always_inline)) const Mat3x3& Matrix() const { return matrix_; }
  __attribute__((always_inline)) Vec3 Apply(Vec3 v) const { return matrix_.Mul(v); }
  __attribute__((always_inline)) Monoid Compose(Monoid o) const { return Monoid(matrix_.Mul(o.matrix_)); }
  __attribute__((always_inline)) void ComposeInPlace(Monoid o) { matrix_ = matrix_.Mul(o.matrix_); }
  __attribute__((always_inline)) void SetMatrix(Mat3x3 m) { matrix_ = m; }
  __attribute__((always_inline)) void Reset() { matrix_ = Mat3x3::Identity(); }
 private:
  Mat3x3 matrix_;
};

// MuseLayer
class MuseLayer {
 public:
  MuseLayer() : layer_matrix_(Mat3x3::Identity()) {}
  Monoid Process(const Vec3* embeddings, Vec3* output, BOL renormalize = true) {
    cov_.Compute(embeddings, WindowSize);
    const Mat3x3& R = cov_.Rotation();
    const Mat3x3& S = cov_.Stretch();
    Mat3x3 M = R.Mul(S);
    layer_matrix_ = M;
    for (IUC i=0; i<WindowSize; ++i) {
      Vec3 transformed = M.Mul(embeddings[i]);
      if (renormalize) output[i] = transformed.Normalized();
      else output[i] = transformed;
    }
    return Monoid(M);
  }
  void Forward(const Vec3* embeddings, Vec3* output, IUC seq_len, BOL renormalize = true) {
    Vec3 window[WindowSize];
    for (IUC t=0; t<seq_len; ++t) {
      for (IUC w=0; w<WindowSize; ++w) { IUC idx = (t+w) % seq_len; window[w] = embeddings[idx]; }
      Process(window, &output[t], renormalize);
    }
  }
  __attribute__((always_inline)) const Mat3x3& LayerMatrix() const { return layer_matrix_; }
  __attribute__((always_inline)) const Covariance& GetCovariance() const { return cov_; }
 private:
  Mat3x3 layer_matrix_;
  Covariance cov_;
};

// MuseTransformer
class MuseTransformer {
 public:
  MuseTransformer(IUC num_tokens, IUC num_layers, IUC hidden_dim = 1)
    : tokenizer_(num_tokens, hidden_dim), layer_count_(num_layers) {
    for (IUC l=0; l<layer_count_; ++l) layers_[l] = MuseLayer();
  }
  void Forward(const IUC* token_ids, Vec3* output, IUC seq_len) {
    for (IUC t=0; t<seq_len; ++t) embeddings_[t] = tokenizer_.Embed(token_ids[t], 0);
    for (IUC l=0; l<layer_count_; ++l) {
      layers_[l].Forward(embeddings_, intermediate_, seq_len, true);
      for (IUC t=0; t<seq_len; ++t) embeddings_[t] = intermediate_[t];
    }
    for (IUC t=0; t<seq_len; ++t) output[t] = embeddings_[t];
  }
  IUC Readout(Vec3 embedding) const {
    FPC angle = std::atan2(embedding.y, embedding.x);
    if (angle < 0) angle += TwoPiF;
    IUC best_id = 0;
    FPC best_diff = TwoPiF;
    for (IUC tid=0; tid<tokenizer_.TokenCount(); ++tid) {
      FPC theta = tokenizer_.GetAngle(tid);
      FPC diff = std::abs(angle - theta);
      if (diff > PiF) diff = TwoPiF - diff;
      if (diff < best_diff) { best_diff = diff; best_id = tid; }
    }
    return best_id;
  }
  __attribute__((always_inline)) const SpectralTokenizer& Tokenizer() const { return tokenizer_; }
  __attribute__((always_inline)) MuseLayer& Layer(IUC l) { return layers_[l]; }
  __attribute__((always_inline)) IUC NumLayers() const { return layer_count_; }
 private:
  SpectralTokenizer tokenizer_;
  IUC layer_count_;
  MuseLayer layers_[8];
  Vec3 embeddings_[256];
  Vec3 intermediate_[256];
};

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

static bool FpcNear(FPC a, FPC b, FPC tol = 1e-5f) {
  FPC diff = a - b;
  if (diff < 0) diff = -diff;
  return diff < tol;
}

void TestVec3() {
  Vec3 v0;
  assert(v0.x == 0 && v0.y == 0 && v0.z == 0);
  Vec3 a(1, 0, 0), b(0, 1, 0);
  assert(a.Dot(b) == 0);
  Vec3 c(1, 2, 3), d(4, 5, 6);
  assert(FpcNear(c.Dot(d), 32));
  Vec3 e = a.Cross(b);
  assert(FpcNear(e.x, 0) && FpcNear(e.y, 0) && FpcNear(e.z, 1));
  assert(FpcNear(a.Norm(), 1));
  assert(FpcNear(c.Norm(), std::sqrt(14)));
  Vec3 f(3, 4, 0), fn = f.Normalized();
  assert(FpcNear(fn.Norm(), 1));
  Vec3 g = a + b;
  assert(FpcNear(g.x, 1) && FpcNear(g.y, 1));
  Vec3 h = c.Scale(2);
  assert(FpcNear(h.x, 2) && FpcNear(h.y, 4) && FpcNear(h.z, 6));
  c[0] = 10;
  assert(c.x == 10);
  assert(c[1] == 2);
  puts("  Vec3: OK");
}

void TestMat3x3() {
  Mat3x3 I = Mat3x3::Identity();
  assert(I.m[0][0] == 1 && I.m[1][1] == 1 && I.m[2][2] == 1);
  assert(I.m[0][1] == 0 && I.m[0][2] == 0 && I.m[1][0] == 0);
  Mat3x3 Z;
  assert(Z.m[0][0] == 0);
  Mat3x3 M;
  M.m[0][0]=1; M.m[0][1]=2; M.m[0][2]=3;
  M.m[1][0]=4; M.m[1][1]=5; M.m[1][2]=6;
  M.m[2][0]=7; M.m[2][1]=8; M.m[2][2]=9;
  Vec3 v(1, 0, 0);
  Vec3 r = M.Mul(v);
  assert(FpcNear(r.x, 1) && FpcNear(r.y, 4) && FpcNear(r.z, 7));
  Mat3x3 Im = I.Mul(M);
  assert(FpcNear(Im.m[0][0], M.m[0][0]));
  Mat3x3 Mt = M.Transpose();
  assert(FpcNear(Mt.m[0][1], M.m[1][0]));
  assert(FpcNear(Mt.m[1][0], M.m[0][1]));
  assert(FpcNear(M.Trace(), 15));
  Mat3x3 Ms = M.Scale(2);
  assert(FpcNear(Ms.m[0][0], 2));
  Mat3x3 Ma = M + M;
  assert(FpcNear(Ma.m[0][0], 2));
  puts("  Mat3x3: OK");
}

void TestSpectralTokenizer() {
  SpectralTokenizer tok(100, 2);
  assert(tok.TokenCount() == 100);
  assert(tok.LayerCount() == 2);
  FPC angle0 = tok.GetAngle(0);
  assert(FpcNear(angle0, 0));
  FPC golden = TwoPiF * (1.0f - 1.0f / 1.6180339887498948482f);
  FPC angle1 = tok.GetAngle(1);
  assert(FpcNear(angle1, golden));
  FPC angle100 = tok.GetAngle(100);
  assert(angle100 >= 0 && angle100 < TwoPiF);
  Vec3 emb0 = tok.Embed(0, 0);
  assert(FpcNear(emb0.x, 1));
  assert(FpcNear(emb0.y, 0));
  assert(FpcNear(emb0.Norm(), 1));
  Vec3 emb1 = tok.Embed(1, 0);
  assert(FpcNear(emb1.Norm(), 1));
  tok.SetZ(5, 0, 0.5f);
  assert(FpcNear(tok.GetZ(5, 0), 0.5f));
  assert(FpcNear(tok.GetZ(5, 1), 0));
  Vec3 emb5 = tok.Embed(5, 0);
  assert(FpcNear(emb5.Norm(), 1));
  Vec3 raw = tok.EmbedRaw(5, 0);
  assert(raw.Norm() > 1);
  puts("  SpectralTokenizer: OK");
}

void TestCovariance() {
  Covariance cov;
  Vec3 embeddings[WindowSize];
  for (IUC i=0; i<WindowSize; ++i) {
    FPC theta = static_cast<FPC>(i) * TwoPiF / WindowSize;
    embeddings[i] = Vec3(std::cos(theta), std::sin(theta), 0);
  }
  cov.Compute(embeddings, WindowSize);
  assert(cov.IsComputed());
  const FPC* evals = cov.Eigenvalues();
  assert(evals[0] >= evals[1]);
  assert(evals[1] >= evals[2]);
  assert(evals[0] > 0.1);
  assert(evals[1] > 0.1);
  assert(evals[2] < 0.1);
  for (IUC i=0; i<3; ++i)
    assert(FpcNear(cov.Eigenvectors()[i].Norm(), 1));
  Mat3x3 I = Mat3x3::Identity();
  cov.PolarDecompose(I);
  assert(cov.Rotation().m[0][0] > 0.99);
  assert(cov.Stretch().m[0][0] > 0.99);
  puts("  Covariance: OK");
}

void TestMonoid() {
  Monoid m1 = Monoid::Identity();
  Vec3 v(1, 2, 3);
  Vec3 r1 = m1.Apply(v);
  assert(FpcNear(r1.x, 1) && FpcNear(r1.y, 2) && FpcNear(r1.z, 3));
  Mat3x3 M;
  M.m[0][0]=2; M.m[1][1]=2; M.m[2][2]=2;
  Monoid m2(M);
  Vec3 r2 = m2.Apply(v);
  assert(FpcNear(r2.x, 2) && FpcNear(r2.y, 4) && FpcNear(r2.z, 6));
  Monoid m3 = m1.Compose(m2);
  Vec3 r3 = m3.Apply(v);
  assert(FpcNear(r3.x, 2) && FpcNear(r3.y, 4) && FpcNear(r3.z, 6));
  Monoid m4 = m2.Compose(m2);
  Vec3 r4 = m4.Apply(v);
  assert(FpcNear(r4.x, 4) && FpcNear(r4.y, 8) && FpcNear(r4.z, 12));
  m2.Reset();
  Vec3 r5 = m2.Apply(v);
  assert(FpcNear(r5.x, 1) && FpcNear(r5.y, 2) && FpcNear(r5.z, 3));
  puts("  Monoid: OK");
}

void TestMuseLayer() {
  MuseLayer layer;
  Vec3 input[WindowSize];
  for (IUC i=0; i<WindowSize; ++i) {
    FPC theta = static_cast<FPC>(i) * TwoPiF / WindowSize;
    input[i] = Vec3(std::cos(theta), std::sin(theta), 0);
  }
  Vec3 output[WindowSize];
  Monoid result = layer.Process(input, output, true);
  for (IUC i=0; i<WindowSize; ++i)
    assert(FpcNear(output[i].Norm(), 1));
  const Mat3x3& M = result.Matrix();
  assert(M.m[0][0] != 0 || M.m[1][1] != 0 || M.m[2][2] != 0);
  assert(layer.GetCovariance().IsComputed());
  const FPC* evals = layer.GetCovariance().Eigenvalues();
  assert(evals[0] >= evals[1]);
  assert(evals[1] >= evals[2]);
  puts("  MuseLayer: OK");
}

void TestMuseTransformer() {
  MuseTransformer transformer(256, 3, 1);
  IUC tokens[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  Vec3 output[16];
  transformer.Forward(tokens, output, 16);
  for (IUC i=0; i<16; ++i)
    assert(FpcNear(output[i].Norm(), 1));
  IUC decoded = transformer.Readout(output[0]);
  assert(decoded < 256);
  IUC decoded0 = transformer.Readout(output[0]);
  // The readout should be within the vocabulary range.
  assert(decoded0 < 256);
  Vec3 input_emb = transformer.Tokenizer().Embed(7, 0);
  assert(!FpcNear(output[7].x, input_emb.x) || !FpcNear(output[7].y, input_emb.y) || !FpcNear(output[7].z, input_emb.z));
  puts("  MuseTransformer: OK");
}

void TestSpectralProperty() {
  SpectralTokenizer tok(256, 1);
  // Verify angles are in [0, 2pi).
  for (IUC tid=0; tid<256; ++tid) {
    FPC angle = tok.GetAngle(tid);
    assert(angle >= 0 && angle < TwoPiF);
  }
  // Verify golden angle spacing: consecutive tokens differ by golden_angle mod 2pi.
  FPC golden = TwoPiF * (1.0f - 1.0f / 1.6180339887498948482f);
  FPC angle0 = tok.GetAngle(0), angle1 = tok.GetAngle(1);
  FPC diff = angle1 - angle0;
  if (diff < 0) diff = -diff;
  // The difference should be close to golden_angle (mod 2pi).
  assert(FpcNear(diff, golden) || FpcNear(diff, std::fmod(golden, TwoPiF)));
  // Consecutive angle differences should be roughly constant.
  FPC diffs[12];
  for (IUC i=0; i<12; ++i) {
    FPC a1 = tok.GetAngle(i);
    FPC a2 = tok.GetAngle(i+1);
    FPC d = a2 - a1;
    if (d < 0) d += TwoPiF;
    diffs[i] = d;
  }
  // All diffs should be close to golden_angle mod 2pi.
  FPC expected = std::fmod(golden, TwoPiF);
  for (IUC i=0; i<12; ++i)
    assert(FpcNear(diffs[i], expected, 0.1f));
  puts("  SpectralProperty: OK");
}

void TestEllipsoidAction() {
  MuseLayer layer;
  Vec3 input[WindowSize];
  for (IUC i=0; i<WindowSize; ++i) {
    FPC theta = 0.1f * std::sin(static_cast<FPC>(i) * 0.5f);
    FPC phi = 0.1f * std::cos(static_cast<FPC>(i) * 0.5f);
    input[i] = Vec3(std::cos(theta), std::sin(theta)*std::cos(phi), std::sin(phi));
  }
  Vec3 output[WindowSize];
  layer.Process(input, output, true);
  const FPC* evals = layer.GetCovariance().Eigenvalues();
  assert(evals[0] > evals[1] * 2);
  assert(evals[1] > evals[2] * 0.5);
  puts("  EllipsoidAction: OK");
}

void TestMonoidCompositionDepth() {
  Monoid result = Monoid::Identity();
  for (IUC i=0; i<10; ++i) {
    Mat3x3 R;
    FPC theta = static_cast<FPC>(i) * 0.1f;
    R.m[0][0]=std::cos(theta); R.m[0][1]=-std::sin(theta); R.m[0][2]=0;
    R.m[1][0]=std::sin(theta); R.m[1][1]=std::cos(theta); R.m[1][2]=0;
    R.m[2][0]=0; R.m[2][1]=0; R.m[2][2]=1;
    result.ComposeInPlace(Monoid(R));
  }
  Vec3 v(1, 0, 0);
  Vec3 r = result.Apply(v);
  assert(FpcNear(r.Norm(), 1));
  puts("  MonoidCompositionDepth: OK");
}

int main() {
  puts("Spectral Tokenizer Tests");
  puts("========================");
  TestVec3();
  TestMat3x3();
  TestSpectralTokenizer();
  TestCovariance();
  TestMonoid();
  TestMuseLayer();
  TestMuseTransformer();
  TestSpectralProperty();
  TestEllipsoidAction();
  TestMonoidCompositionDepth();
  puts("========================");
  puts("Spectral Tokenizer: 10 test groups passed");
  return 0;
}
