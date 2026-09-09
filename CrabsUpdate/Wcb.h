// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TOOLKIT_UPDATE_WCB_H
#define CRABS_TOOLKIT_UPDATE_WCB_H

#include "ASCIITypes.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

#if defined(__x86_64__) || defined(__i386__)
#include <immintrin.h>
#endif

namespace KT {

/*
 * Write-Combining Buffer Cache (WCB)
 *
 * A fixed-size write-combining buffer that lets the CPU read memory without
 * waiting for writes to complete and without polluting the CPU cache.
 *
 * Problem:
 *   Every write to memory contaminates the CPU cache with data the CPU
 *   doesn't need, and the CPU stalls on each write until the memory
 *   controller ACKs it.
 *
 * Solution:
 *   Writes land in the WCB instead of memory. The CPU continues executing
 *   without stalling. Reads go directly to memory (bypassing the WCB), so
 *   the WCB never pollutes the cache. When the CPU is ready, it flushes
 *   all buffered writes in one batch.
 *
 * Coalescing:
 *   If two writes target the same address, the second overwrites the first
 *   in the buffer — only one flush-write reaches memory.
 *
 * Thread safety:
 *   Not thread-safe. The WCB is a per-thread or per-task staging area.
 *
 * Non-temporal stores:
 *   On flush, uses SSE2 stream stores on x86/x86_64 when data is
 *   16-byte aligned. On other platforms falls back to a plain memcpy.
 *   The caller is responsible for cache-line alignment when non-temporal
 *   stores matter.
 */

/* Maximum number of distinct write entries the WCB can hold. */
inline constexpr IUC WcbMaxEntries = 64;

/*
 * One entry in the write-combining buffer.
 *
 * Each entry tracks one (address, data) pair. The valid bit says whether
 * the entry is live. On flush, valid entries are written back to the
 * backing store in insertion order.
 */
struct WcbEntry {
  void* address = nullptr;  //< Target memory address.
  IUA data[8];              //< Buffered data (up to 8 bytes).
  BOL valid = false;        //< Is this entry live?
};

/*
 * Write-Combining Buffer Cache.
 *
 * The WCB wraps a region of memory. Writes queue in the buffer; reads
 * bypass it entirely. Flush commits all buffered writes in one batch.
 *
 * Usage:
 *   Wcb wcb;
 *   wcb.Init(buffer, buffer_size);
 *   wcb.Write(address, &data, sizeof(data));
 *   // ... CPU continues without stalling ...
 *   wcb.Flush();  // All writes go to memory at once.
 */
class Wcb {
  static_assert(std::is_trivially_copyable_v<WcbEntry>);

 public:
  Wcb() = default;

  /* Initialize the WCB with a backing-store pointer and size.
   * Must be called before any Write or Flush. Returns false on
   * null pointer or zero size. */
  BOL Init(void* buffer, IUC size) {
    if (!buffer || size == 0) return false;
    store_ = static_cast<IUA*>(buffer);
    size_  = size;
    count_ = 0;
    return true;
  }

  /* Initialize with a typed backing store. */
  template<typename T>
  BOL Init(T& storage) {
    return Init(&storage, sizeof(storage));
  }

  /* Buffer a write to `address`. Data is copied into the WCB.
   * If the same address already has a pending write, it is updated
   * (coalescing). Returns false if the buffer is full and the address
   * is not already present. */
  BOL Write(const void* address, const void* data, IUC bytes) {
    if (!address || !data || bytes == 0 || bytes > 8) return false;
    if (!store_) return false;
    const IUA* addr = static_cast<const IUA*>(address);
    if (addr < store_ || addr >= store_ + size_) return false;

    /* Coalesce: if address already has a pending write, update it. */
    for (IUC i = 0; i < count_; ++i) {
      if (entries_[i].address == addr) {
        std::memcpy(entries_[i].data, data, bytes);
        return true;
      }
    }

    /* New entry: need space. */
    if (count_ >= WcbMaxEntries) return false;

    entries_[count_].address = const_cast<void*>(static_cast<const void*>(addr));
    std::memcpy(entries_[count_].data, data, bytes);
    entries_[count_].valid   = true;
    ++count_;
    return true;
  }

  /* Read directly from the backing store, bypassing the WCB.
   * This does NOT touch any buffered writes. */
  BOL Read(void* dest, const void* address, IUC bytes) const {
    if (!dest || !address || bytes == 0) return false;
    if (!store_) return false;
    const IUA* addr = static_cast<const IUA*>(address);
    if (addr < store_ || addr >= store_ + size_) return false;
    std::memcpy(dest, addr, bytes);
    return true;
  }

  /* Check whether a given address has a pending write in the buffer.
   * Returns true if the address is buffered, false otherwise. */
  BOL HasPending(const void* address) const {
    if (!address || !store_) return false;
    const IUA* addr = static_cast<const IUA*>(address);
    for (IUC i = 0; i < count_; ++i) {
      if (entries_[i].address == addr) return true;
    }
    return false;
  }

  /* Flush all buffered writes back to the backing store in insertion
   * order. After flush, the buffer is cleared. */
  void Flush() {
    if (!store_ || count_ == 0) return;

    for (IUC i = 0; i < count_; ++i) {
      if (!entries_[i].valid) continue;
      const IUA* src = entries_[i].data;
      IUA* dst = static_cast<IUA*>(entries_[i].address);

#if defined(__x86_64__) || defined(__i386__)
      /* SSE2 stream store for 16-byte aligned addresses.
       * We use a local 16-byte buffer so the read is always safe. */
      if ((reinterpret_cast<std::uintptr_t>(dst) & 15) == 0) {
        alignas(16) IUA block[16] = {};
        std::memcpy(block, src, 8);
        _mm_stream_si128(reinterpret_cast<__m128i*>(dst),
                         *reinterpret_cast<const __m128i*>(block));
        continue;
      }
#endif
      /* Plain copy for unaligned or small data. */
      std::memcpy(dst, src, 8);
    }

    /* Invalidate the buffer entries. */
    for (IUC i = 0; i < count_; ++i) {
      entries_[i].valid = false;
      entries_[i].address = nullptr;
    }
    count_ = 0;
  }

  /* Number of pending (buffered) writes. */
  IUC Count() const { return count_; }

  /* Whether the buffer is full. */
  BOL IsFull() const { return count_ >= WcbMaxEntries; }

  /* Whether the buffer is empty. */
  BOL IsEmpty() const { return count_ == 0; }

  /* Clear all pending writes without flushing. */
  void Clear() {
    for (IUC i = 0; i < count_; ++i) {
      entries_[i].valid = false;
      entries_[i].address = nullptr;
    }
    count_ = 0;
  }

  /* The backing store pointer (for inspection). */
  const IUA* Store() const { return store_; }

  /* The backing store size in bytes. */
  IUC Size() const { return size_; }

 private:
  WcbEntry entries_[WcbMaxEntries];
  IUA* store_ = nullptr;
  IUC size_ = 0;
  IUC count_ = 0;
};

}  // namespace KT

#endif
