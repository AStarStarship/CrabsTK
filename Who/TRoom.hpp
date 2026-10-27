// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_ROOM_HPP
#define CRABSTK_WHO_ROOM_HPP
#include "_Config.h"
#if SEAM >= CRABSTK_WHO_CORE
#include "Global.h"
#include "TMessage.hpp"
#include "TSession.hpp"
#include <ASCIICrabs/Autoject.hxx>  // RAMFactory / ObjectFactoryHeap
#include <ASCIICrabs/Binary.hpp>    // IsError
#include <ASCIICrabs/Array.hxx>     // ArrayFill / ArrayCopy definitions
#include <cstring>
namespace _ {

/* Allocates a zero-initialized, word-aligned buffer of @c bytes via the
given RAMFactory (heap: ObjectFactoryHeap). Returns the origin, or an error
pointer (IUW < AErrorTotal) on failure. Mirrors AujAlloc in BigInt.hxx. */
static inline IUW* RoomAlloc(RAMFactory ram, ISW bytes) {
  return ram(NILP, bytes);
}
/* Frees a buffer previously allocated by RoomAlloc (heap factory deletes). */
static inline void RoomFree(RAMFactory ram, IUW* origin) {
  if (!IsError(origin)) ram(origin, 0);
}

/* A chat room (channel).
A TRoom is a named channel that members belong to. Each room carries:
  - a contiguous member list of session slot indexes (IUC), and
  - a replay ring (AMessageRing) of the room's recent messages, so a late
    joiner can catch up.
Threading model: the EVENT thread (single writer) calls MemberAdd/
MemberRemove/RingWriteShort. Readers (the send loop, UI) read MemberCount
and the ring concurrently; the ring is SPSC-safe by construction (see
AMessageRing). No mutexes on the hot path.

Dynamic memory (the member list) is managed through the CrabsTK/ASCIICrabs
RAMFactory (ObjectFactoryHeap), not raw calloc/new — the no-stdlib rule.
The replay ring uses the AMessageRing's own allocation.

Room 0 is the GLOBAL room (room_ == 0 in a TSession means "global").
*/
struct TRoom {
  enum {
    NameMax = 63,            //< Max room name bytes (UTF-8, NUL-terminated).
    MemberDefault = 64,      //< Initial member-list capacity (grows x2).
  };
  /* Constructs the room with an empty (zero-initialized) member list. */
  TRoom(IUC room = 0)
      : room_(room),
        member_count_(0),
        member_capacity_(MemberDefault),
        ram_(ObjectFactoryHeap),
        ring_(256) {
    name_[0] = '\0';
    members_ = TPtr<IUC>(RoomAlloc(ram_, ISW(MemberDefault) * sizeof(IUC)));
    if (!IsError(members_)) {
      // Zero the member slots (calloc-equivalent via ArrayFill).
      ArrayFill(members_, ISW(MemberDefault) * sizeof(IUC));
    }
  }

  /* Destroys the room, freeing the member list through the RAMFactory.
  (The ring frees itself.) */
  ~TRoom() {
    if (!IsError(members_)) RoomFree(ram_, TPtr<IUW>(members_));
  }

  /* Disables copy (the room owns a RAMFactory-managed heap block). */
  TRoom(const TRoom&) = delete;
  TRoom& operator=(const TRoom&) = delete;

  /* Gets the room index. */
  IUC UID() const { return room_; }

  /* Gets the room name (UTF-8, NUL-terminated). */
  const CHA* Name() const { return name_; }

  /* Sets the room name (clamped to NameMax). */
  BOL NameSet(const CHA* name, IUB length = (IUB)-1) {
    if (!name) return false;
    IUC real = 0;
    if (length == (IUB)-1) {
      while (real < NameMax && name[real]) ++real;
    } else {
      real = length;
    }
    if (real > NameMax) real = NameMax;
    ::memcpy(name_, name, (size_t)real);
    name_[real] = '\0';
    return true;
  }

  /* Adds a session slot to the member list (grows x2 if full).
  @pre Event thread only. */
  BOL MemberAdd(IUC slot) {
    if (member_count_ >= member_capacity_) {
      IUC new_cap = member_capacity_ * 2;
      IUW* new_block = RoomAlloc(ram_, ISW(new_cap) * sizeof(IUC));
      if (IsError(new_block)) return false;
      IUC* new_members = TPtr<IUC>(new_block);
      // Copy existing members, zero the new tail.
      ArrayCopy(new_members, ISW(member_count_) * sizeof(IUC), members_,
                ISW(member_count_) * sizeof(IUC));
      ArrayFill(new_members + member_count_,
                ISW(new_cap - member_count_) * sizeof(IUC));
      RoomFree(ram_, TPtr<IUW>(members_));
      members_ = new_members;
      member_capacity_ = new_cap;
    }
    members_[member_count_++] = slot;
    return true;
  }

  /* Removes a session slot from the member list (swap-and-pop).
  @pre Event thread only.
  @return true if the slot was present. */
  BOL MemberRemove(IUC slot) {
    for (IUC i = 0; i < member_count_; ++i) {
      if (members_[i] == slot) {
        members_[i] = members_[--member_count_];
        return true;
      }
    }
    return false;
  }

  /* Gets the number of members. */
  IUC MemberCount() const { return member_count_; }

  /* Gets the member slot at the given index, or IUC(-1) if out of range. */
  IUC MemberAt(IUC i) const {
    return (i < member_count_ && !IsError(members_))
               ? members_[i]
               : IUC(-1);
  }

  /* Writes a message to the room's replay ring (hot path, WriteShort).
  @pre Event thread only. */
  IUC RingWriteShort(const TMessage* msg) { return ring_.WriteShort(msg); }

