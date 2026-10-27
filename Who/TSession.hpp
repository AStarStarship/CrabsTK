// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_SESSION_HPP
#define CRABSTK_WHO_SESSION_HPP
#include "_Config.h"
#if SEAM >= CRABSTK_WHO_CORE
#include "Global.h"
#include <cstring>
#include <atomic>
namespace _ {

/* A single chat session.
A session is the identity of one connected client. It is the lightest unit
of Who: connecting a client is an O(1) allocation of a TSession; disconnecting
is O(1) (lazy tombstone). All mutable fields are written by the event thread
only (the single-writer rule of the chat server), so no locking is required
on the hot path. Readers (other threads, UI) snapshot via the atomics.

@code
+---------------------------+
|   TSession (cache line)   |
| uid_      : IUD           |
| handle_   : IUC (hash)    |
| state_    : IUA           |
| color_    : IUB           |
| role_     : IUA           |
| room_     : IUC           |
| sent_/    : atomic<IUD>   |
| last_ts_  : atomic<TMD>   |
| name_     : CHA[HandleMax]|
+---------------------------+
@endcode
*/
struct TSession {
  enum {
    HandleMax = CRABSTK_WHO_HANDLE_LENGTH_MAX,  //< Max UTF-8 handle bytes.
  };
  /* Session state codes (mutable, written by event thread). */
  enum State {
    Free = 0,      //< Tombstone: slot vacated, hash empty.
    Connecting,    //< Handshake in progress (socket accepted, no handle).
    Online,        //< Connected, handle set, may send and receive.
    Muted,         //< Connected, send path blocked by moderation.
    Banned,        //< Connected, read-only; kicked on next event tick.
    Draining,      //< Graceful shutdown requested.
  };
  /* Role codes (immutable-ish, set at auth or by moderator). */
  enum Role {
    Viewer = 0,    //< Read-only chat (default).
    Moderator,     //< May mute/ban viewers.
    Broadcaster,   //< The stream owner.
    Admin,         //< Full server control.
  };

  /* Zeroes a session into the Free state. */
  TSession()
      : uid_(0),
        handle_(0),
        state_(Free),
        color_(0),
        role_(Viewer),
        room_(0),
        sent_(0),
        last_ts_(0) {
    name_[0] = '\0';
  }

  /* Copy constructor (atomic members copied by load). */
  TSession(const TSession& other)
      : uid_(other.uid_),
        slot_(other.slot_),
        handle_(other.handle_),
        state_(other.state_),
        role_(other.role_),
        color_(other.color_),
        room_(other.room_),
        sent_(other.sent_.load(::std::memory_order_relaxed)),
        last_ts_(other.last_ts_.load(::std::memory_order_relaxed)) {
    ::memcpy(name_, other.name_, sizeof(name_));
  }

  /* Copy assignment (atomic members copied by load/store).
  Used by ASessionTable::Rehash to move sessions between slots. */
  TSession& operator=(const TSession& other) {
    if (this == &other) return *this;
    uid_ = other.uid_;
    slot_ = other.slot_;
    handle_ = other.handle_;
    state_ = other.state_;
    role_ = other.role_;
    color_ = other.color_;
    room_ = other.room_;
    sent_.store(other.sent_.load(::std::memory_order_relaxed),
                ::std::memory_order_relaxed);
    last_ts_.store(other.last_ts_.load(::std::memory_order_relaxed),
                   ::std::memory_order_relaxed);
    ::memcpy(name_, other.name_, sizeof(name_));
    return *this;
  }

  /* True if this session slot is vacant (a tombstone).
  Named IsFree (not Free): Free is already the State-enum value for the
  tombstone, and a member function named Free would conflict with it
  (-Wchanges-meaning / "conflicts with a previous declaration"). */
  BOL IsFree() const { return state_ == Free; }

  /* True if this session is connected (any post-handshake state). */
  BOL Connected() const {
    return state_ >= Connecting && state_ <= Draining;
  }

  /* True if this session may send messages. */
  BOL CanSend() const { return state_ == Online || state_ == Banned; }

  /* Gets the unique session id. */
  IUD UID() const { return uid_; }

