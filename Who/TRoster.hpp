// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_ROSTER_HPP
#define CRABSTK_WHO_ROSTER_HPP
#include "_Config.h"
#if SEAM >= CRABSTK_WHO_CORE
#include "TSessionTable.hpp"
#include "TRoom.hpp"
#include "TMessage.hpp"
#include "TUIDAssigner.hpp"
#include <ctime>
#include <atomic>
namespace _ {

/* The TRoster is the top-level Who API for a chat server.
It owns:
  - ASessionTable: the session hash table (connect/disconnect/find).
  - ARoomTable: the room/channel table.
  - TUIDAssigner: the UID counter.
  - AMessageRing: the global outbound message ring (for the send loop).

Threading model (the single-writer rule):
  - The EVENT thread (one) calls Connect/Disconnect/SendMessage/Moderate.
  - The SEND thread (one or more) drains the global ring and writes to sockets.
  - READER threads (any number) may call SnapshotStats/Find concurrently.

This is the stress-test surface for CrabsTK: at 100K+ messages/sec the
hot path is Find(handle) + RingWrite + Room broadcast, all allocation-free.
*/
struct TRoster {
  TRoster(IUC session_total = CRABSTK_WHO_SESSION_TOTAL_DEFAULT,
          IUC ring_capacity = 1024)
      : sessions_(session_total),
        rooms_(),
        uids_(),
        outbound_(ring_capacity),
        connect_count_(0),
        disconnect_count_(0),
        message_count_(0),
        grow_count_(0),
        ts_cache_(0),
        ts_cache_tick_(0) {}

  /* Connects a new session.
  @param handle UTF-8 handle string.
  @param length Byte length (pass -1 to compute).
  @param room   Room (channel) index (0 = default).
  @return Pointer to the new TSession, or NILP on failure.
  @pre Event thread only. */
  TSession* Connect(const CHA* handle, IUB length = (IUB)-1,
                    IUC room = 0) {
    /* Grow the table before insert if needed (avoids full-table insert). */
    if (sessions_.GrowNeeded()) {
      if (sessions_.Grow()) ++grow_count_;
    }
    IUD uid = uids_.Next();
    TSession* session = sessions_.Insert(handle, length, uid, room);
    if (!session) return NILP;
    /* Add to the room. */
    if (room != 0) {
      TRoom* room_ptr = rooms_.Get(room);
      if (room_ptr) room_ptr->MemberAdd(session->slot_);
    }
    ++connect_count_;
    return session;
  }

  /* Disconnects a session by handle.
  @param handle UTF-8 handle string.
  @param length Byte length (pass -1 to compute).
  @return true if a session was actually disconnected.
  @pre Event thread only. */
  BOL Disconnect(const CHA* handle, IUB length = (IUB)-1) {
    TSession* session = sessions_.Find(handle, length);
    if (!session) return false;
    IUC slot = session->slot_;
    IUC room = session->Room();
    /* Remove from room. */
    if (room != 0) {
      TRoom* room_ptr = rooms_.Find(room);
      if (room_ptr) room_ptr->MemberRemove(slot);
    }
    sessions_.Remove(slot);
    ++disconnect_count_;
    /* Shrink if over-allocated. */
    if (sessions_.ShrinkNeeded()) sessions_.Shrink();
    return true;
  }

  /* Disconnects a session by slot index (faster, avoids hash lookup).
  @pre Event thread only. */
  BOL DisconnectSlot(IUC slot) {
    TSession* session = &sessions_.Slots()[slot];
    if (session->Free()) return false;
    IUC room = session->Room();
    if (room != 0) {
      TRoom* room_ptr = rooms_.Find(room);
      if (room_ptr) room_ptr->MemberRemove(slot);
    }
    sessions_.Remove(slot);
    ++disconnect_count_;
    if (sessions_.ShrinkNeeded()) sessions_.Shrink();
    return true;
  }

  /* Sends a chat message from a connected session.
  @param handle The sender's handle.
  @param text   The UTF-8 message text.
  @param type   The message type code.
  @return the number of sessions the message was broadcast to, or -1 on error.
  @pre Event thread only.
  @note This is the hot path: Find + validate + RingWrite + Room broadcast.
        All steps are allocation-free. */
  ISN SendMessage(const CHA* handle, const CHA* text,
                  IUA type = TMessage::Chat, IUB length = (IUB)-1) {
    TSession* session = sessions_.Find(handle, length);
    if (!session) return -1;
    if (!session->CanSend()) return -1;
    /* Batched timestamp: time() is a syscall; at 100K+ msg/s we must not
    pay it per message. Refresh the cached ts at most once per 1024 sends. */
    if ((++ts_cache_tick_ & 1023) == 0) ts_cache_ = ClockTS();
    TMessage msg(session->uid_, ts_cache_, type, session->Room(), text,
                 (IUB)-1);
    /* Write to the global outbound ring (the send loop consumes this).
    WriteShort copies only the used text bytes — the hot path. */
    outbound_.WriteShort(&msg);
    /* Write to the room's replay ring and count the fanout.
    The session already carries its room, so this is one lookup, not two. */
    IUC room = session->Room();
    ISN fanout = 0;
    if (room != 0) {
      TRoom* room_ptr = rooms_.Find(room);
      if (room_ptr) {
        room_ptr->RingWriteShort(&msg);
        fanout = (ISN)room_ptr->MemberCount();
      }
    }
    session->SentIncrement();
    session->LastTS(ts_cache_);
    ++message_count_;
    return fanout;
  }