  /* Reads the next replayed message (for late-joiner catch-up).
  @return true if a message was read. */
  BOL RingRead(TMessage* msg) { return ring_.Read(msg); }

  /* Gets the replay ring (for the send loop to drain). */
  AMessageRing& Ring() { return ring_; }
  const AMessageRing& Ring() const { return ring_; }

  IUC room_;                //< Room index (0 = global).
  CHA name_[NameMax + 1];   //< Room name, NUL-terminated.
  IUC* members_;            //< RAMFactory block: IUC[member_capacity_].
  IUC member_count_;        //< Occupied member slots.
  IUC member_capacity_;     //< Allocated member slots.
  RAMFactory ram_;          //< Heap factory for the member block.
  AMessageRing ring_;       //< Replay ring of recent room messages.
};                           //< struct TRoom

/* A fixed-size table of TRoom.
Rooms are allocated lazily on first GetOrCreate into a single
RAMFactory-managed heap block (the ASessionTable pattern): one block holds
total_ slots, and only the slots that become active are constructed (via
placement new) and destructed. No eager default-construction of the whole
array, so there is no double-free.
*/
struct ARoomTable {
  enum {
    RoomTotalDefault = CRABSTK_WHO_ROOM_TOTAL_DEFAULT,  //< Max rooms (64).
  };
  /* Constructs the table: one heap block of total_ zeroed slots. All rooms
  start inactive; GetOrCreate constructs a room on first use. */
  ARoomTable(IUC total = RoomTotalDefault)
      : ram_(ObjectFactoryHeap),
        total_(total > RoomTotalDefault ? RoomTotalDefault : total) {
    blocks_ = RoomAlloc(ram_, ISW(total_) * sizeof(TRoom));
    if (!IsError(blocks_)) {
      // Zero the slot region so inactive slots are safe to test.
      ArrayFill(blocks_, ISW(total_) * sizeof(TRoom));
    }
    active_ = TPtr<BOL>(RoomAlloc(ram_, ISW(total_) * sizeof(BOL)));
    if (!IsError(active_)) ArrayFill(active_, ISW(total_) * sizeof(BOL));
  }

  /* Destroys the table: destructs active rooms, frees both heap blocks. */
  ~ARoomTable() {
    if (!IsError(blocks_)) {
      TRoom* rooms = Rooms();
      for (IUC i = 0; i < total_; ++i) {
        if (active_ && active_[i]) rooms[i].~TRoom();
      }
    }
    RoomFree(ram_, blocks_);
    RoomFree(ram_, TPtr<IUW>(active_));
  }

  /* Disables copy (owns RAMFactory blocks). */
  ARoomTable(const ARoomTable&) = delete;
  ARoomTable& operator=(const ARoomTable&) = delete;

  /* Gets the TRoom-element base of the slot block (blocks_ reinterpreted).
  Pointer arithmetic on this advances by sizeof(TRoom), not sizeof(IUW). */
  TRoom* Rooms() const { return TPtr<TRoom>(blocks_); }

  /* Gets the total room capacity. */
  IUC Total() const { return total_; }

  /* Gets a pointer to the room at the given index, or NILP if inactive. */
  TRoom* Get(IUC room) const {
    if (room >= total_ || IsError(blocks_) || !active_[room]) return NILP;
    return Rooms() + room;
  }

  /* Finds the room at the given index (alias of Get). */
  TRoom* Find(IUC room) const { return Get(room); }

  /* Gets or creates the room at the given index.
  @param room   Room index (0 = global).
  @param name   Optional name (default: "room_<index>", "global" for 0).
  @pre Event thread only.
  @return Pointer to the TRoom, or NILP if the table is full. */
  TRoom* GetOrCreate(IUC room, const CHA* name = NILP) {
    if (room >= total_ || IsError(blocks_)) return NILP;
    if (active_[room]) return Get(room);
    ::new (Rooms() + room) TRoom(room);
    TRoom* tr_room = Rooms() + room;
    if (name) {
      tr_room->NameSet(name);
    } else {
      CHA buf[32];
      IUC i = 0;
      if (room == 0) {
        const CHA* g = "global";
        while (g[i]) buf[i] = g[i++];
      } else {
        const CHA* prefix = "room_";
        while (prefix[i]) buf[i] = prefix[i++];
        IUC tmp = room;
        CHA digits[12];
        IUC dc = 0;
        while (tmp > 0) {
          digits[dc++] = (CHA)('0' + (tmp % 10));
          tmp /= 10;
        }
        for (IUC j = dc; j > 0; --j) buf[i++] = digits[j - 1];
      }
      buf[i] = '\0';
      tr_room->NameSet(buf);
    }
    active_[room] = true;
    return tr_room;
  }

  /* Iterates over all active rooms, calling fn(room, tr_room).
  @return the number of rooms visited. */
  template<typename Fn>
  IUC ForEach(Fn&& fn) const {
    IUC visited = 0;
    if (IsError(blocks_)) return 0;
    TRoom* rooms = Rooms();
    for (IUC i = 0; i < total_; ++i) {
      if (active_[i]) {
        fn(i, rooms + i);
        ++visited;
      }
    }
    return visited;
  }

 private:
  RAMFactory ram_;      //< Heap factory for the slot + active blocks.
  IUW* blocks_;         //< Heap block: TRoom[total_] (zeroed).
  BOL* active_;         //< Heap block: BOL[total_] (per-room active flag).
  IUC total_;           //< Room capacity.
};                        //< struct ARoomTable

}  //< namespace _
#endif
#endif
