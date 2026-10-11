// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_ROOM_HPP
#define CRABSTK_WHO_ROOM_HPP
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
#include "TSessionTable.hpp"
#include "TMessage.hpp"
#include <cstring>
#include <cstdlib>
#include <atomic>
namespace _ {

/* A room (channel) in the chat server.
A room owns:
  - A growable array of connected session slots.
  - A per-room message ring (for replay / late-joiner catch-up).

Adding/removing a session to a room is O(1) amortized.
Broadcast to a room iterates the session slot array (cache-friendly,
sequential memory access).
*/
struct TRoom {
  TRoom(IUC room_uid = 0, IUC ring_capacity = 512)
      : room_uid_(room_uid),
        rings_(0),
        member_total_(CRABSTK_WHO_ROOM_TOTAL_DEFAULT),
        member_count_(0) {
    members_ = (IUC*)::calloc((size_t)member_total_ * sizeof(IUC), 1);
    rings_ = ring_capacity ? new AMessageRing(ring_capacity) : NILP;
  }

  /* Destructor: frees the member array and the replay ring. */
  ~TRoom() {
    if (members_) ::free(members_);
    if (rings_) delete rings_;
  }

  TRoom(const TRoom&) = delete;
  TRoom& operator=(const TRoom&) = delete;

  /* Gets the room uid. */
  IUC RoomUID() const { return room_uid_; }

  /* Gets the number of connected sessions in this room. */
  IUC MemberCount() const { return member_count_; }

  /* Adds a session slot to this room.
  @pre Must be called by the single writer thread only.
  @param slot The session slot index.
  @return true on success. */
  BOL MemberAdd(IUC slot) {
    if (member_count_ >= member_total_) {
      member_total_ *= 2;
      IUC* new_members = (IUC*)::realloc(
          members_, (size_t)member_total_ * sizeof(IUC));
      if (!new_members) return false;
      members_ = new_members;
    }
    members_[member_count_++] = slot;
    return true;
  }

  /* Removes a session slot from this room (swap-remove, O(1) amortized).
  @pre Must be called by the single writer thread only.
  @param slot The session slot index.
  @return true if the slot was found and removed. */
  BOL MemberRemove(IUC slot) {
    for (IUC i = 0; i < member_count_; ++i) {
      if (members_[i] == slot) {
        members_[i] = members_[member_count_ - 1];
        --member_count_;
        return true;
      }
    }
    return false;
  }

  /* True if the session slot is in this room. */
  BOL MemberContains(IUC slot) const {
    for (IUC i = 0; i < member_count_; ++i) {
      if (members_[i] == slot) return true;
    }
    return false;
  }

  /* Gets a pointer to the member slot array. */
  IUC* Members() { return members_; }
  const IUC* Members() const { return members_; }

  /* Gets the per-room message ring (for replay). */
  AMessageRing* Rings() { return rings_; }
  const AMessageRing* Rings() const { return rings_; }

  /* Writes a message to the room's replay ring (single writer). */
  IUC RingWrite(const TMessage* msg) {
    return rings_ ? rings_->Write(msg) : 0;
  }

  /* Writes a message to the replay ring, copying only used text bytes. */
  IUC RingWriteShort(const TMessage* msg) {
    return rings_ ? rings_->WriteShort(msg) : 0;
  }

  IUC room_uid_;          //< Unique room id.
  IUC* members_;          //< Session slots in this room (heap).
  IUC member_total_;      //< Capacity of members_.
  IUC member_count_;      //< Active member count.
  AMessageRing* rings_;   //< Per-room message ring for replay (heap).
};                        //< struct TRoom

/* An array of rooms, indexed by room uid.
Grows on demand when a new room uid is created.
*/
struct ARoomTable {
  ARoomTable(IUC initial_total = 16) {
    total_ = initial_total;
    origin_ = (TRoom*)::calloc((size_t)total_ * sizeof(TRoom), 1);
    count_ = 0;
  }

  ~ARoomTable() {
    if (origin_) {
      for (IUC i = 0; i < total_; ++i) {
        if (origin_[i].RoomUID() != 0) origin_[i].~TRoom();
      }
      ::free(origin_);
    }
  }

  ARoomTable(const ARoomTable&) = delete;
  ARoomTable& operator=(const ARoomTable&) = delete;

  /* Gets a room by uid, creating it if it doesn't exist.
  @pre Must be called by the single writer thread only.
  @param room_uid The room uid.
  @return Pointer to the TRoom, or NILP on allocation failure. */
  TRoom* Get(IUC room_uid) {
    if (room_uid >= total_) Grow(room_uid + 1);
    if (origin_[room_uid].RoomUID() == 0) {
      new (&origin_[room_uid]) TRoom(room_uid);
      ++count_;
    }
    return &origin_[room_uid];
  }

  /* Gets a room by uid without creating it. */
  TRoom* Find(IUC room_uid) const {
    if (room_uid >= total_) return NILP;
    if (origin_[room_uid].RoomUID() == 0) return NILP;
    return const_cast<TRoom*>(&origin_[room_uid]);
  }

  /* Gets the number of active rooms. */
  IUC Count() const { return count_; }

  /* Gets the total capacity. */
  IUC Total() const { return total_; }

  /* Grows the table to at least the given total. */
  void Grow(IUC new_total) {
    if (new_total <= total_) return;
    IUC old_total = total_;
    TRoom* new_origin = (TRoom*)::realloc(
        origin_, (size_t)new_total * sizeof(TRoom));
    if (!new_origin) return;  // allocation failed.
    ::memset(new_origin + old_total, 0,
             (size_t)(new_total - old_total) * sizeof(TRoom));
    origin_ = new_origin;
    total_ = new_total;
  }

 private:
  TRoom* origin_;  //< Heap block of TRoom[total_].
  IUC total_;      //< Capacity.
  IUC count_;      //< Active room count.
};                 //< struct ARoomTable

}  //< namespace _
#endif
#endif
