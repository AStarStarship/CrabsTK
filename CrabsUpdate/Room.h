// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TOOLKIT_UPDATE_ROOM_H
#define CRABS_TOOLKIT_UPDATE_ROOM_H

#include "ASCIITypes.h"
#include "AType.h"
#include "ContiguousStack.h"

namespace CT {

/*
A Chinese Room.

A Room is a stack machine. One Room talks to another by streaming
heterogeneous type-value tuples: each tuple is one 16-bit NeoASCII type
word (IUB: 12-bit type payload plus four MOD bits) followed by value
words. The receiving Room scans the stream one tuple at a time:

- The type payload is a Core POD type index (0-31) from the ASCII Data
  Specification Core POD Table. The value size comes from the table:
  0 bytes for NIL, 1 byte for IUA/ISA/CHA, 2 for FPB/IUB/ISB/CHB,
  4 for FPC/IUC/ISC/CHC, 8 for FPD/IUD/ISD/SSD, 16 for the 128-bit
  types, and context-dependent for PCa-PCl (0 in the plain table).
- Framing: each data tuple is the type word followed by ceil(size/2)
  value words. 1-byte types take 1 value word, 2-byte take 1, 4-byte
  take 2, 8-byte take 4. 16-byte types are rejected: they must be
  referenced by index.
- Payloads 0x20+ are the extended region: Plain Context types and
  escape-sequence references. A reference tuple names an already-loaded
  entry by its 12-bit index, so a short stream can reference big values
  without copying them.
- The Escape control word (0x1b) opens an escape sequence: the following
  Reference tuples are interpreted until the EscapeEnd (0x1e) control
  closes it.
- Machine controls that need values (Checkpoint, Undo, Rollback) are
  carried as their C0 control word in the type payload; the type payload
  of a data tuple is never a C0 code.

The List of Things begins as inline contiguous storage and grows into
one heap allocation, matching the CrabsMachine stack growth model. The
Room never throws: every operation returns a status and leaves the Room
valid.
*/

/* The Core POD Table from the ASCII Data Specification, payload 0-31.
   Sizes: bytes. PCa-PCl are context-dependent; 0 in the plain table. */
constexpr IUC PodTableSize(TypePayload payload) {
  if (payload > 31) return 0;
  switch (payload) {
    case 0: return 0;   // NIL
    case 1: case 2: case 3: return 1;   // IUA ISA CHA
    case 4: case 5: case 6: case 7: return 2;  // FPB IUB ISB CHB
    case 8: case 9: case 10: case 11: return 4;  // FPC IUC ISC CHC
    case 12: case 13: case 14: case 15: return 8;  // FPD IUD ISD SSD
    case 16: case 17: case 18: case 19: return 16;  // FPE IUE ISE SSE
    default: return 0;  // PCa-PCl: context-dependent.
  }
}

constexpr BOL IsPodPayload(TypePayload payload) { return payload <= 31; }

/* Machine controls. Carried as C0 control words in the type payload. */
enum class RoomControl : TypePayload {
  Checkpoint = 0x11,  // DC1: mark an undo checkpoint.
  Undo = 0x12,        // DC2: undo the last recorded mutation.
  Rollback = 0x13,    // DC3: rollback to a checkpoint (value = index).
  Escape = 0x1b,      // Open an escape sequence.
  EscapeEnd = 0x1e,   // RS: close an open escape sequence.
  Reference = 0x1f,   // US: name a loaded entry by 12-bit index.
};

constexpr BOL IsControlWord(IUB word) {
  // A control word has MOD=4 (Constant) and a C0 payload (0-31).
  return TypeModOf(word) == TypeMod::Constant &&
         TypePayloadOf(word) <= 0x1f;
}

/* One entry in the List of Things: an ASCII Data Type with its value.
   Values up to 8 bytes ride inline; the machine can address two ALU
   words at a time, so anything bigger must be referenced by index. */
struct RoomThing {
  IUB type;             //< 16-bit NeoASCII type word.
  IUB index;            //< 12-bit index of the entry.
  IUB size;             //< Value size in bytes.
  IUC lsb;              //< First 32 bits of the value.
  IUC msb;              //< Next 32 bits of the value.
};

/* The Chinese Room: the machine that receives and holds the List of
   Things. All storage is contiguous. No exceptions: every operation
   returns a status and leaves the Room valid on failure. */
class Room {
  enum { ThingsMin = 16 };

