// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_MESSAGE_HPP
#define CRABSTK_WHO_MESSAGE_HPP
#include "_Config.h"
#if SEAM >= CRABSTK_WHO_CORE
#include "Global.h"
#include "TSession.hpp"
#include <cstring>
#include <cstdlib>
#include <atomic>
namespace _ {

/* A single chat message.
A TMessage is a POD that can be copied into a ring buffer or sent over the
wire. The text is a fixed-size inline buffer (no heap allocation per message)
to keep the hot path allocation-free.

@code
+---------------------------+
| TMessage (64B cache line) |
| uid_      : IUD           |
| ts_       : TMD           |
| type_     : IUA           |
| room_     : IUC           |
| pad_      : IUC           |
| text_     : CHA[512]      |
+---------------------------+
@endcode
*/
struct TMessage {
  enum {
    TextMax = CRABSTK_WHO_MESSAGE_LENGTH_MAX,  //< Max UTF-8 text bytes.
  };
  /* Message type codes. */
  enum Type {
    kChat = 0,      //< Normal user chat message.
    kSystem,        //< Server-generated (join, leave, ban).
    kModeration,    //< Moderator action (mute, unban, notice).
    kWhisper,       //< Private message (one-to-one).
    kCommand,       //< Client command (emote, gift, raid, etc.).
    kBroadcast,     //< Broadcaster message (pinned/sub).
  };

  /* Zeroes the message into an empty state. */
  TMessage()
      : uid_(0), ts_(0), type_(kChat), room_(0), pad_(0), text_length_(0) {
    text_[0] = '\0';
  }

  /* Constructs a message with the given fields. */
  TMessage(IUD uid, TMD ts, IUA type, IUC room, const CHA* text,
           IUB length = (IUB)-1)
      : uid_(uid),
        ts_(ts),
        type_(type),
        room_(room),
        pad_(0),
        text_length_(0) {
    /* Compute the real length if not given. Use IUC for the running count
    so it can reach TextMax (512) without the IUB wrapping at 255. */
    IUC real = 0;
    if (length == (IUB)-1) {
      while (real < TextMax && text[real]) ++real;
    } else {
      real = length;
    }
    if (real > TextMax) real = TextMax;
    ::memcpy(text_, text, (size_t)real);
    text_[real] = '\0';
    text_length_ = real;
  }

  /* Copies the text from the given buffer.
  @return the actual length stored (truncated to TextMax). */
  IUC TextSet(const CHA* text, IUC length) {
    IUC real = 0;
    if (text == nullptr) return 0;
    if (length == (IUC)-1) {
      while (real < TextMax && text[real]) ++real;
    } else {
      real = length;
    }
    if (real > TextMax) real = TextMax;
    ::memcpy(text_, text, (size_t)real);
    text_[real] = '\0';
    text_length_ = real;
    return real;
  }

  IUD uid_;          //< Sender session uid.
  TMD ts_;           //< Timestamp (seconds since epoch).
  IUA type_;         //< Type code.
  IUC room_;         //< Room (channel) index.
  IUC pad_;          //< Alignment padding.
  IUC text_length_;  //< UTF-8 byte length of text_ (IUC: can hold 512).
  CHA text_[TextMax + 1];  //< UTF-8 text, NUL-terminated.
};                   //< struct TMessage

/* A ring buffer of TMessage.
Allocation-free after construction: pre-allocates a fixed capacity and wraps
around. When the buffer is full, the oldest message is overwritten.

Threading model: single writer (event thread), any number of readers (drain
loop). The reader uses a monotonic `read_cursor_` that never exceeds
`write_cursor_` (modulo capacity).
*/
struct AMessageRing {
  AMessageRing(IUC capacity = 1024) {
    /* Round up to power of 2 for fast modulo. */
    capacity_ = capacity < 16 ? 16 : 1;
    while (capacity_ < capacity) capacity_ <<= 1;
    mask_ = capacity_ - 1;
    origin_ = (TMessage*)::calloc((size_t)capacity_ * sizeof(TMessage), 1);
  }

  ~AMessageRing() {
    if (origin_) ::free(origin_);
  }

  AMessageRing(const AMessageRing&) = delete;
  AMessageRing& operator=(const AMessageRing&) = delete;

  /* Gets the ring capacity. */
  IUC Capacity() const { return capacity_; }