  /* Gets the session slot index. */
  IUC Slot() const { return slot_; }

  /* Sets the session slot index (set once at allocation by the table). */
  void SlotSet(IUC slot) { slot_ = slot; }

  /* Gets a pointer to the UTF-8 handle. */
  const CHA* Handle() const { return name_; }

  /* Gets the handle byte length. */
  IUB HandleLength() const {
    IUB length = 0;
    while (length < HandleMax && name_[length]) ++length;
    return length;
  }

  /* Copies a UTF-8 handle into this session.
  @param handle UTF-8 handle string.
  @param length Byte length, or (IUB)-1 to compute from the NUL terminator.
  @return false if the handle is empty or too long.
  @note The length is always clamped to the actual NUL-terminated length,
        so a bogus (too-long) length can never overrun the buffer. */
  BOL HandleSet(const CHA* handle, IUB length = (IUB)-1) {
    if (!handle) return false;
    /* Compute the real NUL-terminated length (bounded by HandleMax). */
    IUB real = 0;
    while (real < HandleMax && handle[real]) ++real;
    if (real == 0 || real >= HandleMax) return false;
    /* Clamp: never copy more than the NUL-terminated length. */
    if (length != (IUB)-1 && length < real) length = real;
    else length = real;
    ::memcpy(name_, handle, length);
    name_[length] = '\0';
    /* FNV-1a 32-bit over the UTF-8 bytes. */
    IUC hash = 2166136261u;
    for (IUB i = 0; i < length; ++i) {
      hash ^= (IUC)(IUA)handle[i];
      hash *= 16777619u;
    }
    handle_ = hash;
    return true;
  }

  /* Gets the FNV-1a 32-bit handle hash. */
  IUC HandleHash() const { return handle_; }

  /* Gets the session state code. */
  IUA StateGet() const { return state_; }

  /* Sets the session state code (event thread only). */
  void StateSet(IUA state) { state_ = state; }

  /* Gets the role code. */
  IUA RoleGet() const { return role_; }

  /* Sets the role code (event thread only). */
  void RoleSet(IUA role) { role_ = role; }

  /* Gets the display color (RGB packed into IUB). */
  IUB Color() const { return color_; }

  /* Sets the display color. */
  void ColorSet(IUB color) { color_ = color; }

  /* Gets the room (channel) index this session belongs to. */
  IUC Room() const { return room_; }

  /* Sets the room (channel) index. */
  void RoomSet(IUC room) { room_ = room; }

  /* Gets the total messages sent by this session (atomic snapshot). */
  IUD Sent() const { return sent_.load(::std::memory_order_relaxed); }

  /* Increments the sent counter. */
  void SentIncrement(IUD amount = 1) {
    sent_.fetch_add(amount, ::std::memory_order_relaxed);
  }

  /* Gets the last activity timestamp (seconds since epoch). */
  TMD LastTS() const { return last_ts_.load(::std::memory_order_relaxed); }

  /* Sets the last activity timestamp. */
  void LastTS(TMD ts) { last_ts_.store(ts, ::std::memory_order_relaxed); }

  /* Marks the session free (tombstone), wiping the handle bytes. */
  void FreeSet() {
    state_ = Free;
    uid_ = 0;
    handle_ = 0;
    sent_.store(0, ::std::memory_order_relaxed);
    last_ts_.store(0, ::std::memory_order_relaxed);
    name_[0] = '\0';
  }

  IUD uid_;              //< Unique session id (from TUIDAssigner).
  IUC slot_;             //< Index in the ASessionTable.
  IUC handle_;           //< FNV-1a 32-bit hash of the handle.
  IUA state_;            //< Current State code.
  IUA role_;             //< Current Role code.
  IUB color_;            //< Display color.
  IUC room_;             //< Room (channel) index.
  ::std::atomic<IUD> sent_;   //< Total messages sent.
  ::std::atomic<TMD> last_ts_; //< Last activity timestamp (seconds).
  CHA name_[HandleMax + 1];   //< UTF-8 handle, NUL-terminated.
};                          //< struct TSession



}  //< namespace _
#endif
#endif