 public:
  /* The result of feeding one tuple to the Room. */
  enum class FeedResult {
    Stored,        //< Data tuple appended to the List of Things.
    Control,       //< C0 control interpreted.
    Referenced,    //< Escape-sequence reference consumed.
    Invalid,       //< Malformed tuple; the Room is unchanged.
    Full,          //< List of Things is full and could not grow.
  };

  /* Feeds a flat word stream. Each tuple is the type word followed by
     StreamWordsOf(type) value words. Returns the number of tuples
     consumed before the first failure, or the stream size on success. */
  IUC Feed(const IUB* stream, IUC words) {
    IUC index = 0;
    while (index < words) {
      IUC value_words = 0;
      FeedResult result = FeedTuple(stream[index], stream + index + 1,
                                    words - index - 1, value_words);
      if (result == FeedResult::Invalid || result == FeedResult::Full)
        return index;
      index += value_words + 1;
    }
    return index;
  }

  /* Feeds one tuple. value points to StreamWordsOf(type) value words.
     value_words is set to the number of value words consumed. A type
     payload that is a C0 code is a machine control. While inside an
     escape sequence, only escape controls are legal. */
  FeedResult FeedTuple(IUB type, const IUB* value,
                       IUC value_count, IUC& value_words) {
    value_words = 0;
    if (IsControlWord(type))
      return ControlTuple(type, value, value_count, value_words);
    if (escape_) return EscapeTuple(type, value, value_count, value_words);
    FeedResult result = Store(type, value, value_count);
    if (result == FeedResult::Stored) value_words = StreamWordsOf(type);
    return result;
  }

