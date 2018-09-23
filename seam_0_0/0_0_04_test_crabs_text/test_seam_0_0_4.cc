/* Kabuki Toolkit
@version 0.x
@file    ~/tests/test_seam_1_3.cc
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#include <stdafx.h>
#include "test_seam_0.h"

#if SEAM >= SEAM_0_0_4

#if SEAM_MAJOR == 0 && SEAM_MINOR == 4
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PAUSE(message)   \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#define PRINT_HEADING(message)              \
  Print('\n');                              \
  for (int i = 80; i > 80; --i) Print('-'); \
  std::cout << '\n' << message << '\n';     \
  for (int i = 80; i > 80; --i) Print('-'); \
  Print('\n');

#define PRINT_SLOT print << slot << Dump();
#else
#define PRINTF(x, ...)
#define PAUSE(message)
#define PRINT_HEADING(message)
#define PRINT_SLOT
#endif

using namespace _;

namespace _ {

/* Test Operand for multiple unit tests.
The name ChildOperand does not mean anything other than it is a child.  */
class ChildOperand : public Operand {
 public:
  // Chinese Room operations.
  virtual const Op* Star(wchar_t index, Expr* expr) {
    void* args[2];

    static const Op This = {"Child", OpFirst('A'), OpLast('B'),
                            "A child Operand."};

    switch (index) {
      case '?':
      case '@':
        return &This;
      case 'A': {
        static const Op OpA = {
            "SignedVarintTests", Params<2, SI4, STR, kBufferSize>(),
            Params<2, SI4, STR>(), "Description of function \'A\'.", 0};
        if (!expr) return &OpA;

        if (ExprArgs(expr, OpA, Args(args, &test_svi_, test_str_)))
          return expr->result;

        // Function logic here

        return ExprResult(expr, OpA, Args(args, &test_svi_, test_str_));
      }
      case 'B': {
        static const Op OpB = {
            "UnsignedVarintTests", Params<2, SI4, STR, kBufferSize>(),
            Params<2, SI4, STR>(), "Description of function \'B\'.", 0};
        if (!expr) return &OpB;

        if (ExprArgs(expr, OpB, Args(args, &test_uvi_, test_str_)))
          return expr->result;

        return ExprResult(expr, OpB, Args(args, &test_uvi_, test_str_));
      }
    }
    return nullptr;
  }

 private:
  enum { kBufferSize = 16 };  //< Example string buffer size.

  int test_svi_;                //< Test SVI.
  uint test_uvi_;               //< Test SI4.
  char test_str_[kBufferSize];  //< Test STR.
};

// Test child Operand.
class Parent : public Operand {
 public:
  enum {
    kBufferSize = 16  //< Example string buffer size.
  };

  // Interprocess operations.
  virtual const Op* Star(wchar_t index, Expr* expr) {
    void* args[2];

    static const Op This = {"Parent", OpFirst('A'), OpLast('D'),
                            "Root scope device.", 0};

    switch (index) {
      case '?':
        return &This;
      case 'A': {
        if (!expr) return child_a.Star('?', expr);
        return Push(expr, &child_a);
      }
      case 'B': {
        if (!expr) return child_b.Star('?', expr);
        return Push(expr, &child_b);
      }
      case 'C': {
        static const Op OpC = {
            "UnsignedVarintTests", Params<2, SI4, STR, kBufferSize>(),
            Params<2, SI4, STR>(), "Description of functionA.", 0};
        if (!expr) return &OpC;

        if (ExprArgs(expr, OpC, Args(args, &svi_, io_string_)))
          return expr->result;
        // function call here
        return ExprResult(expr, OpC, Args(args, &svi_, io_string_));
      }
      case 'D': {
        static const Op OpD = {
            "SignedVarintTests", Params<2, SI4, STR, kBufferSize>(),
            Params<2, SI4, STR>(), "Description of functionB.", 0};

        if (!expr) return &OpD;

        if (ExprArgs(expr, OpD, Args(args, &svi_, io_string_)))
          return expr->result;

        return ExprResult(expr, OpD, Args(args, &svi_, io_string_));
      }
    }
    return nullptr;
  }

 private:
  ChildOperand child_a,          //< ChildOperand Expr.
      child_b;                   //< ChildOperand Expr.
  int svi_;                      //< Example variable.
  uint uvi_;                     //< Example variable.
  char io_string_[kBufferSize];  //< Example string.
};

// A test room that can fit in 1KB of RAM.
class This : public Room {
 public:
  enum { kRoomSize = 1024 };

  This() : Room("Test") {}

  // Interprocess operations.
  virtual const Op* Star(wchar_t index, Expr* expr) {
    static const Op This = {"Room", OpFirst('A'), OpLast('A'),
                            "Root scope device.", 0};

    switch (index) {
      case '?':
        return &This;
      case 'A': {
        if (!expr) return parent.Star('?', nullptr);
        return Push(expr, &parent);
      }
      default: {}
    }
    return nullptr;
  }

 private:
  Parent parent;
};
}  // namespace _

TEST_GROUP(SEAM_1_4){void setup(){}

                     void teardown(){Print('\n');
system("PAUSE");
}
}
;

