/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/pro_files/script_tests.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#include "../../../../../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "../../../../../cpputest/include/CppUTest/TestHarness.h"

#include "../include/module_all.h"
#include "global.h"

using namespace _;

TEST_GROUP (SCRIPT_TESTS) {
    void setup () {
        PrintLine ("| ");
    }

    void teardown () {
        std::cout << "| Test completed.\n";
        PrintLine ("+", '-');
    }
};

/*

template<uint year, uint month, uint day, uint  hour = 0, uint minute = 0,
    uint second = 0>
    time_t TestTime (char* buffer, int buffer_size) {
    if (buffer == nullptr)
        return 0;
    time_t t;
    time (&t);
    tm* moment = localtime (&t);
    if (!moment) {
        cout << "|\n| Created invalid test moment: " << moment << '\n';
        return 0;
    }
    moment->tm_year = year - TIME_EPOCH;
    moment->tm_mon = month - 1;
    moment->tm_mday = day;
    moment->tm_hour = hour;
    moment->tm_min = minute;
    moment->tm_sec = second;

    if (!PrintDateTimeString (buffer, buffer_size, moment)) {
        cout << "< Error making timestamp \n";

        return 0;
    }
    cout << "< Creating test time: ";
    PrintDateTime (moment);
    t = mktime (moment);
    if (t < 0) {
        cout << "< Invalid " << t << '\n';
        return 0;
    } else {
        cout << '\n';
    }
    return t;
}

TEST (SCRIPT_TESTS, ClockTests) {
    time_t t,
        t_found;
    tm* lt;
    const char* result;

    PrintBreak ("<", '-');
    cout << "< Testing date-time parser... \n";

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
        cout << "| " << i;
        time_t t = 0;
        result = ParseTimeString (strings[i], t);
        CompareTimes (t, 2017, 8, 9, 0, 0, 0);
    }
    enum {
        SIZE = 32
    };

    char timestamp[SIZE];
    PrintBreak ("<", '-');
    PrintBreak ("<\n< Testing more valid input...\n");
    PrintBreak ("<", '-');

    t = TestTime<8, 9, 17, 4, 20> (timestamp, SIZE);
    PrintTime (t);
    result = ParseTimeString (timestamp, t_found);
    CompareTimes (t, t_found);

    t = TestTime<2020, 4, 20, 4, 20> (timestamp, SIZE);
    PrintTime (t);
    result = ParseTimeString (timestamp, t_found);
    CompareTimes (t, t_found);

    t = TestTime<1947, 12, 7, 23, 5, 7> (timestamp, SIZE);
    PrintTime (t);
    result = ParseTimeString (timestamp, t_found);
    CompareTimes (t, t_found);

    //system ("PAUSE");

    PrintBreak ("<", '-');
    PrintBreak ("<\n< Testing invalid input...\n<", '-');
    ParseTimeString ("cat", t);
    PrintBreak ("<", '-');
    ParseTimeString ("2017-30-40", t);
    PrintBreak ("<", '-');

    cout << "<\n< Done testing date parsing utils! :-)\n";
}
TEST (SCRIPT_TESTS, ExpressionTests) {
    std::cout << "| Running ExpressionTests...\n";
    enum {
        kBufferSize = 248 / sizeof (uint_t)
    };

    uint_t buffer[kBufferSize];
    Root root;
    Expression* expr = ExpressionInit (buffer, 255, 4, &root);
    ExpressionPrint (expr);

    void*         args[4];
    const uint_t* esc            = Params <4, ADR, STR, 32, FLT, SI4> ();
    const char    stx_expected[] = "Hello world!\0";
    const int     si4_expected   = 1;
    const float   flt_expected   = 1.0f;

    char stx_found[64];
    int si4_found;
    float flt_found;
    expr->return_address = "A";
    printf ("\n| Attempting to print to 0x%p\n", ExpressionBout (expr));
    const Operation* m = Result (expr, esc, Args (args, "C", &stx_expected,
                                                       &si4_expected, &flt_expected));
    PrintMemory (expr, ExpressionEndAddress (expr));
    ExpressionPrint (expr);
    //Mirror mirror;                                  //< @todo fix me!
    //Portal* p = dynamic_cast<Portal*>(&mirror);     //< Not working?
    //Portal* p = reinterpret_cast<Portal*>(&mirror); //< 
    //ExpressionScan (expr, p);
    system ("PAUSE");
    CHECK_EQUAL (0, Args (expr, esc, Args (args, &stx_found,
                                         &si4_found, &flt_found)));
    system ("PAUSE");
}

TEST (SCRIPT_TESTS, RoomTests) {
    printf ("|  - Running RoomTestOne...\n");
}

TEST (SCRIPT_TESTS, BookTests) {
    PrintLineBreak ("|  + Running BookTests\n", 10);

    PrintLineBreak ("|  - Running BookInit...\n", 5, ' ');
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

    index = Book2Add<uint8_t, UI1> (book, "Bin", (byte)0xFF);
    CHECK_EQUAL (2, index)
    index = Book2Find (book, "D");
    CHECK_EQUAL (0, index)
    index = Book2Find (book, "C");
    CHECK_EQUAL (1, index)
    index = Book2Find (book, "Bin");
    CHECK_EQUAL (2, index)

    index = Book2Add<uint8_t, UI1> (book, "A", (byte)0xFF);
    CHECK_EQUAL (3, index)
    index = Book2Find (book, "D");
    CHECK_EQUAL (0, index)
    index = Book2Find (book, "C");
    CHECK_EQUAL (1, index)
    index = Book2Find (book, "Bin");
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
}*/