  /* Gets the number of messages currently in the ring. */
  IUC Count() const {
    IUC w = write_cursor_.load(::std::memory_order_acquire);
    IUC r = read_cursor_.load(::std::memory_order_acquire);
    return w >= r ? w - r : 0;
  }

  /* True if the ring is full (writer will overwrite oldest). */
  BOL Full() const {
    IUC w = write_cursor_.load(::std::memory_order_acquire);
    IUC r = read_cursor_.load(::std::memory_order_acquire);
    return (w - r) >= capacity_;
  }

  /* Writes a message into the ring (single writer only).
  @param msg Pointer to the message to copy.
  @return the write cursor position (slot index).
  @note The default Write copies the full struct (fast when text is large).
        Use WriteShort for the hot path where text is short — it skips the
        trailing zero bytes of the text_ buffer, saving ~500B of memcpy per
        message at 100K+ msg/s. */
  IUC Write(const TMessage* msg) {
    IUC w = write_cursor_.load(::std::memory_order_relaxed);
    IUC slot = w & mask_;
    origin_[slot] = *msg;
    write_cursor_.store(w + 1, ::std::memory_order_release);
    return slot;
  }

  /* Writes a message, copying only the used text bytes (hot path).
  The destination slot is assumed to have zeroed text_ beyond text_length_
  (true right after the calloc'd construction, and we keep it that way). */
  IUC WriteShort(const TMessage* msg) {
    IUC w = write_cursor_.load(::std::memory_order_relaxed);
    IUC slot = w & mask_;
    /* Copy the small leading header fields, then only the used text bytes.
    The dst slot's text_ tail may hold stale bytes from a previous longer
    message; if the new message is shorter, zero the gap so the slot stays
    NUL-terminated at text_length_ (cheap: a few bytes, not 512). */
    TMessage& dst = origin_[slot];
    dst.uid_ = msg->uid_;
    dst.ts_ = msg->ts_;
    dst.type_ = msg->type_;
    dst.room_ = msg->room_;
    dst.pad_ = msg->pad_;
    IUC new_len = msg->text_length_;
    IUC old_len = dst.text_length_;
    ::memcpy(dst.text_, msg->text_, (size_t)new_len + 1);
    if (old_len > new_len)
      ::memset(dst.text_ + new_len + 1, 0, (size_t)(old_len - new_len));
    dst.text_length_ = new_len;
    write_cursor_.store(w + 1, ::std::memory_order_release);
    return slot;
  }

  /* Reads the next unread message (single reader drain loop).
  @param msg Output: the message read.
  @return true if a message was read, false if the ring is empty. */
  BOL Read(TMessage* msg) {
    IUC r = read_cursor_.load(::std::memory_order_relaxed);
    IUC w = write_cursor_.load(::std::memory_order_acquire);
    if (r >= w) return false;  // empty.
    IUC slot = r & mask_;
    *msg = origin_[slot];
    read_cursor_.store(r + 1, ::std::memory_order_release);
    return true;
  }

  /* Peeks at the next unread message without consuming it. */
  BOL Peek(TMessage* msg) const {
    IUC r = read_cursor_.load(::std::memory_order_acquire);
    IUC w = write_cursor_.load(::std::memory_order_acquire);
    if (r >= w) return false;
    *msg = origin_[r & mask_];
    return true;
  }

  /* Clears the ring (resets cursors). */
  void Clear() {
    write_cursor_.store(0, ::std::memory_order_relaxed);
    read_cursor_.store(0, ::std::memory_order_relaxed);
  }

  /* Drops the oldest @c n messages (for back-pressure / skip logic). */
  void Drop(IUC n) {
    IUC r = read_cursor_.load(::std::memory_order_relaxed);
    IUC w = write_cursor_.load(::std::memory_order_acquire);
    IUC available = w - r;
    if (n > available) n = available;
    read_cursor_.store(r + n, ::std::memory_order_release);
  }

  TMessage* origin_;           //< Heap block of TMessage[capacity_].
  IUC capacity_;               //< Ring capacity (power of 2).
  IUC mask_;                   //< capacity_ - 1.
  ::std::atomic<IUC> write_cursor_;  //< Next write position (monotonic).
  ::std::atomic<IUC> read_cursor_;   //< Next read position (monotonic).
};                             //< struct AMessageRing

}  //< namespace _
#endif
#endif
