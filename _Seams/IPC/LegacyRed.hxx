// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_RELEASE
#if SEAM == CRABS_RELEASE
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {

inline const CHA* Release(const CHA* args) {
#if SEAM == CRABS_RELEASE
  // Release seam test
  // TODO: insert test logic here
  return NILP;
#endif
  return NILP;
}

}  //< namespace CRTest

// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_RELEASE
#if SEAM == CRABS_RELEASE
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {

inline const CHA* Release(const CHA* args) {
#if SEAM == CRABS_RELEASE
  // Release seam test
  // TODO: insert test logic here
  return NILP;
#endif
  return NILP;
}

}  //< namespace CRTest

// Copyright AStarship <https://astarship.net>.
// Legacy Pipe RED reproduction — self-contained, frozen.
//
// This file compiles the ORIGINAL 2026-10-16 Pipe prototype (singleton
// Master/Slave, 8-byte hardcoded header, no wrap, no ready handshake)
// against the historical pipe_test.hxx expectations. It exists to
// document the legacy fault and is NOT part of the new contract.
//
// Build (from CrabsTK root):
//   g++ -x c++ -std=c++23 -O1 -g -Wno-deprecated-enum-enum-conversion \
//     -IIPC -I.. -I. -DSEAM=CRABSTK_RELEASE -DCRABSTK_IPC_LEGACY \
//     IPC/_Seams/LegacyRed.hxx -o "$TMPDIR/CrabsTKIPC/OldPipeRed"
//   (-x c++ is required: GCC 14 treats a bare .hxx source as a header and
//    saves a precompiled header instead of an executable.)

#include <cstdint>
#include <cstddef>
// Self-contained legacy type spellings (the old prototype's shadow typedefs).
typedef char CHA;
typedef int ISN;
typedef int ISC;
typedef intptr_t ISW;
typedef bool BOL;
typedef uint32_t IUC;
typedef uint8_t IUA;
typedef int64_t IUD;

#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>

// ---- The ORIGINAL Pipe prototype (2026-10-16), verbatim logic ----
// (global namespace: the legacy test references Pipe unqualified)

class Pipe {
public:
  static Pipe& Master();
  static Pipe& Slave();
  ~Pipe();
  Pipe(const Pipe&) = delete;
  Pipe& operator=(const Pipe&) = delete;
  ISN Read(CHA* buf, ISN max_bytes);
  ISN Write(const CHA* buf, ISN n_bytes);
  BOL HasData() const;
  ISN BytesAvailable() const;
  void Reset();
  operator bool() const { return valid_; }
private:
  Pipe();
  CHA* shared_memory_;
  ISW* write_offset_;
  ISW* read_offset_;
  ISW capacity_;
  BOL valid_;
  ISN read_pos_;
  ISN write_pos_;
};

static Pipe* g_pipe_instance = nullptr;

