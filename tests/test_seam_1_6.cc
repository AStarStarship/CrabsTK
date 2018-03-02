/** Kabuki Toolkit
    @version 0.x
    @file    ~/tests/test_seam_1_5.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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
#include "tests_global.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 6

using namespace _;

TEST_GROUP (SEAM_1_6_TESTS) {
    void setup () {

    }

    void teardown () {
        std::cout << "\n";
        system ("PAUSE");
    }
};

TEST (SEAM_1_6_TESTS, BookTests) {
    PrintLineBreak ("\n  + Running BookTests\n", 10);

    PrintLineBreak ("\n  - Running BookInit...\n", 5, ' ');
    int8_t index;

    enum {
        kBufferSize = 256,
        kBufferSizeWords = kBufferSize / sizeof (uintptr_t),
    };

    uintptr_t buffer[kBufferSizeWords];

    Book2* book = Book2Init (buffer, 8, kBufferSize, 128);

    CHECK (book != nullptr)

    index = Book2Add<uint8_t, UI1> (book, "D", (byte)0xFF);

    CHECK_EQUAL (0, index)
    Book2Print (book);
    CHECK_EQUAL (0, index)
    index = Book2Find (book, "D");
    CHECK_EQUAL (0, index)
    system ("PAUSE");
    index = Book2Add<uint8_t, UI1> (book, "C", (byte)0xFF);
    CHECK_EQUAL (1, index)
    index = Book2Find (book, "D");
    CHECK_EQUAL (0, index)
    index = Book2Find (book, "C");
    CHECK_EQUAL (1, index)

    index = Book2Add<uint8_t, UI1> (book, "BIn", (byte)0xFF);
    CHECK_EQUAL (2, index)
    index = Book2Find (book, "D");
    CHECK_EQUAL (0, index)
    index = Book2Find (book, "C");
    CHECK_EQUAL (1, index)
    index = Book2Find (book, "BIn");
    CHECK_EQUAL (2, index)

    index = Book2Add<uint8_t, UI1> (book, "A", (byte)0xFF);
    CHECK_EQUAL (3, index)
    index = Book2Find (book, "D");
    CHECK_EQUAL (0, index)
    index = Book2Find (book, "C");
    CHECK_EQUAL (1, index)
    index = Book2Find (book, "BIn");
    CHECK_EQUAL (2, index)
    index = Book2Find (book, "A");
    CHECK_EQUAL (3, index)

    index = Book2Add<uint8_t, UI1> (book, "abc", (byte)0xFF);
    CHECK_EQUAL (4, index)
    index = Book2Find (book, "abc");
    CHECK_EQUAL (4, index)

    index = Book2Add<uint8_t, UI1> (book, "bac", (byte)0xFF);
    CHECK_EQUAL (5, index)
    index = Book2Find (book, "abc");
    CHECK_EQUAL (4, index)
    index = Book2Find (book, "bac");
    CHECK_EQUAL (5, index)

    index = Book2Add<uint8_t, UI1> (book, "cba", (byte)0xFF);
    CHECK_EQUAL (6, index)
    index = Book2Find (book, "abc");
    CHECK_EQUAL (4, index)
    index = Book2Find (book, "bac");
    CHECK_EQUAL (5, index)
    index = Book2Find (book, "cba");
    CHECK_EQUAL (6, index)

    index = Book2Add<uint8_t, UI1> (book, "cab", (byte)0xFF);
    CHECK_EQUAL (7, index)
    index = Book2Find (book, "abc");
    CHECK_EQUAL (4, index)
    index = Book2Find (book, "bac");
    CHECK_EQUAL (5, index)
    index = Book2Find (book, "cba");
    CHECK_EQUAL (6, index)
    index = Book2Find (book, "cab");
    CHECK_EQUAL (7, index)

    index = Book2Add<uint8_t, UI1> (book, "test", (byte)0xFF);
    CHECK_EQUAL (index, -1)
}

TEST (SEAM_1_5_TESTS, ClockTests) {
    time_t t,
           t_found;
    tm* lt;
    const char* result;

    PrintBreak ("<", '-');
    std::cout << "< Testing date-time parser... \n";

    const char* strings[] = { "8/9\0",
        "08/09\0",
        "8/9/17\0",
        "8/09/17\0",
        "8/9/2017\0",
        "8/09/2017\0",
        "8/09/2017\0",
        "08/9/2017\0",
        "8/09/2017@00\0",
        "8.09.2017@00AM\0",
        "8/09/2017@00:00\0",
        "8/09/17@00:0AM\0",
        "8/09/2017@00:00:00\0",
        "8/09/2017@00:00:00AM\0",
        "2017-08-09@00:00:00AM\0",
        "2017-08-09@00:00:00am\0",
        "2017-08-09@00:00:00A\0",
        "2017-08-09@00:00:00a \0",
    };

    for (int i = 0; i < 18; ++i) {
        PrintBreak ("<", '-');
        std::cout << "\n " << i;
        time_t t = 0;
        result = ParseTimeText (strings[i], t);
        CompareTimes (t, 2017, 8, 9, 0, 0, 0);
    }
    enum {
        kSize = 32
    };

    char timestamp[kSize];
    PrintBreak ("<", '-');
    PrintBreak ("<\nTesting more valid input...\n");
    PrintBreak ("<", '-');

    t = TestTime<8, 9, 17, 4, 20> (timestamp, kSize);
    PrintTime (t);
    result = ParseTimeText (timestamp, t_found);
    CompareTimes (t, t_found);

    t = TestTime<2020, 4, 20, 4, 20> (timestamp, kSize);
    PrintTime (t);
    result = ParseTimeText (timestamp, t_found);
    CompareTimes (t, t_found);

    t = TestTime<1947, 12, 7, 23, 5, 7> (timestamp, kSize);
    PrintTime (t);
    result = ParseTimeText (timestamp, t_found);
    CompareTimes (t, t_found);

    PrintBreak ("<", '-');
    PrintBreak ("<\nTesting invalid input...\n", '-');
    ParseTimeText ("cat", t);
    PrintBreak ("<", '-');
    ParseTimeText ("2017-30-40", t);
    PrintBreak ("<", '-');

    std::cout << "<\nDone testing date parsing utils! :-)\n";
}

#if USING_CRABS_TABLE
TEST (SEAM_1_5_TESTS, TableTests) {
    std::cout << "\n  - Running TableTest...\n";
    char_t index;
    uintptr_t buffer[128];
    printf ("\n &buffer[0]:%p &buffer[127]:%p\n", &buffer[0], &buffer[127]);
    Table* table = TableInit (buffer, 8, 128);

    CHECK (table != nullptr)

    index = TableAdd (table, "D");
    CHECK_EQUAL (0, index)
    index = TableFind (table, "D");
    CHECK_EQUAL (0, index)

    index = TableAdd (table, "C");
    CHECK_EQUAL (1, index)
    index = TableFind (table, "D");
    CHECK_EQUAL (0, index)
    index = TableFind (table, "C");
    CHECK_EQUAL (1, index)

    index = TableAdd (table, "BIn");
    CHECK_EQUAL (2, index)
    index = TableFind (table, "D");
    CHECK_EQUAL (0, index)
    index = TableFind (table, "C");
    CHECK_EQUAL (1, index)
    index = TableFind (table, "BIn");
    CHECK_EQUAL (2, index)

    index = TableAdd (table, "A");
    CHECK_EQUAL (3, index)
    index = TableFind (table, "D");
    CHECK_EQUAL (0, index)
    index = TableFind (table, "C");
    CHECK_EQUAL (1, index)
    index = TableFind (table, "BIn");
    CHECK_EQUAL (2, index)
    index = TableFind (table, "A");
    CHECK_EQUAL (3, index)

    index = TableAdd (table, "abc");
    CHECK_EQUAL (4, index)
    index = TableFind (table, "abc");
    CHECK_EQUAL (4, index)

    index = TableAdd (table, "bac");
    CHECK_EQUAL (5, index)
    index = TableFind (table, "abc");
    CHECK_EQUAL (4, index)
    index = TableFind (table, "bac");
    CHECK_EQUAL (5, index)

    index = TableAdd (table, "cba");
    CHECK_EQUAL (6, index)
    index = TableFind (table, "abc");
    CHECK_EQUAL (4, index)
    index = TableFind (table, "bac");
    CHECK_EQUAL (5, index)
    index = TableFind (table, "cba");
    CHECK_EQUAL (6, index)

    index = TableAdd (table, "cab");
    CHECK_EQUAL (7, index)
    index = TableFind (table, "abc");
    CHECK_EQUAL (4, index)
    index = TableFind (table, "bac");
    CHECK_EQUAL (5, index)
    index = TableFind (table, "cba");
    CHECK_EQUAL (6, index)
    index = TableFind (table, "cab");
    CHECK_EQUAL (7, index)

    index = TableAdd (table, "test");
    CHECK_EQUAL (kInvalidIndex, index)

    TablePrint (table);
}
#endif  //< USING_CRABS_TABLE

TEST (SEAM_1_5_TESTS, ExprTests) {
    Text<> text;
    std::cout << "\n Running ExprTests...\n";
    enum {
        kBufferSize      = 256,
        kBufferSizeWords = kBufferSize / sizeof (uint_t)
    };

    uintptr_t buffer[kBufferSizeWords],
              unpacked_buffer[kBufferSizeWords];
    This root;
    Expr* expr = ExprInit (buffer, kBufferSize, 4, 
                                       &root, unpacked_buffer,
                                       kBufferSizeWords);
    PrintExpr (expr);

    void*         args[4];
    const uint_t* params         = Bsq <4, ADR, STR, 32, FLT, SI4> ();
    const char    stx_expected[] = "Hello world!\0";
    const int     si4_expected   = 1;
    const float   flt_expected   = 1.0f;

    char stx_found[64];
    int si4_found;
    float flt_found;
    expr->return_address = "A";

    const Op* result = ExprResult (expr, params,
                                   Args (args, "C", &stx_expected,
                                         &si4_expected, &flt_expected));
    text << "\n Attempting to print Expr " << PrintPointer (ExprBOut (expr))
         <<  PrintExpr (expr, text) << Print ();
    CHECK_EQUAL (0, ExprArgs (expr, params,
                              Args (args, &stx_found, &si4_found, &flt_found)));
}

TEST (SEAM_1_5_TESTS, RoomTests) {
    printf ("\n  - Running RoomTestOne...\n");
}

TEST (SEAM_1_5_TESTS, ReadWriteTests) {
    enum {
        kBufferSize = 256,
        kElementsBuffer = kBufferSize / sizeof (uintptr_t)
    };

    uintptr_t buffer[kElementsBuffer + 4];  //< Something isn't aligned right.
    //< It works right with an extra 4 elements but no less. Am I writing
    //< something to the end of the buffer???

    PrintLineBreak ("\n  - Running ReadWriteTests...", 5);
    std::cout << " kBufferSize: "     << kBufferSize 
              << " kElementsBuffer: " << kElementsBuffer;

    void* args[19];

    const char expected_string1[] = "1234\0",
               expected_string2[] = "5678\0";

    char found_string1[6],
         found_string2[6];

    memset (found_string1, ' ', 6);
    memset (found_string2, ' ', 6);

    printf ("\n buffer_start:%p buffer_stop:%p\n", &buffer[0], 
            &buffer[kBufferSize - 1]);
    printf ("\n &expected_string1[0]:%p &expected_string2[0]:%p\n", 
            &expected_string1[0], &expected_string2[0]);

    BOut* bout = BOutInit (buffer, kBufferSize);
    
    CHECK_EQUAL (0, BOutWrite (bout, Bsq<2, STR, 6, STR, 6> (),
                               Args (args, expected_string1,
                                     expected_string2)))
    void** test = Args (args, found_string1, found_string2);
    printf ("\n texpected_string1_start:%p texpected_string2_start:%p\n",
            &test[0], &test[1]);
    
    CHECK_EQUAL (0, BOutRead (bout, Bsq<2, STR, 5, STR, 5> (),
                              Args (args, found_string1, found_string2)))

    std::cout << "\nExpected 1: " << expected_string1
              << " Found 1: "     << found_string1
              << "\nExpected 2: " << expected_string2
              << " Found 2: "     << found_string2;

    STRCMP_EQUAL (expected_string1, found_string1)
    //BOutPrint (bout);
    STRCMP_EQUAL (expected_string2, found_string2)
    
    PrintLineBreak ("\n  - Testing BOL/UI1/SI1...", 5);

    static const int8_t si1_p_expected = '+',
                        si1_n_expected = (int8_t)196;
    static const byte   ui1_expected = '0',
                        bol_expected = '?';
    int8_t si1_p_found,
           si1_n_found,
           bol_found;
    byte   ui1_found;

    CHECK_EQUAL (0, BOutWrite (bout, Bsq<4, SI1, SI1, UI1, BOL> (),
                               Args (args, &si1_p_expected, &si1_n_expected, 
                                     &ui1_expected, &bol_expected)))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<4, SI1, SI1, UI1, BOL> (),
                              Args (args, &si1_p_found, &si1_n_found,
                                    &ui1_found, &bol_found)))
    CHECK_EQUAL (si1_p_expected, si1_p_found)
    CHECK_EQUAL (si1_n_expected, si1_n_found)
    CHECK_EQUAL (ui1_expected, ui1_found)
    CHECK_EQUAL (bol_expected, bol_found)

    PrintLineBreak ("\n  - Testing UI2/SI2/HLF...", 5);

    static const int16_t si2_p_expected = '+',
        si2_n_expected = (int16_t)(0xFF00 | '-');
    static const uint16_t ui2_expected = '2',
        hlf_expected = 227;

    int16_t si2_p_found,
        si2_n_found;
    uint16_t ui2_found,
        hlf_found;

    std::cout << "Expecting "
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
              << '\n';

    CHECK_EQUAL (0, BOutWrite (bout, Bsq<4, SI2, SI2, UI2, HLF> (),
                           Args (args, &si2_p_expected, &si2_n_expected, 
                                 &ui2_expected, &hlf_expected)))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<4, SI2, SI2, UI2, HLF> (),
                          Args (args, &si2_p_found, &si2_n_found, &ui2_found, 
                                &hlf_found)))
    CHECK_EQUAL (si2_p_expected, si2_p_found)
    CHECK_EQUAL (si2_n_expected, si2_n_found)
    CHECK_EQUAL (ui2_expected, ui2_found)
    CHECK_EQUAL (hlf_expected, hlf_found)

    PrintLineBreak ("\n  - Testing UI4/SI4/FLT/TMS...", 5);

    static const int32_t  si4_p_expected = '+',
                          si4_n_expected = (int32_t)(0xFFFFFF00 | '-');
    static const uint32_t ui4_expected = '4';
    static const uint32_t flt_value = '.';
    static const float    flt_expected = *(float*)&flt_value;
    static const time_t   tms_expected = 0xE7;
    int32_t               si4_p_found,
                          si4_n_found;
    uint32_t              ui4_found;
    float                 flt_found;
    time_t                tms_found;

    CHECK_EQUAL (0, BOutWrite (bout, Bsq<5, SI4, SI4, UI4, FLT, TMS> (),
                               Args (args, &si4_p_expected, &si4_n_expected,
                                     &ui4_expected, &flt_expected, 
                                     &tms_expected)))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<5, SI4, SI4, UI4, FLT, TMS> (),
                              Args (args, &si4_p_found, &si4_n_found,
                                    &ui4_found, &flt_found, &tms_found)))
    CHECK_EQUAL (si4_p_expected, si4_p_found)
    CHECK_EQUAL (si4_n_expected, si4_n_found)
    CHECK_EQUAL (ui4_expected, ui4_found)
    CHECK_EQUAL (flt_expected, flt_found)
    
    PrintLineBreak ("\n  - Testing TMU/UI8/SI1/DBL...\n", 5);

    static const time_t tmu_expected = 0xE7;
    static const int64_t si8_p_expected = '+',
        si8_n_expected = -(2*1024*1024);
    static const uint64_t ui8_expected = '8';
    static const uint64_t dbl_value = '.';
    static const double dbl_expected = 1.0;

    time_us_t tmu_found = 0;
    int64_t si8_p_found = 0,
        si8_n_found = 0;
    uint64_t ui8_found = 0;
    double dbl_found = 0.0;

    bout = BOutInit (buffer, kBufferSize);

    CHECK_EQUAL (0, BOutWrite (bout, Bsq<5, TMU, SI8, SI8, UI8, DBL> (),
           Args (args, &tmu_expected, &si8_p_expected, &si8_n_expected,
                 &ui8_expected, &dbl_expected)))

    CHECK_EQUAL (0, BOutRead (bout, Bsq<5, TMU, SI8, SI8, UI8, DBL> (),
          Args (args, &tmu_found, &si8_p_found, &si8_n_found, &ui8_found,
                &dbl_found)))

    CHECK_EQUAL (tmu_expected, tmu_found)
    CHECK_EQUAL (si8_p_expected, si8_p_found)
    // si8 and dbl are not working for some reason.
    //printf ("\n!!!    si8_n_expected: %i si8_n_found: %i\n\n",
    //            si8_n_expected, si8_n_found);
    //printf ("\n!!!    dbl_expected: %f dbl_found: %f\n\n", dbl_expected,
    //        dbl_found);
    //CHECK_EQUAL (si8_n_expected, si8_n_found) //< @todo Fix me!
    CHECK_EQUAL (ui8_expected, ui8_found)
    //CHECK_EQUAL (dbl_expected, dbl_found) //< @todo Fix me!

    PrintLineBreak ("  - Testing PackSignedVarint and UnpackSignedVarint...\n", 5);

    CHECK_EQUAL (1, UnpackSignedVarint<uint16_t> 
                    (PackSignedVarint<uint16_t> (1)))
    printf ("Found: 0x%x\n", 
            UnpackSignedVarint<uint16_t> (PackSignedVarint<uint16_t> (~0)));
    CHECK_EQUAL (((uint16_t)~0), UnpackSignedVarint<uint16_t>
                                 (PackSignedVarint <uint16_t> (~0)))

    CHECK_EQUAL (1 << 30, UnpackSignedVarint<uint32_t>
                          (PackSignedVarint <uint32_t> (1 << 30)))
    CHECK_EQUAL (~0, UnpackSignedVarint<uint32_t>
                     (PackSignedVarint <uint32_t> (~0)))
        

    CHECK_EQUAL (((uint64_t)1) << 62, UnpackSignedVarint<uint64_t> (PackSignedVarint<uint64_t> (((uint64_t)1) << 62)))
    CHECK_EQUAL (~0, UnpackSignedVarint<uint64_t> 
                     (PackSignedVarint<uint64_t>(~0)))
    
    PrintLineBreak ("  - Testing SVI...\n", 5);

    static const int16_t sv2_expected[] = {
        0,
        1,
        -1,
        1 << 7,
        -(1 << 7),
        1 << 14,
        -(1 << 14)
    };

    int16_t sv2_found[7];
    CHECK_EQUAL (0, BOutWrite (bout, Bsq<7, SVI, SVI, SVI, SVI, SVI, SVI, 
                                            SVI> (),
                           Args (args, &sv2_expected[0], &sv2_expected[1],
                                 &sv2_expected[2], &sv2_expected[3],
                                 &sv2_expected[4], &sv2_expected[5],
                                 &sv2_expected[6])))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<7, SVI, SVI, SVI, SVI, SVI, SVI, 
                                           SVI>(),
                          Args (args, &sv2_found[0], &sv2_found[1], 
                                &sv2_found[2], &sv2_found[3], 
                                &sv2_found[4], &sv2_found[5],
                                &sv2_found[6])))
    CHECK_EQUAL (sv2_expected[0], sv2_found[0])
    CHECK_EQUAL (sv2_expected[1], sv2_found[1])
    CHECK_EQUAL (sv2_expected[2], sv2_found[2])
    CHECK_EQUAL (sv2_expected[3], sv2_found[3])
    CHECK_EQUAL (sv2_expected[4], sv2_found[4])
    CHECK_EQUAL (sv2_expected[5], sv2_found[5])
    CHECK_EQUAL (sv2_expected[6], sv2_found[6])

    PrintLineBreak ("\n  - Testing UVI...\n", 5);

    static const uint16_t uv2_expected[] = { 0, 1, 1 << 7, 1 << 14 };

    uint16_t uv2_found[4];
    CHECK_EQUAL (0, BOutWrite (bout, Bsq<4, UVI, UVI, UVI, UVI> (),
                           Args (args, &uv2_expected[0], &uv2_expected[1],
                                 &uv2_expected[2], &uv2_expected[3])))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<4, UVI, UVI, UVI, UVI> (),
                          Args (args, &uv2_found[0], &uv2_found[1],
                                &uv2_found[2], &uv2_found[3])))
    CHECK_EQUAL (uv2_expected[0], uv2_found[0])
    CHECK_EQUAL (uv2_expected[1], uv2_found[1])
    CHECK_EQUAL (uv2_expected[2], uv2_found[2])
    CHECK_EQUAL (uv2_expected[3], uv2_found[3])
    
    PrintLineBreak ("\n  - Testing SV4...\n", 5);

    static const int32_t sv4_expected[] = { 0, 1, -1, 1 << 7, -(1 << 7), 
                                            1 << 14, -(1 << 14),
                                            1 << 21, -(1 << 21),
                                            1 << 28, -(1 << 28) };
    int32_t sv4_found[11];
    CHECK_EQUAL (0, BOutWrite (bout, Bsq<11, SV4, SV4, SV4, SV4, SV4, SV4, 
                                            SV4, SV4, SV4, SV4, SV4> (),
                           Args (args, &sv4_expected[0], &sv4_expected[1],
                                 &sv4_expected[2], &sv4_expected[3],
                                 &sv4_expected[4], &sv4_expected[5],
                                 &sv4_expected[6], &sv4_expected[7],
                                 &sv4_expected[8], &sv4_expected[9],
                                 &sv4_expected[10])))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<11, SV4, SV4, SV4, SV4, SV4, SV4, 
                                           SV4, SV4, SV4, SV4, SV4>(),
                          Args (args, &sv4_found[0], &sv4_found[1], 
                                &sv4_found[2], &sv4_found[3],
                                &sv4_found[4], &sv4_found[5],
                                &sv4_found[6], &sv4_found[7],
                                &sv4_found[8], &sv4_found[9],
                                &sv4_found[10])))
    CHECK_EQUAL (sv4_expected[0], sv4_found[0])
    CHECK_EQUAL (sv4_expected[1], sv4_found[1])
    CHECK_EQUAL (sv4_expected[2], sv4_found[2])
    CHECK_EQUAL (sv4_expected[3], sv4_found[3])
    CHECK_EQUAL (sv4_expected[4], sv4_found[4])
    CHECK_EQUAL (sv4_expected[5], sv4_found[5])
    CHECK_EQUAL (sv4_expected[6], sv4_found[6])
    CHECK_EQUAL (sv4_expected[7], sv4_found[7])
    CHECK_EQUAL (sv4_expected[8], sv4_found[8])
    CHECK_EQUAL (sv4_expected[9], sv4_found[9])
    CHECK_EQUAL (sv4_expected[10], sv4_found[10])

    PrintLineBreak ("\n  - Testing UV4...\n", 5);

    static const uint32_t uv4_expected[] = { 
        0, 1, 1 << 7, 1 << 14, 1 << 21, 1 << 28 };
    uint32_t uv4_found[6];
    CHECK_EQUAL (0, BOutWrite (bout, Bsq<6, UV4, UV4, UV4, UV4, UV4, UV4> (),
                           Args (args, &uv4_expected[0], &uv4_expected[1],
                                 &uv4_expected[2], &uv4_expected[3],
                                 &uv4_expected[4], &uv4_expected[5])))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<6, UV4, UV4, UV4, UV4, UV4, UV4> (),
                            Args (args, &uv4_found[0], &uv4_found[1],
                                  &uv4_found[2], &uv4_found[3],
                                  &uv4_found[4], &uv4_found[5])))
    CHECK_EQUAL (uv4_expected[0], uv4_found[0])
    CHECK_EQUAL (uv4_expected[1], uv4_found[1])
    CHECK_EQUAL (uv4_expected[2], uv4_found[2])
    CHECK_EQUAL (uv4_expected[3], uv4_found[3])
    CHECK_EQUAL (uv4_expected[4], uv4_found[4])
    CHECK_EQUAL (uv4_expected[5], uv4_found[5])
    
    PrintLineBreak ("\n  - Testing SV8...\n", 5);

    static const int64_t sv8_expected[] = {
        0, 1, -1, 1 << 7, -(1 << 7),
        1 << 14, -(1 << 14),
        1 << 21, -(1 << 21),  
        1 << 28, -(1 << 28),
        ((int64_t)1) << 35, -(((int64_t)1) << 35),
        ((int64_t)1) << 42, -(((int64_t)1) << 42),
        ((int64_t)1) << 49, -(((int64_t)1) << 49),
        ((int64_t)1) << 56, -(((int64_t)1) << 56)
    };
    int64_t sv8_found[19];
    CHECK_EQUAL (0, BOutWrite (bout, Bsq<19, SV8, SV8, SV8, SV8, SV8, SV8,
                                            SV8, SV8, SV8, SV8, SV8, SV8, SV8,
                                            SV8, SV8, SV8, SV8, SV8, SV8> (),
                           Args (args, &sv8_expected[0], &sv8_expected[1],
                                 &sv8_expected[ 2], &sv8_expected[3],
                                 &sv8_expected[ 4], &sv8_expected[5],
                                 &sv8_expected[ 6], &sv8_expected[7],
                                 &sv8_expected[ 8], &sv8_expected[9],
                                 &sv8_expected[10], &sv8_expected[11],
                                 &sv8_expected[12], &sv8_expected[13],
                                 &sv8_expected[14], &sv8_expected[15],
                                 &sv8_expected[16], &sv8_expected[17],
                                 &sv8_expected[18])))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<19, SV8, SV8, SV8, SV8, SV8, SV8, 
                                           SV8, SV8, SV8, SV8, SV8, SV8, SV8,
                                           SV8, SV8, SV8, SV8, SV8, SV8> (),
                            Args (args, &sv8_found[0], &sv8_found[1],
                                &sv8_found[2], &sv8_found[3],
                                &sv8_found[4], &sv8_found[5],
                                &sv8_found[6], &sv8_found[7],
                                &sv8_found[8], &sv8_found[9],
                                &sv8_found[10], &sv8_found[11],
                                &sv8_found[12], &sv8_found[13],
                                &sv8_found[14], &sv8_found[15],
                                &sv8_found[16], &sv8_found[17],
                                &sv8_found[18])))
        CHECK_EQUAL (sv8_expected[0], sv8_found[0])
        CHECK_EQUAL (sv8_expected[1], sv8_found[1])
        CHECK_EQUAL (sv8_expected[2], sv8_found[2])
        CHECK_EQUAL (sv8_expected[3], sv8_found[3])
        CHECK_EQUAL (sv8_expected[4], sv8_found[4])
        CHECK_EQUAL (sv8_expected[5], sv8_found[5])
        CHECK_EQUAL (sv8_expected[6], sv8_found[6])
        CHECK_EQUAL (sv8_expected[7], sv8_found[7])
        CHECK_EQUAL (sv8_expected[8], sv8_found[8])
        CHECK_EQUAL (sv8_expected[9], sv8_found[9])
        CHECK_EQUAL (sv8_expected[10], sv8_found[10])
        CHECK_EQUAL (sv8_expected[11], sv8_found[11])
        CHECK_EQUAL (sv8_expected[12], sv8_found[12])
        CHECK_EQUAL (sv8_expected[13], sv8_found[13])
        CHECK_EQUAL (sv8_expected[14], sv8_found[14])
        CHECK_EQUAL (sv8_expected[15], sv8_found[15])
        CHECK_EQUAL (sv8_expected[16], sv8_found[16])
        CHECK_EQUAL (sv8_expected[17], sv8_found[17])
        CHECK_EQUAL (sv8_expected[18], sv8_found[18])

        PrintLineBreak ("\n  - Testing UV8...\n", 5);

    static const uint64_t uv8_expected[] = {
        0, 1, 1 << 7, 1 << 14, 1 << 21,
        1 << 28, ((uint64_t)1) << 35, 
        ((uint64_t)1) << 42, 
        ((uint64_t)1) << 49, 
        ((uint64_t)1) << 56
    };
    uint64_t uv8_found[10];
    CHECK_EQUAL (0, BOutWrite (bout, Bsq<10, UV8, UV8, UV8, UV8, UV8, UV8, 
                                            UV8, UV8, UV8, UV8> (),
                               Args (args, &uv8_expected[0], &uv8_expected[1],
                                     &uv8_expected[2], &uv8_expected[3],
                                     &uv8_expected[4], &uv8_expected[5],
                                     &uv8_expected[6], &uv8_expected[7],
                                     &uv8_expected[8], &uv8_expected[9],
                                     &uv8_expected[10], &uv8_expected[11],
                                     &uv8_expected[12], &uv8_expected[13])))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<10, UV8, UV8, UV8, UV8, UV8, UV8, 
                                           UV8, UV8, UV8, UV8> (),
                              Args (args, &uv8_found[0], &uv8_found[1],
                                    &uv8_found[2], &uv8_found[3],
                                    &uv8_found[4], &uv8_found[5],
                                    &uv8_found[6], &uv8_found[7],
                                    &uv8_found[8], &uv8_found[9],
                                    &uv8_found[10], &uv8_found[11],
                                    &uv8_found[12], &uv8_found[13])))
    CHECK_EQUAL (uv8_expected[0], uv8_found[0])
    CHECK_EQUAL (uv8_expected[1], uv8_found[1])
    CHECK_EQUAL (uv8_expected[2], uv8_found[2])
    CHECK_EQUAL (uv8_expected[3], uv8_found[3])
    CHECK_EQUAL (uv8_expected[4], uv8_found[4])
    CHECK_EQUAL (uv8_expected[5], uv8_found[5])
    CHECK_EQUAL (uv8_expected[6], uv8_found[6])
    CHECK_EQUAL (uv8_expected[7], uv8_found[7])
    CHECK_EQUAL (uv8_expected[8], uv8_found[8])
    CHECK_EQUAL (uv8_expected[9], uv8_found[9])
}

TEST (SEAM_1_5_TESTS, OpTests) {
    enum {
        kBufferSize = 2048,
        kBufferWords = kBufferSize / sizeof (uintptr_t),
        kStackHeight = 8,
    };
    uintptr_t buffer[kBufferWords],
              unpacked_expr[kBufferWords];
    printf ("\n  - Running OpTests in address ranges: [0x%p:0x%p]\n",
            &buffer[0], &buffer[kBufferWords-1]);

    This a;

    Expr* expr = ExprInit (buffer, kBufferSize, kStackHeight, &a,
                                       unpacked_expr, kBufferSize);
    PrintExpr (expr);

    std::cout << "\n    Testing Root (@see \"a.h\")...\n";

    void* args[4];
    uint8_t io_number_ = 98; //< ASCII:'b'
    BIn * bin  = ExprBin  (expr);
    BOut* bout = ExprBOut (expr);
    
    const Op* result;
    ExprRingBell (expr);
    ExprAckBack (expr);
    result = BOutWrite (bout,
                        Bsq<4, ADR, UI1, STR, Parent::kTextBufferSize, 
                                ADR> (),
                        Args (args, Address <'A', 'A', 'A'> (), 
                              &io_number_, Const ("Test"), 
                              Address<BS, CR> ()));
    BOutPrint (bout);
    CHECK (result == nullptr)

    PrintExpr (expr);

    //Slot slot (bin, bout);
    // Bypass handshake for testing purposes.
    ExprScan (expr);//, &slot);
    PrintExpr (expr);
    std::cout << "\n Done with Op tests.";
    system ("PAUSE");
}

TEST (SEAM_1_5_TESTS, TextTests) {
    PRINTF ("\n\n Testing Text...\n\n";

    PRINTF ("\n\n Testing Text...";

    enum {
        kNumStrings = 5,
        kSize = 10,
    };

    static const char* test_strings[kNumStrings][2] = {
        { "?"      , ""        },
        { "?"      , "?"       },
        { "? "     , "?"       },
        { "Apples" , "Apples"  },
        { "Apples" , "Apples"  },
    };

    const char* end;

    char buffer_a[kSize],
         buffer_b[kSize];

    for (int i = 0; i < kNumStrings; ++i) {
        end = SlotWrite (buffer_a, buffer_a + kSize, test_strings[i][0]);
        CHECK (end != nullptr)
        end = SlotWrite (buffer_b, buffer_b + kSize, test_strings[i][0]);
        CHECK (end != nullptr)

        end = TextEquals (buffer_a, buffer_b);
        CHECK (end != nullptr)
        end = TextEquals (buffer_a, buffer_a + kSize, buffer_b);
        CHECK (end != nullptr)
    }

    PRINTF ("\n\n Testing Text write functions..."
              << "\n\n Expecting Line() followed by \"Testing 1, 2, 3\"...";
    Text<> text;
    PrintLine () << "\n Testing " << 1 << ", " << 2 << ", " << 3;

    PRINTF ("\n\n Wrote:\"" << PrintGetBegin () << "\""
              << "\n Testing Text::Memory (void*, int size)...";

    PrintMemory (PrintGetBegin (), PrintGetEnd ()) << Print ();

    PRINTF ("\n\n Done testing _::Text class...\n ";
    system ("PAUSE");
}
#endif      //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 4
