// Copyright AStarship <https://astarship.net>.
// IPC — bounded, versioned, process-shared SPSC ring pairs over POSIX shm.
//
// 2026-10-20 rewrite of the 2026-10-16 prototype. The prototype faults
// (documented in AGENT_PLAN.md P0, reproduced in _Seams/02.LegacyRed.cpp):
//   - Master()/Slave() returned the same object: no full duplex.
//   - Two ISW counters (16 bytes on a pointer-sized host) with data hardcoded
//     at offset 8: the payload overlapped the read counter.
//   - Offsets advanced without modulo: the ring died after one full fill and
//     BytesAvailable() went negative.
//   - The attacher zeroed live peer counters; no create/open distinction.
//   - No process-shared memory ordering; no documented synchronization.
//
// Design (this file):
//   A PipePair owns ONE named POSIX shared-memory object containing TWO
//   independent SPSC byte rings — request (master->slave) and reply
//   (slave->master). Each direction is single-producer/single-consumer, so
//   a producer and a consumer are never two writers. The header is a
//   versioned, explicitly laid-out struct (PipeHeader) whose size the writer
//   derives from the declaration: no hard-coded offset.
//
// Synchronization contract (documented, tested, NOT silent std::atomic):
//   - All shared counters are IUC (32-bit) MONOTONIC byte counters, one per
//     direction per role: head = bytes the producer has published,
//     tail = bytes the consumer has consumed. A ring holds head-tail bytes.
//   - Publication is a single 32-bit release store of head (or tail) AFTER
//     the data bytes. On x86-64/TSO a plain store is release and loads are
//     acquire, so the byte payload is ordered before the counter. On
//     weakly-ordered platforms (ARM64) a plain store is NOT release; the
//     futex_wait/futex_wake path (Poll/WaitForData/WaitForSpace) provides
//     the acquire/release fence pair and the bounded blocking wait.
//   - futex(2) is the chosen platform primitive: Linux kernel futex,
//   4-byte aligned word, no C++ stdlib dependency. Wait only blocks when
//   the expected value has already changed (the futex_wait fast path), so a
//   peer that is already publishing never costs a syscall.
//   - The creator publishes a ready flag with the same release semantics;
//   an attacher waits on it (bounded) and never zeros or rewrites the
//   header of a live mapping.
//
// Ownership and teardown:
// Direction ownership: the creator endpoint is the REQUEST-PRODUCER and
// REPLY-CONSUMER (the "master" role); the attaching endpoint is the
// REPLY-PRODUCER and REQUEST-CONSUMER (the "slave" role). Pipe::Write
// always publishes into the role's write direction; Pipe::Read always
// consumes from the role's read direction. The two endpoints therefore
// never consume their own messages, and a full-duplex exchange is one
// request ring plus one reply ring.
//
// Ownership and teardown:
//   - Pipe::Create(name, ...) creates and names the mapping; the creator
//     owns the name. On the final Close of the in-process reference count
//     the name is unlinked; an attacher never unlinks a peer's resource.
//   - Pipe::Attach(name) opens an existing mapping; it validates
//     magic/version/header_bytes/ring sizes before use and does not
//     truncate, zero or rewrite a live peer's header.
//   - The class is move-only. A moved-from or closed endpoint rejects all
//     operations with PipeClosed.
//
// Framing: bounded BYTE streams, NOT length-prefixed messages. Read reads
// at most max_bytes and writes NO extra NUL. Message framing (operation
// ID, version, payload length) is the P1 Slot adapter's job on top of this
// transport; this layer only guarantees byte order, wraparound safety and
// the SPSC ownership above.
//
// Partial transfers: a write larger than the current free space publishes
// the whole free space and returns the short count — an explicit,
// recoverable result, distinct from an error; the caller loops with the
// remainder. Read with max_bytes > available returns the available bytes
// (>= 0), never a negative short-read. would-block (block_ms == 0,
// nothing to do) is PipeEmpty/PipeFull; timeout is PipeTimeout; peer
// closure is PipePeerGone; a closed local endpoint is PipeClosed.
//
// Hot-path rules (inherited from the iGeek handoff): Read/Write/Poll do no
// allocation, no file I/O beyond the futex fast path, and no blocking
// beyond the caller's requested timeout.

#pragma once
#ifndef CRABSTK_IPC_PIPE_H
#define CRABSTK_IPC_PIPE_H

#include "_Config.h"

