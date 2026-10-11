// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_UIDASSIGNER_HPP
#define CRABSTK_WHO_UIDASSIGNER_HPP
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
#include <ASCIICrabs/Array.hpp>
#include <cstring>
#include <cstdlib>
#include <atomic>
#include <cstdio>
namespace _ {

/* A lock-free monotonically increasing UID assigner.
Uses a single atomic counter (fetch_add) so multiple threads can allocate
UIDs concurrently without a mutex. UIDs are positive integers; 0 is reserved
as the invalid/nil sentinel.

For persistence across restarts, call Save() periodically and Load() at
startup. The on-disk format is a single 64-bit big-endian integer.
*/
struct TUIDAssigner {
  static const IUD cInvalidUid = 0;  //< Nil UID.

  /* Constructs the assigner starting at the given uid. */
  TUIDAssigner(IUD init_uid = 1)
      : next_uid_(init_uid <= cInvalidUid ? cInvalidUid + 1 : init_uid) {}

  /* Gets the next uid without consuming it. */
  IUD Peek() const { return next_uid_.load(::std::memory_order_relaxed); }

  /* Allocates and returns the next uid (atomic, thread-safe). */
  IUD Next() { return next_uid_.fetch_add(1, ::std::memory_order_relaxed); }

  /* Sets the counter to the given value (for Load or testing). */
  void Set(IUD uid) {
    next_uid_.store(uid <= cInvalidUid ? cInvalidUid + 1 : uid,
                    ::std::memory_order_relaxed);
  }

  /* Gets the current counter value (next uid to be allocated). */
  IUD Count() const { return next_uid_.load(::std::memory_order_relaxed); }

  /* Saves the current counter to the given file.
  @return 0 on success, -1 on failure. */
  ISN Save(const CHA* filename) const {
    if (!filename) return -1;
    FILE* file = ::fopen(filename, "wb");
    if (!file) return -1;
    IUD value = next_uid_.load(::std::memory_order_relaxed);
    /* Write as 8 bytes big-endian. */
    CHA bytes[8];
    for (ISN i = 0; i < 8; ++i)
      bytes[i] = (CHA)(value >> (56 - i * 8));
    size_t written = ::fwrite(bytes, 1, 8, file);
    ::fclose(file);
    return written == 8 ? 0 : -1;
  }

  /* Loads the counter from the given file.
  @return 0 on success, -1 on failure. */
  ISN Load(const CHA* filename) {
    if (!filename) return -1;
    FILE* file = ::fopen(filename, "rb");
    if (!file) return -1;
    CHA bytes[8];
    size_t read = ::fread(bytes, 1, 8, file);
    ::fclose(file);
    if (read != 8) return -1;
    IUD value = 0;
    for (ISN i = 0; i < 8; ++i)
      value = (value << 8) | (IUD)(IUA)bytes[i];
    Set(value);
    return 0;
  }

  ::std::atomic<IUD> next_uid_;  //< Next uid to allocate.
};                               //< struct TUIDAssigner

}  //< namespace _
#endif
#endif
