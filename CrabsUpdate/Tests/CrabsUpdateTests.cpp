// Copyright AStarship <https://astarship.net>.

#include "../_Package.hxx"
#include "../Room.h"
#include "../Wcb.h"

#include <cassert>
#include <cstdint>
#include <cstring>

using namespace CT;

namespace {

void TestTypes() {
  constexpr TypeWord unsigned_char = TypePack('A', TypeMod::Unsigned);
  constexpr TypeWord signed_int = TypePack(0x123, TypeMod::Signed);
  static_assert(TypePayloadOf(unsigned_char) == 'A');
  static_assert(TypeModOf(unsigned_char) == TypeMod::Unsigned);
  static_assert(TypePayloadOf(signed_int) == 0x123);
  static_assert(TypeModOf(signed_int) == TypeMod::Signed);
  static_assert(TypeIsSigned(signed_int));
  static_assert(ControlType(Control::Escape) == 0x1b);
}

void TestContiguousStack() {
  ContiguousStack<IUC, 2> stack;
  assert(stack.Push(10));
  auto checkpoint = stack.Mark();
  assert(stack.Push(20));
  assert(stack.Push(30));
  assert(stack.is_dynamic());
  assert(stack.count() == 3);
  assert(*stack.Peek() == 30);
  assert(stack.Rollback(checkpoint));
  assert(stack.count() == 1);
  assert(*stack.Peek() == 10);
  IUC item = 0;
  assert(stack.Pop(item));
  assert(item == 10);
  assert(!stack.Pop());
}

void TestUndoStack() {
  UndoStack<2> undo;
  IUC channel = 1;
  IUD route = 100;
  auto checkpoint = undo.Mark();
  assert(undo.Assign(channel, IUC(16)));
  assert(undo.Assign(route, IUD(9001)));
  assert(channel == 16);
  assert(route == 9001);
  assert(undo.Rollback(checkpoint));
  assert(channel == 1);
  assert(route == 100);
  assert(undo.count() == 0);
}

void TestRoom() {
  Room room;
  // A 16-bit NeoASCII stream: [type, value...] tuples.
  // Framing: each data tuple is the type word followed by value words.
  // 1-byte types take 1 value word, 2-byte take 1, 4-byte take 2,
  // 8-byte take 4. Core POD Table: 3=CHA 6=ISB 9=IUC 13=ISD 10=ISC.
  const IUB stream[] = {
      TypePack(3, TypeMod::Unsigned), 0x41,        // CHA 'A'
      TypePack(6, TypeMod::Signed), 0x7fff,        // ISB 32767
      TypePack(9, TypeMod::Unsigned), 0x1234, 0x5678,  // IUC
      TypePack(13, TypeMod::Signed), 0x1111, 0x2222, 0x3333, 0x4444,  // ISD
      TypePack(0x1b, TypeMod::Constant),           // ESC open (C0 + MOD=4)
      TypePack(0x1f, TypeMod::Constant), 1,        // reference entry #1
      TypePack(0x1e, TypeMod::Constant),           // ESC END
      TypePack(10, TypeMod::Context), 0x2a2b, 0x2c2d,  // ISC
  };
  IUC consumed = room.Feed(stream, 19);
  assert(consumed == 19);
  assert(room.Count() == 5);
  assert(room.LastIndex() == 4);

  IUC lsb = 0, msb = 0;
  assert(room.ValueAt(0, &lsb, &msb));
  assert(lsb == 0x41);
  assert(room.ValueAt(1, &lsb, &msb));
  assert(lsb == 0x7fff);
  assert(room.ValueAt(2, &lsb, &msb));
  assert(lsb == 0x56781234 && msb == 0);
  assert(room.ValueAt(3, &lsb, &msb));
  assert(lsb == 0x22221111 && msb == 0x44443333);
  assert(room.ValueAt(4, &lsb, &msb));
  assert(lsb == 0x2c2d2a2b && msb == 0);

  // Escape references counted; nothing stored by controls.
  assert(room.ReferencesCount() == 1);
  assert(!room.InEscape());

  // MOD bits preserved on the loaded entries.
  assert(TypeModOf(room.Get(0)->type) == TypeMod::Unsigned);
  assert(TypeModOf(room.Get(1)->type) == TypeMod::Signed);
  assert(TypeModOf(room.Get(4)->type) == TypeMod::Context);

  // Undo pops the last entry; rollback restores a checkpoint.
  IUC checkpoint = room.Checkpoint();
  IUB value = 0x2b;
  assert(room.Store(TypePack(1, TypeMod::Unsigned), &value, 1) ==
         Room::FeedResult::Stored);
  assert(room.Count() == 6);
  assert(room.Rollback(checkpoint));
  assert(room.Count() == 5);
  assert(room.Get(5) == nullptr);

  // Controls in the text stream: Checkpoint, Escape, Reference, EscapeEnd.
  // Data inside an escape sequence is malformed.
  {
    Room control_room;
    IUB value = 0x42;
    assert(control_room.Store(TypePack(3, TypeMod::Unsigned), &value, 1) ==
           Room::FeedResult::Stored);
    IUB words[] = {
        TypePack(0x11, TypeMod::Constant),  // DC1 Checkpoint (C0 + MOD=4)
        TypePack(0x1b, TypeMod::Constant),  // ESC open
        TypePack(0x1f, TypeMod::Constant), 0,  // reference entry #0
        TypePack(3, TypeMod::Unsigned), 0x43,  // data in escape = invalid
    };
    assert(control_room.Feed(words, 6) == 4);
    assert(control_room.Count() == 1);
    assert(control_room.ReferencesCount() == 1);
  }
}

/* ==========================================================================
 * Write-Combining Buffer Cache (WCB) Tests
 * ========================================================================== */

void TestWcbInit() {
  IUB store[1024] = {0};
  Wcb wcb;
  assert(wcb.Init(store, sizeof(store)));
  assert(wcb.Store() != nullptr);
  assert(wcb.Size() == sizeof(store));
  assert(wcb.IsEmpty());
  assert(wcb.Count() == 0);

  // Null/zero init should fail.
  Wcb bad;
  assert(!bad.Init(nullptr, 0));
  assert(!bad.Init(store, 0));
}

void TestWcbWriteRead() {
  IUB store[1024] = {0};
  Wcb wcb;
  wcb.Init(store, sizeof(store));

  // Write a value to store[1].
  IUB data = 0xABCD;
  assert(wcb.Write(&store[1], &data, sizeof(data)));
  assert(wcb.Count() == 1);
  assert(!wcb.IsEmpty());

  // Read directly from store (bypasses WCB — store[1] is still zero).
  IUB readback = 0;
  assert(wcb.Read(&readback, &store[1], sizeof(readback)));
  assert(readback == 0);  // store[1] hasn't been flushed yet.

  // Flush — now the store should reflect the write.
  wcb.Flush();
  assert(wcb.IsEmpty());
  assert(wcb.Count() == 0);

  // Verify the write landed at store[1].
  IUB verify = 0;
  assert(wcb.Read(&verify, &store[1], sizeof(verify)));
  assert(verify == 0xABCD);
}

void TestWcbCoalesce() {
  IUB store[1024] = {0};
  Wcb wcb;
  wcb.Init(store, sizeof(store));

  // Write same address twice — should coalesce to one entry.
  IUB v1 = 0x1111;
  IUB v2 = 0x2222;
  assert(wcb.Write(&store[3], &v1, sizeof(v1)));
  assert(wcb.Count() == 1);
  assert(wcb.Write(&store[3], &v2, sizeof(v2)));
  assert(wcb.Count() == 1);  // still one entry.

  wcb.Flush();
  IUB result = 0;
  assert(wcb.Read(&result, &store[3], sizeof(result)));
  assert(result == 0x2222);  // last write wins.
}

void TestWcbMultipleEntries() {
  IUB store[1024] = {0};
  Wcb wcb;
  wcb.Init(store, sizeof(store));

  // Write to three different offsets.
  IUB v1 = 0x0101;
  IUB v2 = 0x0202;
  IUB v3 = 0x0303;
  assert(wcb.Write(&store[1], &v1, sizeof(v1)));
  assert(wcb.Write(&store[7], &v2, sizeof(v2)));
  assert(wcb.Write(&store[15], &v3, sizeof(v3)));
  assert(wcb.Count() == 3);

  wcb.Flush();

  IUB r1 = 0, r2 = 0, r3 = 0;
  assert(wcb.Read(&r1, &store[1], sizeof(r1)));
  assert(wcb.Read(&r2, &store[7], sizeof(r2)));
  assert(wcb.Read(&r3, &store[15], sizeof(r3)));
  assert(r1 == 0x0101);
  assert(r2 == 0x0202);
  assert(r3 == 0x0303);
}

void TestWcbFull() {
  IUB store[1024] = {0};
  Wcb wcb;
  wcb.Init(store, sizeof(store));

  // Fill the buffer to WcbMaxEntries (64).
  for (IUC i = 0; i < WcbMaxEntries; ++i) {
    IUB v = static_cast<IUB>(i);
    assert(wcb.Write(&store[i], &v, sizeof(v)));
  }
  assert(wcb.IsFull());
  assert(wcb.Count() == WcbMaxEntries);

  // One more write should fail.
  IUB extra = 0xFFFF;
  assert(!wcb.Write(&store[WcbMaxEntries], &extra, sizeof(extra)));
}

void TestWcbClear() {
  IUB store[1024] = {0};
  Wcb wcb;
  wcb.Init(store, sizeof(store));

  for (IUC i = 0; i < 5; ++i) {
    IUB v = static_cast<IUB>(i);
    wcb.Write(&store[i], &v, sizeof(v));
  }
  assert(wcb.Count() == 5);

  wcb.Clear();
  assert(wcb.IsEmpty());
  assert(wcb.Count() == 0);

  // Data should NOT have been flushed.
  IUB r = 0;
  assert(wcb.Read(&r, &store[0], sizeof(r)));
  assert(r == 0);
}

void TestWcbBytes() {
  IUB store[1024] = {0};
  Wcb wcb;
  wcb.Init(store, sizeof(store));

  // Write 1 byte.
  IUA byte = 0x42;
  assert(wcb.Write(&store[10], &byte, 1));
  wcb.Flush();
  IUA rbyte = 0;
  assert(wcb.Read(&rbyte, &store[10], 1));
  assert(rbyte == 0x42);

  // Write 2 bytes.
  IUB word = 0x4242;
  assert(wcb.Write(&store[20], &word, 2));
  wcb.Flush();
  IUB rword = 0;
  assert(wcb.Read(&rword, &store[20], 2));
  assert(rword == 0x4242);

  // Write 4 bytes.
  IUC dword = 0xDEADBEEF;
  assert(wcb.Write(&store[30], &dword, 4));
  wcb.Flush();
  IUC rdword = 0;
  assert(wcb.Read(&rdword, &store[30], 4));
  assert(rdword == 0xDEADBEEF);

  // Write 8 bytes.
  IUD qword = 0x0102030405060708ULL;
  assert(wcb.Write(&store[40], &qword, 8));
  wcb.Flush();
  IUD rqword = 0;
  assert(wcb.Read(&rqword, &store[40], 8));
  assert(rqword == 0x0102030405060708ULL);
}

void TestWcbInvalidWrites() {
  IUB store[1024] = {0};
  Wcb wcb;
  wcb.Init(store, sizeof(store));

  // Null address.
  IUB v = 0;
  assert(!wcb.Write(nullptr, &v, sizeof(v)));

  // Null data.
  assert(!wcb.Write(&store[0], nullptr, sizeof(v)));

  // Zero bytes.
  assert(!wcb.Write(&store[0], &v, 0));

  // Too many bytes (> 8).
  assert(!wcb.Write(&store[0], &v, 9));

  // Address outside backing store.
  IUB outside = 0;
  assert(!wcb.Write(&outside, &v, sizeof(v)));
}

}  // namespace

int main() {
  TestTypes();
  TestContiguousStack();
  TestUndoStack();
  TestRoom();
  TestWcbInit();
  TestWcbWriteRead();
  TestWcbCoalesce();
  TestWcbMultipleEntries();
  TestWcbFull();
  TestWcbClear();
  TestWcbBytes();
  TestWcbInvalidWrites();
  puts("CrabsUpdate: 10 test groups passed");
  return 0;
}