  /* Sends a system message (server-generated).
  @param room  The room to broadcast to.
  @param text  The UTF-8 message text.
  @return the number of sessions the message was broadcast to.
  @pre Event thread only. */
  ISN SendSystem(IUC room, const CHA* text) {
    if ((++ts_cache_tick_ & 1023) == 0) ts_cache_ = ClockTS();
    TMessage msg(0, ts_cache_, TMessage::System, room, text, (IUB)-1);
    outbound_.WriteShort(&msg);
    ISN fanout = 0;
    if (room != 0) {
      TRoom* room_ptr = rooms_.Find(room);
      if (room_ptr) {
        room_ptr->RingWriteShort(&msg);
        fanout = (ISN)room_ptr->MemberCount();
      }
    }
    ++message_count_;
    return fanout;
  }

  /* Mutes a session (blocks its send path).
  @pre Event thread only. */
  BOL Mute(const CHA* handle, IUB length = (IUB)-1) {
    TSession* session = sessions_.Find(handle, length);
    if (!session) return false;
    session->StateSet(TSession::Muted);
    return true;
  }

  /* Unmutes a session.
  @pre Event thread only. */
  BOL Unmute(const CHA* handle, IUB length = (IUB)-1) {
    TSession* session = sessions_.Find(handle, length);
    if (!session) return false;
    session->StateSet(TSession::Online);
    return true;
  }

  /* Bans a session (read-only, kicked on next tick).
  @pre Event thread only. */
  BOL Ban(const CHA* handle, IUB length = (IUB)-1) {
    TSession* session = sessions_.Find(handle, length);
    if (!session) return false;
    session->StateSet(TSession::Banned);
    return true;
  }

  /* Gets the number of currently connected sessions. */
  IUC ConnectedCount() const { return sessions_.Count(); }

  /* Gets the total sessions ever connected. */
  IUD ConnectTotal() const { return connect_count_; }

  /* Gets the total sessions ever disconnected. */
  IUD DisconnectTotal() const { return disconnect_count_; }

  /* Gets the total messages sent. */
  IUD MessageTotal() const { return message_count_; }

  /* Gets the total table grow operations. */
  IUC GrowTotal() const { return grow_count_; }

  /* Gets the session table (for direct access in the send loop). */
  ASessionTable& Sessions() { return sessions_; }
  const ASessionTable& Sessions() const { return sessions_; }

  /* Gets the room table. */
  ARoomTable& Rooms() { return rooms_; }
  const ARoomTable& Rooms() const { return rooms_; }

  /* Gets the global outbound message ring (for the send loop). */
  AMessageRing& Outbound() { return outbound_; }
  const AMessageRing& Outbound() const { return outbound_; }

  /* Gets the UID assigner. */
  TUIDAssigner& UIDs() { return uids_; }
  const TUIDAssigner& UIDs() const { return uids_; }

  /* Gets a snapshot of the session table stats (thread-safe). */
  struct Stats {
    IUC connected;      //< Currently connected.
    IUC table_total;    //< Table capacity.
    IUC table_count;    //< Occupied slots.
    FPD load_factor;    //< Occupancy ratio.
    IUD messages;       //< Total messages sent.
    IUD connects;       //< Total connects.
    IUD disconnects;    //< Total disconnects.
  };

  Stats SnapshotStats() const {
    Stats s;
    s.connected = sessions_.Count();
    s.table_total = sessions_.Total();
    s.table_count = sessions_.Count();
    s.load_factor = sessions_.Load();
    s.messages = message_count_;
    s.connects = connect_count_;
    s.disconnects = disconnect_count_;
    return s;
  }

 private:
  /* Gets the current wall clock timestamp in seconds since epoch. */
  static TMD ClockTS() {
    return (TMD)::time(nullptr);
  }

  ASessionTable sessions_;    //< Session hash table.
  ARoomTable rooms_;          //< Room/channel table.
  TUIDAssigner uids_;         //< UID counter.
  AMessageRing outbound_;     //< Global outbound ring.
  IUD connect_count_;         //< Total connects.
  IUD disconnect_count_;      //< Total disconnects.
  IUD message_count_;         //< Total messages.
  IUC grow_count_;            //< Table grow count.
  TMD ts_cache_;              //< Cached wall-clock ts (batched, not per-msg).
  IUD ts_cache_tick_;         //< Message counter for the ts batch refresh.
};                            //< struct TRoster

}  //< namespace _
#endif
#endif