Pipe::Pipe() : shared_memory_(nullptr), write_offset_(nullptr),
               read_offset_(nullptr), capacity_(4 * 1024), valid_(false),
               read_pos_(0), write_pos_(0) {
  const char* shm_name = "/crabs_pipe_test";
  int fd = shm_open(shm_name, O_RDWR, 0);
  if (fd < 0) {
    fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) return;
    if (ftruncate(fd, capacity_) < 0) { close(fd); shm_unlink(shm_name); return; }
  }
  shared_memory_ = static_cast<CHA*>(mmap(nullptr, capacity_,
      PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
  if (shared_memory_ == MAP_FAILED) { close(fd); shm_unlink(shm_name); return; }
  write_offset_ = reinterpret_cast<ISW*>(shared_memory_);
  read_offset_ = write_offset_ + 1;
  *write_offset_ = 0;
  *read_offset_ = 0;
  memset(shared_memory_ + 8, 0, capacity_ - 8);
  valid_ = true;
  (void)close(fd);
}

Pipe::~Pipe() {
  if (shared_memory_) { munmap(shared_memory_, capacity_); shared_memory_ = nullptr; }
  valid_ = false;
}

Pipe& Pipe::Master() {
  if (!g_pipe_instance) g_pipe_instance = new Pipe();
  return *g_pipe_instance;
}
Pipe& Pipe::Slave() { return Master(); }

ISN Pipe::Read(CHA* buf, ISN max_bytes) {
  if (!valid_ || !buf || max_bytes <= 0) return -1;
  ISN data_available = *write_offset_ - *read_offset_;
  if (data_available <= 0) return 0;
  if (data_available > max_bytes) data_available = max_bytes;
  CHA* data_start = shared_memory_ + 8;
  if (*read_offset_ + data_available <= capacity_)
    memcpy(buf, data_start + *read_offset_, data_available);
  else {
    ISN first_piece = capacity_ - *read_offset_;
    memcpy(buf, data_start + *read_offset_, first_piece);
    memcpy(buf + first_piece, data_start, data_available - first_piece);
  }
  *read_offset_ += data_available;
  read_pos_ = *read_offset_;
  return data_available;
}

ISN Pipe::Write(const CHA* buf, ISN n_bytes) {
  if (!valid_ || !buf || n_bytes <= 0) return -1;
  ISN total_capacity = capacity_ - 8;
  ISN used_space = *write_offset_ - *read_offset_;
  ISN free_space = total_capacity - used_space;
  if (free_space <= 0) return -1;
  if (n_bytes > free_space) n_bytes = free_space;
  CHA* data_start = shared_memory_ + 8;
  if (*write_offset_ + n_bytes <= capacity_)
    memcpy(data_start + *write_offset_, buf, n_bytes);
  else {
    ISN first_piece = capacity_ - *write_offset_;
    memcpy(data_start + *write_offset_, buf, first_piece);
    memcpy(data_start, buf + first_piece, n_bytes - first_piece);
  }
  *write_offset_ += n_bytes;
  write_pos_ = *write_offset_;
  return n_bytes;
}

BOL Pipe::HasData() const {
  if (!valid_) return false;
  return *write_offset_ > *read_offset_;
}
ISN Pipe::BytesAvailable() const {
  if (!valid_) return 0;
  return *write_offset_ - *read_offset_;
}
void Pipe::Reset() {
  if (valid_) {
    *read_offset_ = 0; *write_offset_ = 0;
    memset(shared_memory_ + 8, 0, capacity_ - 8);
    read_pos_ = 0; write_pos_ = 0;
  }
}

// ---- The RED reproduction (the historical pipe_test.hxx logic) ----

int main() {
  printf("Starting in-process pipe test for inter-process RPC protocol...\n");
  int failures = 0;

  Pipe& pipe = Pipe::Master();
  if (!pipe) {
    printf("FAIL: Could not create pipe pair\n");
    return 1;
  }

  const char test_payload[] = "Hello, Crabs RPC Pipeline!";
  ISN payload_size = ISN(strlen(test_payload));
  printf("Payload: '%s' (%d bytes)\n", test_payload, payload_size);

  // RED-A: ISW is 8 bytes; two ISW counters need 16 but data starts at 8.
  if (sizeof(ISW) == 8) {
    printf("RED-A: ISW is %d bytes; two ISW counters need 16 but data starts at 8\n",
           (int)sizeof(ISW));
    ++failures;
  }

  // RED-B: no wrap — offsets grow unbounded; ring dies after one full fill.
  ISN data_cap = 4 * 1024 - 8;
  CHA big[4096];
  for (ISN i = 0; i < data_cap; ++i) big[i] = (CHA)('a' + i % 26);
  ISN w = pipe.Write(big, data_cap);
  if (w <= 0) { printf("RED-B: first write failed w=%d\n", w); ++failures; }
  else {
    CHA out[4096];
    ISN r = pipe.Read(out, sizeof(out));
    if (r != w) { printf("RED-B: read %d != wrote %d\n", r, w); ++failures; }
    ISN w2 = pipe.Write(big, 16);
    ISN r2 = (w2 > 0) ? pipe.Read(out, sizeof(out)) : -1;
    if (w2 <= 0 || r2 != w2) {
      printf("RED-B: second round w=%d r=%d — ring dead after one full fill (no wrap)\n",
             w2, r2);
      ++failures;
    }
  }
  ISN avail = pipe.BytesAvailable();
  if (avail != 0) {
    printf("RED-B: BytesAvailable()=%d after drain (unsigned offsets unwrapped)\n", avail);
    ++failures;
  }

  // RED-C: Master() and Slave() return the same object.
  if (&Pipe::Master() == &Pipe::Slave()) {
    printf("RED-C: Master() and Slave() return the same object\n");
    ++failures;
  }

  printf("OLD_PIPE_RED_FAILURES=%d\n", failures);
  return failures ? 1 : 0;
}
