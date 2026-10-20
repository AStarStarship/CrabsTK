// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_SESSIONTABLE_HPP
#define CRABSTK_WHO_SESSIONTABLE_HPP
#include "_Config.h"
#if SEAM >= CRABSTK_WHO_CORE
#include "TSession.hpp"
#include <ASCIICrabs/Array.hpp>
#include <cstdlib>
namespace _ {

/* A lock-free (single-writer) open-addressing hash table of TSession.
This is the hot path of an ultra-high volume chat server:
  - Insert (connect):  O(1) amortized, linear probing, FNV-1a hash.
  - Find (per message): one hash + probe sequence, no allocation.
  - Remove (disconnect): O(1) tombstone, no memory movement.
  - Prune: rehash when load factor exceeds 70% or occupancy drops below
    50% of total (shrinks).

Threading model: single writer (the event thread) calls Insert/Remove/Prune.
Any number of readers may call Find concurrently; TSession fields they read
are either immutable-after-connect or atomic. No mutexes on the hot path.

@code
Memory layout (heap block, 16-byte aligned):
+---------------------------+
| header: total, count      |  <-- ASessionTable struct (inline)
+---------------------------+
| TSession slot_0           |
| TSession slot_1           |
| ...                       |
| TSession slot_{total-1}   |
+---------------------------+
@endcode
*/
struct ASessionTable {
  /* Loads the probe sequence: home, home+1, home+2, ... (mod total).
  Standard linear probing; with FNV-1a and power-of-2 sizing this is
  cache-friendly because the probe sequence is contiguous. */

  /* Initializes an empty table with the given total slot capacity.
  @param total Minimum slots (rounded up to a power of 2 >= total). */
  ASessionTable(IUC total = CRABSTK_WHO_SESSION_TOTAL_DEFAULT) {
    total_ = Power2Up(total < 16 ? 16 : total);
    mask_ = total_ - 1;
    count_ = 0;
    origin_ = (TSession*)::calloc((size_t)total_ * sizeof(TSession), 1);
  }

  /* Moves the table from the source (single-writer construction). */
  ASessionTable(ASessionTable&& src)
      : total_(src.total_),
        mask_(src.mask_),
        count_(src.count_),
        origin_(src.origin_) {
    src.origin_ = NILP;
    src.total_ = 0;
    src.mask_ = 0;
    src.count_ = 0;
  }

  /* Destructs the table, freeing the heap block. */
  ~ASessionTable() {
    if (origin_) ::free(origin_);
  }

  /* Disables copy (the table owns a heap block). */
  ASessionTable(const ASessionTable&) = delete;
  ASessionTable& operator=(const ASessionTable&) = delete;

  /* Gets the slot array. */
  TSession* Slots() { return origin_; }
  const TSession* Slots() const { return origin_; }

  /* Gets the total slot capacity. */
  IUC Total() const { return total_; }

  /* Gets the number of occupied (non-free) slots. */
  IUC Count() const { return count_; }

  /* Gets the load factor (0.0 .. 1.0). */
  FPD Load() const {
    return total_ ? FPD((ISD)count_) / FPD((ISD)total_) : FPD(0.0);
  }

  /* True if the table needs a rehash (load factor >= 0.7). */
  BOL GrowNeeded() const { return count_ * 10 >= total_ * 7; }

  /* True if the table is over-allocated (occupancy < 50% and total > 16). */
  BOL ShrinkNeeded() const {
    return total_ > 16 && count_ * CRABSTK_WHO_SESSION_SHRINK_FACTOR < total_;
  }

  /* Finds the session with the given handle.
  @param handle UTF-8 handle string.
  @param length Byte length of handle (pass -1 to compute).
  @return Pointer to the TSession, or NILP if not found.
  @note Free slots are tombstones, not terminators: a key inserted after a
        tombstone in its probe chain must still be findable. We stop only
        after a full circuit (the table is kept < 70% full, so this is bounded
        and there is always a Free slot to end the circuit). */
  TSession* Find(const CHA* handle, IUB length = (IUB)-1) const {
    if (!handle) return NILP;
    if (length == (IUB)-1) {
      length = 0;
      while (length < TSession::HandleMax && handle[length]) ++length;
      if (length == 0) return NILP;
    }
    IUC hash = FNV1a(handle, length);
    IUC probe = hash & mask_;
    for (IUC i = 0; i < total_; ++i) {
      const TSession* session = &Slots()[probe];
      if (session->handle_ == hash &&
          session->HandleLength() == length &&
          ::memcmp(session->name_, handle, length) == 0) {
        return const_cast<TSession*>(session);
      }
      probe = (probe + 1) & mask_;
    }
    return NILP;  // full circuit: not present.
  }

  /* Finds the session by uid (linear scan; use Find(handle) on the hot path).
  @return Pointer to the TSession, or NILP if not found. */
  TSession* FindUID(IUD uid) const {
    if (uid == 0) return NILP;
    for (IUC i = 0; i < total_; ++i) {
      const TSession* session = &Slots()[i];
      if (!session->Free() && session->uid_ == uid)
        return const_cast<TSession*>(session);
    }
    return NILP;
  }