namespace _ {

// Status codes; PipeOK is zero. Negative values are failures.
enum PipeStatus : ISC {
  PipeOK = 0,
  PipeClosed = -1,       ///< endpoint already closed or moved-from
  PipeInvalid = -2,      ///< bad argument (null buffer, non-positive size)
  PipeFull = -3,         ///< would-block: no space for the whole message
  PipeEmpty = -4,        ///< no data available (non-blocking read)
  PipeTimeout = -5,      ///< bounded wait expired with no progress
  PipePeerGone = -6,     ///< peer closed / mapping vanished
  PipeBadVersion = -7,   ///< peer mapping has a different header version
  PipeBadSize = -8,      ///< peer mapping size does not match the schema
  PipeCreate = -9,       ///< shm_open/ftruncate/mmap failure
  PipePartial = -10      ///< a partial transfer completed (see note below)
};

// Fixed, explicitly declared layout of the shared header.
// Derive PipeHeaderBytes from this struct — never hardcode the offset.
struct PipeHeader {
  IUC magic;        ///< PipeMagic, endianness-anchored sentinel
  IUC version;      ///< PipeVersion
  IUC header_bytes; ///< sizeof(PipeHeader) as published by the creator
  IUC request_ring; ///< usable request-direction bytes (head - tail max)
  IUC reply_ring;   ///< usable reply-direction bytes
  IUC head_req;     ///< bytes published into the request ring (monotonic)
  IUC tail_req;     ///< bytes consumed from the request ring (monotonic)
  IUC head_rep;     ///< bytes published into the reply ring (monotonic)
  IUC tail_rep;     ///< bytes consumed from the reply ring (monotonic)
  IUC ready;        ///< 1 once the creator has published a valid mapping
  IUC creator;      ///< creator's process id (forensic, not security)
  IUC reserved0;    ///< zero, reserved for header growth
};

constexpr IUC PipeMagic = 0x43524250;   ///< "CRBP" little-endian anchor
constexpr IUC PipeVersion = 1;
constexpr IUC PipeHeaderBytes = sizeof(PipeHeader);
static_assert(PipeHeaderBytes >= 40, "PipeHeader must carry all counters");
static_assert((PipeHeaderBytes & 7) == 0, "PipeHeader must be word-aligned");
static_assert(PipeHeaderBytes < 4096, "PipeHeader must fit one page");

// A bounded SPSC byte ring over one direction of a PipePair mapping.
class Pipe {
public:
  Pipe();
  ~Pipe();
  Pipe(const Pipe&) = delete;
  Pipe& operator=(const Pipe&) = delete;
  Pipe(Pipe&& other) noexcept;
  Pipe& operator=(Pipe&& other) noexcept;

  // Create the named mapping (owner of the name) and attach to it.
  // ring_bytes is the usable capacity of EACH direction, power-of-two
  // reduced to the next lower power of two (maskable, exact wrap).
  // Returns PipeOK or PipeCreate; on failure the endpoint is closed and
  // any name this call created is unlinked.
  static Pipe Create(const CHA* name, IUC ring_bytes);


  // Attach to an existing named mapping without zeroing or resizing it.
  // Validates magic/version/header_bytes/ring sizes; a mismatched mapping
  // is unmapped and the endpoint is left closed with the exact status.
  // ready-wait: up to timeout_ms for the creator's ready publication.
  static Pipe Attach(const CHA* name, ISC timeout_ms);

  // Write n_bytes into this endpoint's write direction. If the ring cannot
  // hold all n_bytes at this moment — including the case where n_bytes
  // exceeds the whole ring — the whole free space is published now and the
  // short count returned (the documented partial transfer; the caller
  // loops with the remainder). A completely full ring returns PipeFull
  // with nothing written. Returns bytes written (>=0) or a negative
  // PipeStatus (PipeInvalid only for a null buffer or non-positive size).
  ISW Write(const CHA* data, ISW n_bytes);

  // Read up to max_bytes into buf. Writes NO extra NUL.
  // Returns bytes read (>=0) or a negative PipeStatus (PipeEmpty when
  // there is nothing and block_ms == 0).
  ISW Read(CHA* buf, ISW max_bytes, ISC block_ms = 0);

  // Bytes currently available on this endpoint's read direction.
  ISW BytesAvailable() const;
  // Bytes of free space on this endpoint's write direction.
  ISW SpaceAvailable() const;

  // Bounded blocking wait for data (read direction) / space (write
  // direction) using the futex backend. block_ms < 0 waits unbounded.
  // Returns PipeOK on progress, PipeTimeout, or PipeClosed/PipePeerGone.
  PipeStatus WaitForData(ISC block_ms);
  PipeStatus WaitForSpace(ISC block_ms);

  // Non-blocking poll: true iff BytesAvailable() > 0.
  BOL HasData() const;

  // Close this endpoint. Unmaps the mapping; if this endpoint owns the
  // name AND no other endpoint has it mapped in this process, unlinks the
  // name. After Close, all operations return PipeClosed.
  void Close();

  // The shared name ("" if never created/attached).
  const CHA* Name() const { return name_; }
  operator bool() const { return valid_; }

public:
  // Next lower power of two (min 64): the ring size reduction applied by
  // Create. Public so seam tests can verify the contract without a mapping.
  static IUC RoundRing(IUC ring_bytes);

private:
  PipeStatus Bind(CHA* map, IUC map_bytes, BOL owns_name,
                  const CHA* name, IUC ring_bytes, ISC timeout_ms);
  void Unmap(BOL unlink_name);
  PipeStatus WaitForCounter(IUC* counter, IUC expected, ISC block_ms);

  CHA* map_;          ///< mapped bytes (header + both rings), or null
  IUC map_bytes_;     ///< full mapping size in bytes
  IUC ring_bytes_;    ///< usable bytes per ring direction
  IUC ring_mask_;     ///< ring_bytes_ - 1 (power of two)
  ISC name_fd_;       ///< -1: no name ownership tracking
  CHA name_[256];     ///< shm name (owned if creator)
  BOL owns_name_;
  BOL valid_;
};

// Compatibility wrappers over the historical single-object API.
// HONEST SEMANTICS: both calls return references to the same in-process
// Pipe endpoint (one object, both directions visible). They exist so the
// legacy happy-path test compiles and runs; they are NOT proof of duplex —
// a duplex assertion must use Create/Attach or two distinct endpoints.
Pipe& PipeMaster();
Pipe& PipeSlave();

}  //< namespace _

#endif // CRABSTK_IPC_PIPE_H