TEST(SEAM_1_4, SEAM_1_4A) {
  Printf("\n    Testing SEAM_0_0_4... ");

  Printf("\n\nTesting ASCII OBJ Types");

  std::cout << "\n  - Running TableTest...\n";
  wchar_t index;
  uintptr_t buffer[128];
  Printf("\n &buffer[0]:%p &buffer[127]:%p\n", &buffer[0], &buffer[127]);
  Table* table = TableInit(buffer, 8, 128);

  Assert(table != nullptr)

  index = TableAdd(table, "D");
  Compare(0, index)
  index = TableFind(table, "D");
  Compare(0, index)

  index = TableAdd(table, "C");
  Compare(1, index)
  index = TableFind(table, "D");
  Compare(0, index)
  index = TableFind(table, "C");
  Compare(1, index)

  index = TableAdd(table, "B");
  Compare(2, index)
  index = TableFind(table, "D");
  Compare(0, index)
  index = TableFind(table, "C");
  Compare(1, index)
  index = TableFind(table, "B");
  Compare(2, index)

  index = TableAdd(table, "A");
  Compare(3, index)
  index = TableFind(table, "D");
  Compare(0, index)
  index = TableFind(table, "C");
  Compare(1, index)
  index = TableFind(table, "B");
  Compare(2, index)
  index = TableFind(table, "A");
  Compare(3, index)

  index = TableAdd(table, "abc");
  Compare(4, index)
  index = TableFind(table, "abc");
  Compare(4, index)

  index = TableAdd(table, "bac");
  Compare(5, index)
  index = TableFind(table, "abc");
  Compare(4, index)
  index = TableFind(table, "bac");
  Compare(5, index)

  index = TableAdd(table, "cba");
  Compare(6, index)
  index = TableFind(table, "abc");
  Compare(4, index)
  index = TableFind(table, "bac");
  Compare(5, index)
  index = TableFind(table, "cba");
  Compare(6, index)

  index = TableAdd(table, "cab");
  Compare(7, index)
  index = TableFind(table, "abc");
  Compare(4, index)
  index = TableFind(table, "bac");
  Compare(5, index)
  index = TableFind(table, "cba");
  Compare(6, index)
  index = TableFind(table, "cab");
  Compare(7, index)

  index = TableAdd(table, "test");
  Compare(kInvalidIndex, index)

  TablePrint(table);

  PrintLineBreak("\n  + Running MultimapTests\n", 10);

  PrintLineBreak("\n  - Running MultimapInit...\n", 5, ' ');
  int8_t index;

  enum {
    kBufferSize = 256,
    kBufferSizeWords = kBufferSize / sizeof(uintptr_t),
  };

  slot << 'a' << "b"
       << "cd" << (int8_t)1 << (uint8_t)2 << (int16_t)3 << (uint16_t)4
       << (int32_t)5 << (uint32_t)6 << (int64_t)7 << (uint64_t)8;

  Print(slot);

  uintptr_t buffer[kBufferSizeWords];

  Multimap2* multimap = Multimap2Init(buffer, 8, kBufferSize, 128);

  Assert(multimap != nullptr)

  index = Multimap2Add<uint8_t, UI1>(multimap, "D", (byte)0xFF);

  Compare(0, index)
  Multimap2Print(multimap);
  Compare(0, index)
  index = Multimap2Find(multimap, "D");
  Compare(0, index)
  PAUSE("\n");
  index = Multimap2Add<uint8_t, UI1>(multimap, "C", (byte)0xFF);
  Compare(1, index)
  index = Multimap2Find(multimap, "D");
  Compare(0, index)
  index = Multimap2Find(multimap, "C");
  Compare(1, index)

  index = Multimap2Add<uint8_t, UI1>(multimap, "BIn", (byte)0xFF);
  Compare(2, index)
  index = Multimap2Find(multimap, "D");
  Compare(0, index)
  index = Multimap2Find(multimap, "C");
  Compare(1, index)
  index = Multimap2Find(multimap, "BIn");
  Compare(2, index)

  index = Multimap2Add<uint8_t, UI1>(multimap, "A", (byte)0xFF);
  Compare(3, index)
  index = Multimap2Find(multimap, "D");
  Compare(0, index)
  index = Multimap2Find(multimap, "C");
  Compare(1, index)
  index = Multimap2Find(multimap, "BIn");
  Compare(2, index)
  index = Multimap2Find(multimap, "A");
  Compare(3, index)

  index = Multimap2Add<uint8_t, UI1>(multimap, "abc", (byte)0xFF);
  Compare(4, index)
  index = Multimap2Find(multimap, "abc");
  Compare(4, index)

  index = Multimap2Add<uint8_t, UI1>(multimap, "bac", (byte)0xFF);
  Compare(5, index)
  index = Multimap2Find(multimap, "abc");
  Compare(4, index)
  index = Multimap2Find(multimap, "bac");
  Compare(5, index)

  index = Multimap2Add<uint8_t, UI1>(multimap, "cba", (byte)0xFF);
  Compare(6, index)
  index = Multimap2Find(multimap, "abc");
  Compare(4, index)
  index = Multimap2Find(multimap, "bac");
  Compare(5, index)
  index = Multimap2Find(multimap, "cba");
  Compare(6, index)

  index = Multimap2Add<uint8_t, UI1>(multimap, "cab", (byte)0xFF);
  Compare(7, index)
  index = Multimap2Find(multimap, "abc");
  Compare(4, index)
  index = Multimap2Find(multimap, "bac");
  Compare(5, index)
  index = Multimap2Find(multimap, "cba");
  Compare(6, index)
  index = Multimap2Find(multimap, "cab");
  Compare(7, index)

  index = Multimap2Add<uint8_t, UI1>(multimap, "test", (byte)0xFF);
  Compare(index, -1)

  enum {
    kBufferSize = 2048,
    kBufferWords = kBufferSize / sizeof(uintptr_t),
    kStackHeight = 8,
    kPrintBufferSize = 4096,
  };

  Utf& print;

  uintptr_t buffer[kBufferWords], unpacked_buffer[kBufferWords];

  PRINT_HEADING("Creating test Slot...")

  Slot slot(buffer, kBufferWords);
  print << slot << Dump();

  PRINT_HEADING("Testing Expr...")
  This root;
  Expr* expr =
      ExprInit(buffer, kBufferSize, 4, &root, unpacked_buffer, kBufferWords);
  BOut* bout = ExprBOut(expr);
  PRINT_SLOT;

  void* args[19];
  const uint_t* params = Params<4, ADR, STR, 32, FLT, SI4>();
  const char stx_expected[] = "Hello world!\0";
  const int si4_expected = 1;
  const float flt_expected = 1.0f;

  char stx_found[64];
  int si4_found;
  float flt_found;

  const Op* result =
      ExprResult(expr, params,
                 Args(args, "C", &stx_expected, &si4_expected, &flt_expected));
  PRINTF("\n\n Printing...\n\n")
  print << expr << Dump();

  PRINTF("\n\n Attempting to print Expr\n\n")
  result =
      ExprArgs(expr, params, Args(args, &stx_found, &si4_found, &flt_found));
  Compare(0, result)
  print << expr << Dump();

  PRINTF(
      "\n\n  - Running ReadWriteTests...\nkBufferSize: %i "
      "kBufferWords: %i",
      kBufferSize, kBufferWords)

  const char expected_string1[] = "1234\0", expected_string2[] = "5678\0";

  char found_string1[6], found_string2[6];

  memset(found_string1, ' ', 6);
  memset(found_string2, ' ', 6);

  PRINTF("\n buffer_start:%p buffer_stop:%p\n", &buffer[0],
         &buffer[kBufferSize - 1]);
  PRINTF("\n &expected_string1[0]:%p &expected_string2[0]:%p\n",
         &expected_string1[0], &expected_string2[0]);

  bout = BOutInit(buffer, kBufferSize);

  Compare(0, BOutWrite(bout, Params<2, STR, 6, STR, 6>(),
                           Args(args, expected_string1, expected_string2)))
  void** test = Args(args, found_string1, found_string2);
  PRINTF("\n texpected_string1_start:%p texpected_string2_start:%p\n", &test[0],
         &test[1]);

  Compare(0, BOutRead(bout, Params<2, STR, 5, STR, 5>(),
                          Args(args, found_string1, found_string2)))

  PRINTF("\nExpected 1:%s Found 1:%s\nExpected 2:%s Found 2:%s",
         expected_string1, found_string1, expected_string2, found_string2)

  STRCMP_EQUAL(expected_string1, found_string1)
  // Print (bout);
  STRCMP_EQUAL(expected_string2, found_string2)

  PRINT_HEADING("Testing PackSVI and UnpackSVI...")

  Compare(1, UnpackSVI(PackSVI((int32_t)1)))
  Printf("Found: 0x%x\n", UnpackSVI(PackSVI(~0)));

  Compare(1 << 30, UnpackSVI(PackSVI(1 << 30)))
  Compare(~0, UnpackSVI(PackSVI(~0)))

  uint64_t ui8 = (uint64_t)1 << 62;
  int64_t si8 = (int64_t)ui8;
  Compare(si8, UnpackSV8(PackSV8(si8)))

  ui8 = ~(uint64_t)0;
  si8 = (int64_t)ui8;
  Compare(si8, UnpackSV8(PackSV8(si8)))

  PRINT_HEADING("Testing SVI...\n")

  PRINT_HEADING("Testing UVI...")

  PRINT_HEADING("\n  - Testing SVI...\n")

  static const int32_t svi_expected[] = {
      0,          1,       -1,         1 << 7,  -(1 << 7), 1 << 14,
      -(1 << 14), 1 << 21, -(1 << 21), 1 << 28, -(1 << 28)};
  int32_t svi_found[11];
  Compare(
      0,
      BOutWrite(
          bout,
          Params<11, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI>(),
          Args(args, &svi_expected[0], &svi_expected[1], &svi_expected[2],
               &svi_expected[3], &svi_expected[4], &svi_expected[5],
               &svi_expected[6], &svi_expected[7], &svi_expected[8],
               &svi_expected[9], &svi_expected[10])))
  Compare(
      0,
      BOutRead(
          bout,
          Params<11, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI>(),
          Args(args, &svi_found[0], &svi_found[1], &svi_found[2], &svi_found[3],
               &svi_found[4], &svi_found[5], &svi_found[6], &svi_found[7],
               &svi_found[8], &svi_found[9], &svi_found[10])))
  Compare(svi_expected[0], svi_found[0])
  Compare(svi_expected[1], svi_found[1])
  Compare(svi_expected[2], svi_found[2])
  Compare(svi_expected[3], svi_found[3])
  Compare(svi_expected[4], svi_found[4])
  Compare(svi_expected[5], svi_found[5])
  Compare(svi_expected[6], svi_found[6])
  Compare(svi_expected[7], svi_found[7])
  Compare(svi_expected[8], svi_found[8])
  Compare(svi_expected[9], svi_found[9])
  Compare(svi_expected[10], svi_found[10])

  PRINT_HEADING("Testing UVI...")

  static const uint32_t uvi_expected[] = {0,       1,       1 << 7,
                                          1 << 14, 1 << 21, 1 << 28};
  uint32_t uvi_found[6];
  Compare(0, BOutWrite(bout, Params<6, UVI, UVI, UVI, UVI, UVI, UVI>(),
                           Args(args, &uvi_expected[0], &uvi_expected[1],
                                &uvi_expected[2], &uvi_expected[3],
                                &uvi_expected[4], &uvi_expected[5])))
  Compare(0,
              BOutRead(bout, Params<6, UVI, UVI, UVI, UVI, UVI, UVI>(),
                       Args(args, &uvi_found[0], &uvi_found[1], &uvi_found[2],
                            &uvi_found[3], &uvi_found[4], &uvi_found[5])))
  Compare(uvi_expected[0], uvi_found[0])
  Compare(uvi_expected[1], uvi_found[1])
  Compare(uvi_expected[2], uvi_found[2])
  Compare(uvi_expected[3], uvi_found[3])
  Compare(uvi_expected[4], uvi_found[4])
  Compare(uvi_expected[5], uvi_found[5])

  uintptr_t buffer_b[kBufferWords];
  Printf("\n  - Running OpTests in address ranges: [0x%p:0x%p]\n", &buffer[0],
         &buffer[kBufferWords - 1]);

  This a;
  expr = ExprInit(buffer, kBufferSize, kStackHeight, &a, buffer_b, kBufferSize);
  print << expr;

  PRINTF("\n    Testing Expr...\n")

  int io_number_ = 98;  //< ASCII:'b'
  BIn* bin = ExprBIn(expr);
  bout = ExprBOut(expr);

  ExprRingBell(expr);
  ExprAckBack(expr);
  result = BOutWrite(bout, Params<4, ADR, SVI, STR, Parent::kBufferSize, ADR>(),
                     Args(args, Address<'A', 'A', 'A'>(), &io_number_, "Test",
                          Address<BS, CR>()));

  Assert(result == nullptr)

  print << bout;

  slot.Clear();
  print << expr;

  // Utf& slot (bin, bout);
  // Bypass handshake for testing purposes.

  ExprUnpack(expr);  //, &slot);
  print << expr;
  PRINTF("\n Done with Op tests.")

  enum {
    kBufferSize = 2048,
    kBufferWords = kBufferSize / sizeof(uintptr_t),
    kStackHeight = 8,
    kPrintBufferSize = 4096,
  };

  Utf& print;

  uintptr_t buffer[kBufferWords], unpacked_buffer[kBufferWords];

  PRINT_HEADING("Creating test Slot...")

  Slot slot(buffer, kBufferWords);
  print << slot << Dump();

  PRINT_HEADING("Testing Expr...")
  This root;
  Expr* expr =
      ExprInit(buffer, kBufferSize, 4, &root, unpacked_buffer, kBufferWords);
  BOut* bout = ExprBOut(expr);
  PRINT_SLOT;

  void* args[19];
  const uint_t* params = Params<4, ADR, STR, 32, FLT, SI4>();
  const char stx_expected[] = "Hello world!\0";
  const int si4_expected = 1;
  const float flt_expected = 1.0f;

  char stx_found[64];
  int si4_found;
  float flt_found;

  const Op* result =
      ExprResult(expr, params,
                 Args(args, "C", &stx_expected, &si4_expected, &flt_expected));
  PRINTF("\n\n Printing...\n\n")
  print << expr << Dump();

  PRINTF("\n\n Attempting to print Expr\n\n")
  result =
      ExprArgs(expr, params, Args(args, &stx_found, &si4_found, &flt_found));
  Compare(0, result)
  print << expr << Dump();

  PRINTF(
      "\n\n  - Running ReadWriteTests...\nkBufferSize: %i "
      "kBufferWords: %i",
      kBufferSize, kBufferWords)

  const char expected_string1[] = "1234\0", expected_string2[] = "5678\0";

  char found_string1[6], found_string2[6];

  memset(found_string1, ' ', 6);
  memset(found_string2, ' ', 6);

  PRINTF("\n buffer_start:%p buffer_stop:%p\n", &buffer[0],
         &buffer[kBufferSize - 1]);
  PRINTF("\n &expected_string1[0]:%p &expected_string2[0]:%p\n",
         &expected_string1[0], &expected_string2[0]);

  bout = BOutInit(buffer, kBufferSize);

  Compare(0, BOutWrite(bout, Params<2, STR, 6, STR, 6>(),
                           Args(args, expected_string1, expected_string2)))
  void** test = Args(args, found_string1, found_string2);
  PRINTF("\n texpected_string1_start:%p texpected_string2_start:%p\n", &test[0],
         &test[1]);

  Compare(0, BOutRead(bout, Params<2, STR, 5, STR, 5>(),
                          Args(args, found_string1, found_string2)))

  PRINTF("\nExpected 1:%s Found 1:%s\nExpected 2:%s Found 2:%s",
         expected_string1, found_string1, expected_string2, found_string2)

  STRCMP_EQUAL(expected_string1, found_string1)
  // Print (bout);
  STRCMP_EQUAL(expected_string2, found_string2)

  PRINT_HEADING("Testing BOL/UI1/SI1...")

  static const int8_t si1_p_expected = '+', si1_n_expected = (int8_t)196;
  static const byte ui1_expected = '0', bol_expected = '?';
  int8_t si1_p_found, si1_n_found, bol_found;
  byte ui1_found;

  Compare(0, BOutWrite(bout, Params<4, SI1, SI1, UI1, BOL>(),
                           Args(args, &si1_p_expected, &si1_n_expected,
                                &ui1_expected, &bol_expected)))
  Compare(0, BOutRead(bout, Params<4, SI1, SI1, UI1, BOL>(),
                          Args(args, &si1_p_found, &si1_n_found, &ui1_found,
                               &bol_found)))
  Compare(si1_p_expected, si1_p_found)
  Compare(si1_n_expected, si1_n_found)
  Compare(ui1_expected, ui1_found)
  Compare(bol_expected, bol_found)

  PRINT_HEADING("Testing UI2/SI2/HLF...")

  static const int16_t si2_p_expected = '+',
                       si2_n_expected = (int16_t)(0xFF00 | '-');
  static const uint16_t ui2_expected = '2', hlf_expected = 227;

  int16_t si2_p_found, si2_n_found;
  uint16_t ui2_found, hlf_found;

  /*PRINTF ("Expecting %u %u"
  << si2_p_expected
  << (si2_p_expected >> 8)
  << ' '
  << si2_n_expected
  << (si2_n_expected >> 8)
  << ' '
  << ui2_expected
  << (ui2_expected >> 8)
  << ' '
  << hlf_expected
  << (hlf_expected >> 8)
  << '\n';*/

  Compare(0, BOutWrite(bout, Params<4, SI2, SI2, UI2, HLF>(),
                           Args(args, &si2_p_expected, &si2_n_expected,
                                &ui2_expected, &hlf_expected)))
  Compare(0, BOutRead(bout, Params<4, SI2, SI2, UI2, HLF>(),
                          Args(args, &si2_p_found, &si2_n_found, &ui2_found,
                               &hlf_found)))
  Compare(si2_p_expected, si2_p_found)
  Compare(si2_n_expected, si2_n_found)
  Compare(ui2_expected, ui2_found)
  Compare(hlf_expected, hlf_found)

  PRINT_HEADING("Testing UI4/SI4/FLT/TMS...")

  static const int32_t si4_p_expected = '+',
                       si4_n_expected = (int32_t)(0xFFFFFF00 | '-');
  static const uint32_t ui4_expected = '4';
  static const uint32_t flt_value = '.';
  static const float flt_expected2 = *(float*)&flt_value;
  static const Tms tms_expected = 0xE7;

  int32_t si4_p_found, si4_n_found;
  uint32_t ui4_found;
  Tms tms_found;

  Compare(0, BOutWrite(bout, Params<5, SI4, SI4, UI4, FLT, TMS>(),
                           Args(args, &si4_p_expected, &si4_n_expected,
                                &ui4_expected, &flt_expected2, &tms_expected)))
  Compare(0, BOutRead(bout, Params<5, SI4, SI4, UI4, FLT, TMS>(),
                          Args(args, &si4_p_found, &si4_n_found, &ui4_found,
                               &flt_found, &tms_found)))
  Compare(si4_p_expected, si4_p_found)
  Compare(si4_n_expected, si4_n_found)
  Compare(ui4_expected, ui4_found)
  Compare(flt_expected2, flt_found)

  PRINT_HEADING("Testing TME/UI8/SI1/DBL...\n")

  static const Tms tmu_expected = 0xE7;
  static const int64_t si8_p_expected = '+',
                       si8_n_expected = -(2 * 1024 * 1024);
  static const uint64_t ui8_expected = '8';
  static const uint64_t dbl_value = '.';
  static const double dbl_expected = 1.0;

  Tme tmu_found = 0;
  int64_t si8_p_found = 0, si8_n_found = 0;
  uint64_t ui8_found = 0;
  double dbl_found = 0.0;

  bout = BOutInit(buffer, kBufferSize);

  Compare(0, BOutWrite(bout, Params<5, TME, SI8, SI8, UI8, DBL>(),
                           Args(args, &tmu_expected, &si8_p_expected,
                                &si8_n_expected, &ui8_expected, &dbl_expected)))

  Compare(0, BOutRead(bout, Params<5, TME, SI8, SI8, UI8, DBL>(),
                          Args(args, &tmu_found, &si8_p_found, &si8_n_found,
                               &ui8_found, &dbl_found)))

  Compare(tmu_expected, tmu_found)
  Compare(si8_p_expected, si8_p_found)
  // si8 and dbl are not working for some reason.
  // Printf ("\n!!!    si8_n_expected: %i si8_n_found: %i\n\n",
  //            si8_n_expected, si8_n_found);
  // Printf ("\n!!!    dbl_expected: %f dbl_found: %f\n\n", dbl_expected,
  //        dbl_found);
  // Compare (si8_n_expected, si8_n_found) //< @todo Fix me!
  Compare(ui8_expected, ui8_found)
  // Compare (dbl_expected, dbl_found) //< @todo Fix me!

  PRINT_HEADING("Testing PackSVI and UnpackSVI...")

  Compare(1, UnpackSVI(PackSVI((int32_t)1)))
  Printf("Found: 0x%x\n", UnpackSVI(PackSVI(~0)));

  Compare(1 << 30, UnpackSVI(PackSVI(1 << 30)))
  Compare(~0, UnpackSVI(PackSVI(~0)))

  uint64_t ui8 = (uint64_t)1 << 62;
  int64_t si8 = (int64_t)ui8;
  Compare(si8, UnpackSV8(PackSV8(si8)))

  ui8 = ~(uint64_t)0;
  si8 = (int64_t)ui8;
  Compare(si8, UnpackSV8(PackSV8(si8)))

  PRINT_HEADING("Testing SVI...\n")

  static const int svi_expected[] = {0,         1,       -1,        1 << 7,
                                     -(1 << 7), 1 << 14, -(1 << 14)};

  int16_t svi_found[7];
  Compare(
      0, BOutWrite(bout, Params<7, SVI, SVI, SVI, SVI, SVI, SVI, SVI>(),
                   Args(args, &svi_expected[0], &svi_expected[1],
                        &svi_expected[2], &svi_expected[3], &svi_expected[4],
                        &svi_expected[5], &svi_expected[6])))
  Compare(0, BOutRead(bout, Params<7, SVI, SVI, SVI, SVI, SVI, SVI, SVI>(),
                          Args(args, &svi_found[0], &svi_found[1],
                               &svi_found[2], &svi_found[3], &svi_found[4],
                               &svi_found[5], &svi_found[6])))
  Compare(svi_expected[0], svi_found[0])
  Compare(svi_expected[1], svi_found[1])
  Compare(svi_expected[2], svi_found[2])
  Compare(svi_expected[3], svi_found[3])
  Compare(svi_expected[4], svi_found[4])
  Compare(svi_expected[5], svi_found[5])
  Compare(svi_expected[6], svi_found[6])

  PRINT_HEADING("Testing UVI...")

  static const uint16_t uv2_expected[] = {0, 1, 1 << 7, 1 << 14};

  uint16_t uv2_found[4];
  Compare(0, BOutWrite(bout, Params<4, UVI, UVI, UVI, UVI>(),
                           Args(args, &uv2_expected[0], &uv2_expected[1],
                                &uv2_expected[2], &uv2_expected[3])))
  Compare(0, BOutRead(bout, Params<4, UVI, UVI, UVI, UVI>(),
                          Args(args, &uv2_found[0], &uv2_found[1],
                               &uv2_found[2], &uv2_found[3])))
  Compare(uv2_expected[0], uv2_found[0])
  Compare(uv2_expected[1], uv2_found[1])
  Compare(uv2_expected[2], uv2_found[2])
  Compare(uv2_expected[3], uv2_found[3])

  PRINT_HEADING("\n  - Testing SVI...\n")

  static const int32_t sv4_expected[] = {
      0,          1,       -1,         1 << 7,  -(1 << 7), 1 << 14,
      -(1 << 14), 1 << 21, -(1 << 21), 1 << 28, -(1 << 28)};
  int32_t sv4_found[11];
  Compare(
      0,
      BOutWrite(
          bout,
          Params<11, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI>(),
          Args(args, &sv4_expected[0], &sv4_expected[1], &sv4_expected[2],
               &sv4_expected[3], &sv4_expected[4], &sv4_expected[5],
               &sv4_expected[6], &sv4_expected[7], &sv4_expected[8],
               &sv4_expected[9], &sv4_expected[10])))
  Compare(
      0,
      BOutRead(
          bout,
          Params<11, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI, SVI>(),
          Args(args, &sv4_found[0], &sv4_found[1], &sv4_found[2], &sv4_found[3],
               &sv4_found[4], &sv4_found[5], &sv4_found[6], &sv4_found[7],
               &sv4_found[8], &sv4_found[9], &sv4_found[10])))
  Compare(sv4_expected[0], sv4_found[0])
  Compare(sv4_expected[1], sv4_found[1])
  Compare(sv4_expected[2], sv4_found[2])
  Compare(sv4_expected[3], sv4_found[3])
  Compare(sv4_expected[4], sv4_found[4])
  Compare(sv4_expected[5], sv4_found[5])
  Compare(sv4_expected[6], sv4_found[6])
  Compare(sv4_expected[7], sv4_found[7])
  Compare(sv4_expected[8], sv4_found[8])
  Compare(sv4_expected[9], sv4_found[9])
  Compare(sv4_expected[10], sv4_found[10])

  PRINT_HEADING("Testing UVI...")

  static const uint32_t uv4_expected[] = {0,       1,       1 << 7,
                                          1 << 14, 1 << 21, 1 << 28};
  uint32_t uv4_found[6];
  Compare(0, BOutWrite(bout, Params<6, UVI, UVI, UVI, UVI, UVI, UVI>(),
                           Args(args, &uv4_expected[0], &uv4_expected[1],
                                &uv4_expected[2], &uv4_expected[3],
                                &uv4_expected[4], &uv4_expected[5])))
  Compare(0,
              BOutRead(bout, Params<6, UVI, UVI, UVI, UVI, UVI, UVI>(),
                       Args(args, &uv4_found[0], &uv4_found[1], &uv4_found[2],
                            &uv4_found[3], &uv4_found[4], &uv4_found[5])))
  Compare(uv4_expected[0], uv4_found[0])
  Compare(uv4_expected[1], uv4_found[1])
  Compare(uv4_expected[2], uv4_found[2])
  Compare(uv4_expected[3], uv4_found[3])
  Compare(uv4_expected[4], uv4_found[4])
  Compare(uv4_expected[5], uv4_found[5])

  PRINT_HEADING("Testing SV8...")

  static const int64_t sv8_expected[] = {0,
                                         1,
                                         -1,
                                         1 << 7,
                                         -(1 << 7),
                                         1 << 14,
                                         -(1 << 14),
                                         1 << 21,
                                         -(1 << 21),
                                         1 << 28,
                                         -(1 << 28),
                                         ((int64_t)1) << 35,
                                         -(((int64_t)1) << 35),
                                         ((int64_t)1) << 42,
                                         -(((int64_t)1) << 42),
                                         ((int64_t)1) << 49,
                                         -(((int64_t)1) << 49),
                                         ((int64_t)1) << 56,
                                         -(((int64_t)1) << 56)};
  int64_t sv8_found[19];
  Compare(
      0, BOutWrite(bout,
                   Params<19, SV8, SV8, SV8, SV8, SV8, SV8, SV8, SV8, SV8, SV8,
                          SV8, SV8, SV8, SV8, SV8, SV8, SV8, SV8, SV8>(),
                   Args(args, &sv8_expected[0], &sv8_expected[1],
                        &sv8_expected[2], &sv8_expected[3], &sv8_expected[4],
                        &sv8_expected[5], &sv8_expected[6], &sv8_expected[7],
                        &sv8_expected[8], &sv8_expected[9], &sv8_expected[10],
                        &sv8_expected[11], &sv8_expected[12], &sv8_expected[13],
                        &sv8_expected[14], &sv8_expected[15], &sv8_expected[16],
                        &sv8_expected[17], &sv8_expected[18])))
  Compare(
      0,
      BOutRead(
          bout,
          Params<19, SV8, SV8, SV8, SV8, SV8, SV8, SV8, SV8, SV8, SV8, SV8, SV8,
                 SV8, SV8, SV8, SV8, SV8, SV8, SV8>(),
          Args(args, &sv8_found[0], &sv8_found[1], &sv8_found[2], &sv8_found[3],
               &sv8_found[4], &sv8_found[5], &sv8_found[6], &sv8_found[7],
               &sv8_found[8], &sv8_found[9], &sv8_found[10], &sv8_found[11],
               &sv8_found[12], &sv8_found[13], &sv8_found[14], &sv8_found[15],
               &sv8_found[16], &sv8_found[17], &sv8_found[18])))
  Compare(sv8_expected[0], sv8_found[0])
  Compare(sv8_expected[1], sv8_found[1])
  Compare(sv8_expected[2], sv8_found[2])
  Compare(sv8_expected[3], sv8_found[3])
  Compare(sv8_expected[4], sv8_found[4])
  Compare(sv8_expected[5], sv8_found[5])
  Compare(sv8_expected[6], sv8_found[6])
  Compare(sv8_expected[7], sv8_found[7])
  Compare(sv8_expected[8], sv8_found[8])
  Compare(sv8_expected[9], sv8_found[9])
  Compare(sv8_expected[10], sv8_found[10])
  Compare(sv8_expected[11], sv8_found[11])
  Compare(sv8_expected[12], sv8_found[12])
  Compare(sv8_expected[13], sv8_found[13])
  Compare(sv8_expected[14], sv8_found[14])
  Compare(sv8_expected[15], sv8_found[15])
  Compare(sv8_expected[16], sv8_found[16])
  Compare(sv8_expected[17], sv8_found[17])
  Compare(sv8_expected[18], sv8_found[18])

  PRINT_HEADING("Testing UV8...")

  static const uint64_t uv8_expected[] = {0,
                                          1,
                                          1 << 7,
                                          1 << 14,
                                          1 << 21,
                                          1 << 28,
                                          ((uint64_t)1) << 35,
                                          ((uint64_t)1) << 42,
                                          ((uint64_t)1) << 49,
                                          ((uint64_t)1) << 56};
  uint64_t uv8_found[10];
  Compare(
      0,
      BOutWrite(bout,
                Params<10, UV8, UV8, UV8, UV8, UV8, UV8, UV8, UV8, UV8, UV8>(),
                Args(args, &uv8_expected[0], &uv8_expected[1], &uv8_expected[2],
                     &uv8_expected[3], &uv8_expected[4], &uv8_expected[5],
                     &uv8_expected[6], &uv8_expected[7], &uv8_expected[8],
                     &uv8_expected[9], &uv8_expected[10], &uv8_expected[11],
                     &uv8_expected[12], &uv8_expected[13])))
  Compare(
      0,
      BOutRead(bout,
               Params<10, UV8, UV8, UV8, UV8, UV8, UV8, UV8, UV8, UV8, UV8>(),
               Args(args, &uv8_found[0], &uv8_found[1], &uv8_found[2],
                    &uv8_found[3], &uv8_found[4], &uv8_found[5], &uv8_found[6],
                    &uv8_found[7], &uv8_found[8], &uv8_found[9], &uv8_found[10],
                    &uv8_found[11], &uv8_found[12], &uv8_found[13])))
  Compare(uv8_expected[0], uv8_found[0])
  Compare(uv8_expected[1], uv8_found[1])
  Compare(uv8_expected[2], uv8_found[2])
  Compare(uv8_expected[3], uv8_found[3])
  Compare(uv8_expected[4], uv8_found[4])
  Compare(uv8_expected[5], uv8_found[5])
  Compare(uv8_expected[6], uv8_found[6])
  Compare(uv8_expected[7], uv8_found[7])
  Compare(uv8_expected[8], uv8_found[8])
  Compare(uv8_expected[9], uv8_found[9])

  uintptr_t buffer_b[kBufferWords];
  Printf("\n  - Running OpTests in address ranges: [0x%p:0x%p]\n", &buffer[0],
         &buffer[kBufferWords - 1]);

  This a;
  expr = ExprInit(buffer, kBufferSize, kStackHeight, &a, buffer_b, kBufferSize);
  print << expr;

  PRINTF("\n    Testing Expr...\n")

  uint8_t io_number_ = 98;  //< ASCII:'b'
  BIn* bin = ExprBIn(expr);
  bout = ExprBOut(expr);

  ExprRingBell(expr);
  ExprAckBack(expr);
  result =
      BOutWrite(bout, Params<4, ADR, UI1, STR, Parent::kUtfBufferSize, ADR>(),
                Args(args, Address<'A', 'A', 'A'>(), &io_number_, "Test",
                     Address<BS, CR>()));

  Assert(result == nullptr)

  print << bout;

  slot.Clear();
  print << expr;

  // Utf& slot (bin, bout);
  // Bypass handshake for testing purposes.

  ExprUnpack(expr);  //, &slot);
  print << expr;
  PRINTF("\n Done with Op tests.")

  std::cout << "\n  - Running TableTest...\n";
  wchar_t index;
  uintptr_t buffer[128];
  Printf("\n &buffer[0]:%p &buffer[127]:%p\n", &buffer[0], &buffer[127]);
  Table* table = TableInit(buffer, 8, 128);

  Assert(table != nullptr)

  index = TableAdd(table, "D");
  Compare(0, index)
  index = TableFind(table, "D");
  Compare(0, index)

  index = TableAdd(table, "C");
  Compare(1, index)
  index = TableFind(table, "D");
  Compare(0, index)
  index = TableFind(table, "C");
  Compare(1, index)

  index = TableAdd(table, "B");
  Compare(2, index)
  index = TableFind(table, "D");
  Compare(0, index)
  index = TableFind(table, "C");
  Compare(1, index)
  index = TableFind(table, "B");
  Compare(2, index)

  index = TableAdd(table, "A");
  Compare(3, index)
  index = TableFind(table, "D");
  Compare(0, index)
  index = TableFind(table, "C");
  Compare(1, index)
  index = TableFind(table, "B");
  Compare(2, index)
  index = TableFind(table, "A");
  Compare(3, index)

  index = TableAdd(table, "abc");
  Compare(4, index)
  index = TableFind(table, "abc");
  Compare(4, index)

  index = TableAdd(table, "bac");
  Compare(5, index)
  index = TableFind(table, "abc");
  Compare(4, index)
  index = TableFind(table, "bac");
  Compare(5, index)

  index = TableAdd(table, "cba");
  Compare(6, index)
  index = TableFind(table, "abc");
  Compare(4, index)
  index = TableFind(table, "bac");
  Compare(5, index)
  index = TableFind(table, "cba");
  Compare(6, index)

  index = TableAdd(table, "cab");
  Compare(7, index)
  index = TableFind(table, "abc");
  Compare(4, index)
  index = TableFind(table, "bac");
  Compare(5, index)
  index = TableFind(table, "cba");
  Compare(6, index)
  index = TableFind(table, "cab");
  Compare(7, index)

  index = TableAdd(table, "test");
  Compare(kInvalidIndex, index)

  TablePrint(table);

  PRINTF("\n\n Done testing Table!! ({:-)-+=<")

  PRINTF("\n\n Testing VTable...\n\n")

  PAUSE("\n\nDone Testing SEAM_0_0_4! ({:-)-+=<")
}
#undef PAUSE
#undef PRINTF
#else
void TestSeam_1_4() {}
#endif  //< #if SEAM >= SEAM_0_0_4