  /* Inserts a new session with the given handle.
  @pre Must be called by the single writer thread only.
  @param handle UTF-8 handle.
  @param length Byte length (pass -1 to compute).
  @param uid    The unique session id (from TUIDAssigner).
  @param room   The room (channel) index.
  @return Pointer to the new TSession, or NILP if the handle already exists
          or the table is full. */
  TSession* Insert(const CHA* handle, IUB length, IUD uid, IUC room = 0) {
    if (!handle) return NILP;
    if (length == (IUB)-1) {
      length = 0;
      while (length < TSession::HandleMax && handle[length]) ++length;
    }
    if (length == 0 || length >= TSession::HandleMax) return NILP;
    /* Reject duplicates: find first. */
    IUC hash = FNV1a(handle, length);
    IUC probe = hash & mask_;
    IUC free_probe = (IUC)-1;
    for (IUC i = 0; i < total_; ++i) {
      TSession* session = &Slots()[probe];
      IUA state = session->StateGet();
      if (state == TSession::Free) {
        if (free_probe == (IUC)-1) free_probe = probe;
        /* Keep probing in case of a duplicate after a tombstone. */
      } else if (session->handle_ == hash &&
                 session->HandleLength() == length &&
                 ::memcmp(session->name_, handle, length) == 0) {
        return NILP;  // duplicate handle.
      }
      probe = (probe + 1) & mask_;
    }
    if (free_probe == (IUC)-1) return NILP;  // table full: caller must Grow.
    TSession* session = &Slots()[free_probe];
    session->uid_ = uid;
    session->slot_ = free_probe;
    session->room_ = room;
    session->state_ = TSession::Connecting;
    session->HandleSet(handle, length);
    session->SentIncrement(0);
    ++count_;
    return session;
  }

  /* Removes (frees) the session at the given slot index.
  @pre Must be called by the single writer thread only.
  @param slot Index into Slots().
  @return true if a session was actually freed. */
  BOL Remove(IUC slot) {
    if (slot >= total_) return false;
    TSession* session = &Slots()[slot];
    if (session->Free()) return false;
    session->FreeSet();
    --count_;
    return true;
  }

  /* Removes the session with the given handle.
  @return true if a session was actually freed. */
  BOL Remove(const CHA* handle, IUB length = (IUB)-1) {
    TSession* session = Find(handle, length);
    if (!session) return false;
    return Remove(session->slot_);
  }

  /* Grows the table to at least double the current total.
  @pre Must be called by the single writer thread only.
  @return true on success. */
  BOL Grow() {
    return Rehash(Power2Up(total_ * 2));
  }

  /* Shrinks the table to half the current total (min 16). */
  BOL Shrink() {
    IUC new_total = total_ / 2;
    if (new_total < 16) new_total = 16;
    return Rehash(new_total);
  }

  /* Rehashes the table to the given new total (must be power of 2).
  @pre Must be called by the single writer thread only.
  @param new_total New slot count (power of 2).
  @return true on success. */
  BOL Rehash(IUC new_total) {
    if (new_total < 16) new_total = 16;
    TSession* new_slots =
        (TSession*)::calloc((size_t)new_total * sizeof(TSession), 1);
    if (!new_slots) return false;
    IUC new_mask = new_total - 1;
    IUC new_count = 0;
    for (IUC i = 0; i < total_; ++i) {
      TSession* session = &Slots()[i];
      if (session->Free()) continue;
      IUC hash = session->handle_;
      IUC probe = hash & new_mask;
      for (IUC j = 0; j < new_total; ++j) {
        if (new_slots[probe].Free()) break;
        probe = (probe + 1) & new_mask;
      }
      new_slots[probe] = *session;
      new_slots[probe].slot_ = probe;
      ++new_count;
    }
    ::free(origin_);
    origin_ = new_slots;
    total_ = new_total;
    mask_ = new_mask;
    count_ = new_count;
    return true;
  }

  /* Iterates over all occupied sessions, calling @c fn(slot, session).
  @return the number of sessions visited. */
  template<typename Fn>
  IUC ForEach(Fn&& fn) const {
    IUC visited = 0;
    for (IUC i = 0; i < total_; ++i) {
      if (!Slots()[i].Free()) {
        fn(i, &const_cast<TSession&>(Slots()[i]));
        ++visited;
      }
    }
    return visited;
  }

 private:
  /* Rounds up to the next power of 2 (min 16). */
  static IUC Power2Up(IUC value) {
    if (value < 16) return 16;
    IUC p = 16;
    while (p < value) p <<= 1;
    return p;
  }

  /* FNV-1a 32-bit hash over the given bytes. */
  static IUC FNV1a(const CHA* bytes, IUB length) {
    IUC hash = 2166136261u;
    for (IUB i = 0; i < length; ++i) {
      hash ^= (IUC)(IUA)bytes[i];
      hash *= 16777619u;
    }
    return hash;
  }

  TSession* origin_;  //< Heap block: [ASessionTable][TSession total_].
  IUC total_;         //< Slot capacity (power of 2).
  IUC mask_;          //< total_ - 1.
  IUC count_;         //< Occupied slot count.
};                     //< struct ASessionTable

}  //< namespace _
#endif
#endif