  /* Appends one data tuple to the List of Things. The type payload must
     be a Core POD type (0-31) with a size of 8 bytes or less. value
     points to StreamWordsOf(type) value words. */
  FeedResult Store(IUB type, const IUB* value, IUC value_count = 0) {
    TypePayload payload = TypePayloadOf(type);
    if (!IsPodPayload(payload)) return FeedResult::Invalid;
    IUC size = PodTableSize(payload);
    if (size > 8) return FeedResult::Invalid;
    IUC words = StreamWordsOf(type);
    if (value && value_count < words) return FeedResult::Invalid;

    IUA raw[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    if (value && size > 0) {
      for (IUC word = 0; word < words; ++word) {
        const IUA* bytes =
            reinterpret_cast<const IUA*>(&value[word]);
        raw[word * 2] = bytes[0];
        raw[word * 2 + 1] = bytes[1];
      }
    }
    if (things_.count() >= things_.total() &&
        !things_.Reserve(things_.total() * 2))
      return FeedResult::Full;

    RoomThing thing{};
    thing.type = type;
    thing.index = TypePayload(static_cast<IUB>(things_.count()));
    thing.size = static_cast<IUB>(size);
    thing.lsb = IUC(raw[0]) | (IUC(raw[1]) << 8) |
                (IUC(raw[2]) << 16) | (IUC(raw[3]) << 24);
    thing.msb = (size >= 8)
                    ? (IUC(raw[4]) | (IUC(raw[5]) << 8) |
                       (IUC(raw[6]) << 16) | (IUC(raw[7]) << 24))
                    : 0;
    things_.Push(thing);
    last_index_ = thing.index;
    return FeedResult::Stored;
  }

  /* Gets the loaded entry by its 12-bit index. */
  const RoomThing* Get(TypePayload index) const {
    if (index >= TypePayload(things_.count())) return nullptr;
    return things_.Begin() + index;
  }

  /* Gets the value of the entry at index as two words. */
  BOL ValueAt(TypePayload index, IUC* lsb, IUC* msb) const {
    const RoomThing* thing = Get(index);
    if (!thing) return false;
    if (lsb) *lsb = thing->lsb;
    if (msb) *msb = thing->msb;
    return true;
  }

  /* The number of entries in the List of Things. */
  IUC Count() const { return things_.count(); }

  /* The index of the most recently stored entry. */
  TypePayload LastIndex() const { return last_index_; }

  /* Marks an undo checkpoint (the current List count). */
  IUC Checkpoint() const { return things_.count(); }

  /* Pops to a checkpoint: every entry stored since is rolled back. */
  BOL Rollback(IUC checkpoint) { return things_.Rollback(checkpoint); }

  /* True while an escape sequence is open. */
  BOL InEscape() const { return escape_; }

  /* Number of entries named through escape sequences this session. */
  IUC ReferencesCount() const { return references_; }

 private:
  /* Number of stream value words a data type consumes. 1-byte PODs
     take 1 value word (the low 8 bits of the first word). */
  static IUC StreamWordsOf(IUB type) {
    IUC size = PodTableSize(TypePayloadOf(type));
    if (size < 2) return size ? 1 : 0;
    return size / 2;
  }

  /* Interprets a C0 control word (payload 0-31). */
  FeedResult ControlTuple(IUB type, const IUB* value,
                          IUC value_count, IUC& value_words) {
    TypePayload payload = TypePayloadOf(type);
    if (payload == TypePayload(RoomControl::Checkpoint)) {
      last_checkpoint_ = Checkpoint();
      value_words = 0;
      return FeedResult::Control;
    }
    if (payload == TypePayload(RoomControl::Undo)) {
      if (!things_.Pop()) return FeedResult::Invalid;
      value_words = 0;
      return FeedResult::Control;
    }
    if (payload == TypePayload(RoomControl::Rollback)) {
      if (value_count < 1) return FeedResult::Invalid;
      if (!things_.Rollback(value[0])) return FeedResult::Invalid;
      value_words = 1;
      return FeedResult::Control;
    }
    if (payload == TypePayload(RoomControl::Escape)) {
      if (escape_) return FeedResult::Invalid;  // No nested escapes.
      escape_ = true;
      value_words = 0;
      return FeedResult::Control;
    }
    if (payload == TypePayload(RoomControl::EscapeEnd)) {
      if (!escape_) return FeedResult::Invalid;
      escape_ = false;
      value_words = 0;
      return FeedResult::Control;
    }
    if (payload == TypePayload(RoomControl::Reference)) {
      if (!escape_ || value_count < 1) return FeedResult::Invalid;
      if (!Get(static_cast<TypePayload>(value[0]))) return FeedResult::Invalid;
      references_++;
      value_words = 1;
      return FeedResult::Referenced;
    }
    // Any other C0 code (NUL, SOH, ...): no-op.
    value_words = 0;
    return FeedResult::Control;
  }

  /* Interprets a tuple while inside an escape sequence. Only escape
     controls and reference tuples are legal there. */
  FeedResult EscapeTuple(IUB type, const IUB* value,
                         IUC value_count, IUC& value_words) {
    TypePayload payload = TypePayloadOf(type);
    if (payload == TypePayload(RoomControl::EscapeEnd)) {
      escape_ = false;
      value_words = 0;
      return FeedResult::Control;
    }
    if (payload == TypePayload(RoomControl::Reference)) {
      if (value_count < 1 || !Get(static_cast<TypePayload>(value[0])))
        return FeedResult::Invalid;
      references_++;
      value_words = 1;
      return FeedResult::Referenced;
    }
    // Data tuples are illegal inside an escape sequence.
    return FeedResult::Invalid;
  }

  ContiguousStack<RoomThing, ThingsMin> things_;
  TypePayload last_index_ = 0;
  IUC last_checkpoint_ = 0;
  BOL escape_ = false;
  IUC references_ = 0;
};

}  // namespace CT
#endif