TEST (SCRIPT_TESTS, TableTests) {
    std::cout << "|  - Running TableTest...\n";
    char_t index;
    uintptr_t buffer[128];
    printf ("| &buffer[0]:%p &buffer[127]:%p\n", &buffer[0], &buffer[127]);
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
        
    index = TableAdd (table, "Bin");
    CHECK_EQUAL (2, index)
    index = TableFind (table, "D");
    CHECK_EQUAL (0, index)
    index = TableFind (table, "C");
    CHECK_EQUAL (1, index)
    index = TableFind (table, "Bin");
    CHECK_EQUAL (2, index)
        
    index = TableAdd (table, "A");
    CHECK_EQUAL (3, index)
    index = TableFind (table, "D");
    CHECK_EQUAL (0, index)
    index = TableFind (table, "C");
    CHECK_EQUAL (1, index)
    index = TableFind (table, "Bin");
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
    //system ("PAUSE");
}

TEST (SCRIPT_TESTS, ReadWriteTests) {
    enum {
        kBufferSize = 256,
        kElementsBuffer = kBufferSize / sizeof (uintptr_t)
    };

    uintptr_t buffer[kElementsBuffer + 4];  //< Something isn't aligned right.
    //< It works right with an extra 4 elements but no less. Am I writing
    //< something to the end of the buffer???

    PrintLineBreak ("|  - Running ReadWriteTests...", 5);
    std::cout << " kBufferSize: "     << kBufferSize 
              << " kElementsBuffer: " << kElementsBuffer;

    void* args[19];

    const char expected_string1[] = "1234\0",
               expected_string2[] = "5678\0";

    char found_string1[6],
         found_string2[6];

    memset (found_string1, ' ', 6);
    memset (found_string2, ' ', 6);

    printf ("\n| buffer_start:%p buffer_stop:%p\n", &buffer[0], 
            &buffer[kBufferSize - 1]);
    printf ("\n| &expected_string1[0]:%p &expected_string2[0]:%p\n", 
            &expected_string1[0], &expected_string2[0]);

    Bout* bout = BoutInit (buffer, kBufferSize);
    
    CHECK_EQUAL (0, BoutWrite (bout, BSeq<2, STR, 6, STR, 6> (),
                               Args (args, expected_string1,
                                     expected_string2)))
    void** test = Args (args, found_string1, found_string2);
    printf ("\n| texpected_string1_start:%p texpected_string2_start:%p\n",
            &test[0], &test[1]);
    
    CHECK_EQUAL (0, BoutRead (bout, BSeq<2, STR, 5, STR, 5> (),
                              Args (args, found_string1, found_string2)))

    std::cout << "\nExpected 1: " << expected_string1
              << " Found 1: "     << found_string1
              << "\nExpected 2: " << expected_string2
              << " Found 2: "     << found_string2;

    STRCMP_EQUAL (expected_string1, found_string1)
    //BoutPrint (bout);
    STRCMP_EQUAL (expected_string2, found_string2)
    
    PrintLineBreak ("|  - Testing BOL/UI1/SI1...", 5);

    static const int8_t si1_p_expected = '+',
                        si1_n_expected = (int8_t)196;
    static const byte   ui1_expected = '0',
                        bol_expected = '?';
    int8_t si1_p_found,
           si1_n_found,
           bol_found;
    byte   ui1_found;

    CHECK_EQUAL (0, BoutWrite (bout, BSeq<4, SI1, SI1, UI1, BOL> (),
                               Args (args, &si1_p_expected, &si1_n_expected, 
                                     &ui1_expected, &bol_expected)))
    CHECK_EQUAL (0, BoutRead (bout, BSeq<4, SI1, SI1, UI1, BOL> (),
                              Args (args, &si1_p_found, &si1_n_found,
                                    &ui1_found, &bol_found)))
    CHECK_EQUAL (si1_p_expected, si1_p_found)
    CHECK_EQUAL (si1_n_expected, si1_n_found)
    CHECK_EQUAL (ui1_expected, ui1_found)
    CHECK_EQUAL (bol_expected, bol_found)

    PrintLineBreak ("|  - Testing UI2/SI2/HLF...", 5);

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

    CHECK_EQUAL (0, BoutWrite (bout, BSeq<4, SI2, SI2, UI2, HLF> (),
                           Args (args, &si2_p_expected, &si2_n_expected, 
                                 &ui2_expected, &hlf_expected)))
    CHECK_EQUAL (0, BoutRead (bout, BSeq<4, SI2, SI2, UI2, HLF> (),
                          Args (args, &si2_p_found, &si2_n_found, &ui2_found, 
                                &hlf_found)))
    CHECK_EQUAL (si2_p_expected, si2_p_found)
    CHECK_EQUAL (si2_n_expected, si2_n_found)
    CHECK_EQUAL (ui2_expected, ui2_found)
    CHECK_EQUAL (hlf_expected, hlf_found)

    PrintLineBreak ("|  - Testing UI4/SI4/FLT/TMS...", 5);

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

    CHECK_EQUAL (0, BoutWrite (bout, BSeq<5, SI4, SI4, UI4, FLT, TMS> (),
                               Args (args, &si4_p_expected, &si4_n_expected,
                                     &ui4_expected, &flt_expected, 
                                     &tms_expected)))
    CHECK_EQUAL (0, BoutRead (bout, BSeq<5, SI4, SI4, UI4, FLT, TMS> (),
                              Args (args, &si4_p_found, &si4_n_found,
                                    &ui4_found, &flt_found, &tms_found)))
    CHECK_EQUAL (si4_p_expected, si4_p_found)
    CHECK_EQUAL (si4_n_expected, si4_n_found)
    CHECK_EQUAL (ui4_expected, ui4_found)
    CHECK_EQUAL (flt_expected, flt_found)
    
    PrintLineBreak ("|  - Testing TMU/UI8/SI1/DBL...\n", 5);

    static const time_t tmu_expected = 0xE7;
    static const int64_t si8_p_expected = '+',
        si8_n_expected = -(2*1024*1024);
    static const uint64_t ui8_expected = '8';
    static const uint64_t dbl_value = '.';
    static const double dbl_expected = 1.0;

    timestamp_t tmu_found = 0;
    int64_t si8_p_found = 0,
        si8_n_found = 0;
    uint64_t ui8_found = 0;
    double dbl_found = 0.0;

    bout = BoutInit (buffer, kBufferSize);

    CHECK_EQUAL (0, BoutWrite (bout, BSeq<5, TMU, SI8, SI8, UI8, DBL> (),
           Args (args, &tmu_expected, &si8_p_expected, &si8_n_expected,
                 &ui8_expected, &dbl_expected)))

    CHECK_EQUAL (0, BoutRead (bout, BSeq<5, TMU, SI8, SI8, UI8, DBL> (),
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
    
    PrintLineBreak ("  - Testing SV2...\n", 5);

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
    CHECK_EQUAL (0, BoutWrite (bout, BSeq<7, SV2, SV2, SV2, SV2, SV2, SV2, 
                                            SV2> (),
                           Args (args, &sv2_expected[0], &sv2_expected[1],
                                 &sv2_expected[2], &sv2_expected[3],
                                 &sv2_expected[4], &sv2_expected[5],
                                 &sv2_expected[6])))
    CHECK_EQUAL (0, BoutRead (bout, BSeq<7, SV2, SV2, SV2, SV2, SV2, SV2, 
                                           SV2>(),
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

    PrintLineBreak ("|  - Testing UV2...\n", 5);

    static const uint16_t uv2_expected[] = { 0, 1, 1 << 7, 1 << 14 };

    uint16_t uv2_found[4];
    CHECK_EQUAL (0, BoutWrite (bout, BSeq<4, UV2, UV2, UV2, UV2> (),
                           Args (args, &uv2_expected[0], &uv2_expected[1],
                                 &uv2_expected[2], &uv2_expected[3])))
    CHECK_EQUAL (0, BoutRead (bout, BSeq<4, UV2, UV2, UV2, UV2> (),
                          Args (args, &uv2_found[0], &uv2_found[1],
                                &uv2_found[2], &uv2_found[3])))
    CHECK_EQUAL (uv2_expected[0], uv2_found[0])
    CHECK_EQUAL (uv2_expected[1], uv2_found[1])
    CHECK_EQUAL (uv2_expected[2], uv2_found[2])
    CHECK_EQUAL (uv2_expected[3], uv2_found[3])
    
    PrintLineBreak ("|  - Testing SV4...\n", 5);

    static const int32_t sv4_expected[] = { 0, 1, -1, 1 << 7, -(1 << 7), 
                                            1 << 14, -(1 << 14),
                                            1 << 21, -(1 << 21),
                                            1 << 28, -(1 << 28) };
    int32_t sv4_found[11];
    CHECK_EQUAL (0, BoutWrite (bout, BSeq<11, SV4, SV4, SV4, SV4, SV4, SV4, 
                                            SV4, SV4, SV4, SV4, SV4> (),
                           Args (args, &sv4_expected[0], &sv4_expected[1],
                                 &sv4_expected[2], &sv4_expected[3],
                                 &sv4_expected[4], &sv4_expected[5],
                                 &sv4_expected[6], &sv4_expected[7],
                                 &sv4_expected[8], &sv4_expected[9],
                                 &sv4_expected[10])))
    CHECK_EQUAL (0, BoutRead (bout, BSeq<11, SV4, SV4, SV4, SV4, SV4, SV4, 
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

    PrintLineBreak ("|  - Testing UV4...\n", 5);

    static const uint32_t uv4_expected[] = { 
        0, 1, 1 << 7, 1 << 14, 1 << 21, 1 << 28 };
    uint32_t uv4_found[6];
    CHECK_EQUAL (0, BoutWrite (bout, BSeq<6, UV4, UV4, UV4, UV4, UV4, UV4> (),
                           Args (args, &uv4_expected[0], &uv4_expected[1],
                                 &uv4_expected[2], &uv4_expected[3],
                                 &uv4_expected[4], &uv4_expected[5])))
    CHECK_EQUAL (0, BoutRead (bout, BSeq<6, UV4, UV4, UV4, UV4, UV4, UV4> (),
                            Args (args, &uv4_found[0], &uv4_found[1],
                                  &uv4_found[2], &uv4_found[3],
                                  &uv4_found[4], &uv4_found[5])))
    CHECK_EQUAL (uv4_expected[0], uv4_found[0])
    CHECK_EQUAL (uv4_expected[1], uv4_found[1])
    CHECK_EQUAL (uv4_expected[2], uv4_found[2])
    CHECK_EQUAL (uv4_expected[3], uv4_found[3])
    CHECK_EQUAL (uv4_expected[4], uv4_found[4])
    CHECK_EQUAL (uv4_expected[5], uv4_found[5])
    
    PrintLineBreak ("|  - Testing SV8...\n", 5);

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
    CHECK_EQUAL (0, BoutWrite (bout, BSeq<19, SV8, SV8, SV8, SV8, SV8, SV8,
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
    CHECK_EQUAL (0, BoutRead (bout, BSeq<19, SV8, SV8, SV8, SV8, SV8, SV8, 
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

        PrintLineBreak ("|  - Testing UV8...\n", 5);

    static const uint64_t uv8_expected[] = {
        0, 1, 1 << 7, 1 << 14, 1 << 21,
        1 << 28, ((uint64_t)1) << 35, 
        ((uint64_t)1) << 42, 
        ((uint64_t)1) << 49, 
        ((uint64_t)1) << 56
    };
    uint64_t uv8_found[10];
    CHECK_EQUAL (0, BoutWrite (bout, BSeq<10, UV8, UV8, UV8, UV8, UV8, UV8, 
                                            UV8, UV8, UV8, UV8> (),
                               Args (args, &uv8_expected[0], &uv8_expected[1],
                                     &uv8_expected[2], &uv8_expected[3],
                                     &uv8_expected[4], &uv8_expected[5],
                                     &uv8_expected[6], &uv8_expected[7],
                                     &uv8_expected[8], &uv8_expected[9],
                                     &uv8_expected[10], &uv8_expected[11],
                                     &uv8_expected[12], &uv8_expected[13])))
    CHECK_EQUAL (0, BoutRead (bout, BSeq<10, UV8, UV8, UV8, UV8, UV8, UV8, 
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

TEST (SCRIPT_TESTS, UtilsTests) {
    std::cout << "  - Running HexTest...\n";
    for (int i = 0; i < 16; ++i) {
        int value = ToByte (NibbleToLowerCaseHex (i));
        CHECK_EQUAL (i, value)
            value = ToByte (NibbleToUpperCaseHex (i));
        CHECK_EQUAL (i, value)
    }

    for (int i = 0; i < 256; ++i) {
        int value = ToByte (ToLowerCaseHex (i));
        CHECK_EQUAL (i, value)
            value = ToByte (ToUpperCaseHex (i));
        CHECK_EQUAL (i, value)
    }

    std::cout << "\nTesting string utils...\n";

    const char* strings[] = {
        "Testing",
        "Texting",
        "Testing@",
        "Texting@",
    };

    CHECK (!StringEquals (strings[0], strings[1]))
    CHECK (!StringEquals (strings[0], strings[3]))
    CHECK ( StringEquals (strings[0], strings[0]))
    CHECK (!StringEquals (strings[2], strings[3], '@'))
    CHECK ( StringEquals (strings[2], strings[2], '@'))

    // @todo Add Token module and TokenEquals here.
}

TEST (SCRIPT_TESTS, OperationTests) {
    enum {
        kBufferSize = 1024,
        kBufferWords = kBufferSize / sizeof (uintptr_t),
        kStackHeight = 8,
    };
    uintptr_t buffer[kBufferWords];
    printf ("|  - Running OperationTests in address ranges: [0x%p:0x%p]\n",
            &buffer[0], &buffer[kBufferWords-1]);

    This a;

    Expression* expr = ExpressionInit (buffer, kBufferSize, kStackHeight, &a);
    ExpressionPrint (expr);

    std::cout << "|    Testing Root (@see \"a.h\")...\n";

    void* args[4];
    uint8_t io_number_ = 98; //< ASCII:'b'
    char  io_string_[Parent::kStringBufferSize];//< Example string.
    Bin * bin  = ExpressionBin  (expr);
    Bout* bout = ExpressionBout (expr);
    
    const Operation* result;
    result = BoutWrite (bout,
                        BSeq<4, ADR, UI1, STR, Parent::kStringBufferSize, 
                                ADR> (),
                        Args (args, Address <BEL, ACK, 'a', 'a', 'a'> (), 
                              &io_number_, Const ("Test"), 
                              Address<DEL, CR> ()));
    BoutPrint (bout);
    CHECK (result == nullptr)

    ExpressionPrint (expr);

    Window window (bin, bout);
    // Bypass handshake for testing purposes.
    ExpressionScan (expr, &window);
    ExpressionPrint (expr);
    std::cout << "\n| Done with Expression tests.\n";
    system ("PAUSE");
}
