/** Kabuki Toolkit
    @version 0.x
    @file    ~/tests/test_seam_1_3.cc
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

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

#if MAJOR_SEAM == 1 && MINOR_SEAM == 3
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PRINT_PAUSE(message)\
    printf ("\n\n%s\n", message); system ("PAUSE");
#define PRINT_HEADING(message) \
    std::cout << '\n';\
    for (int i = 80; i > 80; --i) std::cout << '-';\
    std::cout << '\n' << message;\
    for (int i = 80; i > 80; --i) std::cout << '-';
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#define PRINT_HEADING(message)
#endif

using namespace _;

void TestSeam1_3 () {

    printf ("\n    Testing SEAM_1_3... ");

    PRINT_HEADING ("Testing Expr...")
    enum {
        kBufferSize  = 2048,
        kBufferWords = kBufferSize / sizeof (uintptr_t),
        kStackHeight = 8,
    };
    
    uintptr_t buffer[kBufferWords],
              unpacked_buffer[kBufferWords];
    Slot slot;
    SlotInit (buffer, kBufferWords);

    slot << 'a' << "b" << "cd" << (int8_t)1 << (uint8_t)2 << (int16_t)3
         << (uint16_t)4 << (int32_t)5 << (uint32_t)6 << (int64_t)7 
         << (uint64_t)8;

    Print (slot);

    This root;
    Expr* expr = ExprInit (buffer, kBufferSize, 4, 
                           &root, unpacked_buffer,
                           kBufferWords);
    PrintExpr (expr, slot);
    
    void        * args[19];
    const uint_t* params         = Bsq <4, ADR, STR, 32, FLT, SI4> ();
    const char    stx_expected[] = "Hello world!\0";
    const int     si4_expected   = 1;
    const float   flt_expected   = 1.0f;

    char  stx_found[64];
    int   si4_found;
    float flt_found;

    const Op* result = ExprResult (expr, params,
                                   Args (args, "C", &stx_expected,
                                         &si4_expected, &flt_expected));
    PRINTF ("\n Attempting to print Expr 0x%p", ExprBOut (expr))
    //PrintExpr (expr, text) << Print ();
    result = ExprArgs (expr, params,
                       Args (args, &stx_found, &si4_found, &flt_found));
    CHECK_EQUAL (0, result);

    //< It works right with an extra 4 elements but no less. Am I writing
    //< something to the end of the buffer???

    PRINTF ("\n\n  - Running ReadWriteTests...\nkBufferSize: %i "
            "kBufferWords: %i", kBufferSize, kBufferWords)

    const char expected_string1[] = "1234\0",
               expected_string2[] = "5678\0";

    char found_string1[6],
         found_string2[6];

    memset (found_string1, ' ', 6);
    memset (found_string2, ' ', 6);

    PRINTF ("\n buffer_start:%p buffer_stop:%p\n", &buffer[0], 
            &buffer[kBufferSize - 1]);
    PRINTF ("\n &expected_string1[0]:%p &expected_string2[0]:%p\n", 
            &expected_string1[0], &expected_string2[0]);

    BOut* bout = BOutInit (buffer, kBufferSize);
    
    CHECK_EQUAL (0, BOutWrite (bout, Bsq<2, STR, 6, STR, 6> (),
                               Args (args, expected_string1,
                                     expected_string2)))
    void** test = Args (args, found_string1, found_string2);
    PRINTF ("\n texpected_string1_start:%p texpected_string2_start:%p\n",
            &test[0], &test[1]);
    
    CHECK_EQUAL (0, BOutRead (bout, Bsq<2, STR, 5, STR, 5> (),
                              Args (args, found_string1, found_string2)))

    PRINTF ("\nExpected 1:%s Found 1:%s\nExpected 2:%s Found 2:%s",
            expected_string1, found_string1, expected_string2,
            found_string2)

    STRCMP_EQUAL (expected_string1, found_string1)
    //BOutPrint (bout);
    STRCMP_EQUAL (expected_string2, found_string2)
    
    PRINT_HEADING ("Testing BOL/UI1/SI1...")

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

    PRINT_HEADING ("Testing UI2/SI2/HLF...")

    static const int16_t si2_p_expected = '+',
                         si2_n_expected = (int16_t)(0xFF00 | '-');
    static const uint16_t ui2_expected = '2',
        hlf_expected = 227;

    int16_t  si2_p_found,
             si2_n_found;
    uint16_t ui2_found,
             hlf_found;

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

    PRINT_HEADING ("Testing UI4/SI4/FLT/TMS...")

    static const int32_t  si4_p_expected = '+',
                          si4_n_expected = (int32_t)(0xFFFFFF00 | '-');
    static const uint32_t ui4_expected = '4';
    static const uint32_t flt_value = '.';
    static const float    flt_expected2 = *(float*)&flt_value;
    static const time_t   tms_expected = 0xE7;

    int32_t               si4_p_found,
                          si4_n_found;
    uint32_t              ui4_found;
    time_t                tms_found;

    CHECK_EQUAL (0, BOutWrite (bout, Bsq<5, SI4, SI4, UI4, FLT, TMS> (),
                               Args (args, &si4_p_expected, &si4_n_expected,
                                     &ui4_expected, &flt_expected2, 
                                     &tms_expected)))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<5, SI4, SI4, UI4, FLT, TMS> (),
                              Args (args, &si4_p_found, &si4_n_found,
                                    &ui4_found, &flt_found, &tms_found)))
    CHECK_EQUAL (si4_p_expected, si4_p_found)
    CHECK_EQUAL (si4_n_expected, si4_n_found)
    CHECK_EQUAL (ui4_expected, ui4_found)
    CHECK_EQUAL (flt_expected2, flt_found)
    
    PRINT_HEADING ("Testing TMU/UI8/SI1/DBL...\n")

    static const time_t   tmu_expected = 0xE7;
    static const int64_t  si8_p_expected = '+',
                          si8_n_expected = -(2*1024*1024);
    static const uint64_t ui8_expected = '8';
    static const uint64_t dbl_value = '.';
    static const double   dbl_expected = 1.0;

    time_us_t tmu_found   = 0;
    int64_t   si8_p_found = 0,
              si8_n_found = 0;
    uint64_t  ui8_found   = 0;
    double    dbl_found   = 0.0;

    bout = BOutInit (buffer, kBufferSize);

    CHECK_EQUAL (0, BOutWrite (bout, Bsq<5, TMU, SI8, SI8, UI8, DBL> (),
           Args (args, &tmu_expected, &si8_p_expected, &si8_n_expected,
                 &ui8_expected, &dbl_expected)))

    CHECK_EQUAL (0, BOutRead (bout, Bsq<5, TMU, SI8, SI8, UI8, DBL> (),
                              Args (args, &tmu_found, &si8_p_found, 
                                    &si8_n_found, &ui8_found, &dbl_found)))

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

    PRINT_HEADING ("Testing PackSVI and UnpackSVI...")
    
    CHECK_EQUAL (1, UnpackSVI (PackSVI ((int32_t)1)))
    printf ("Found: 0x%x\n", 
            UnpackSVI (PackSVI (~0)));

    CHECK_EQUAL (1 << 30, UnpackSVI (PackSVI (1 << 30)))
    CHECK_EQUAL (~0, UnpackSVI (PackSVI (~0)))
        
    uint64_t ui8 = (uint64_t)1 << 62;
    int64_t  si8 = (int64_t)ui8;
    CHECK_EQUAL (si8, UnpackSV8 (PackSV8 (si8)))

    ui8 = ~(uint64_t)0;
    si8 = (int64_t)ui8;
    CHECK_EQUAL (si8, UnpackSV8 (PackSV8 (si8)))
    
    PRINT_HEADING ("Testing SVI...\n")

    static const int svi_expected[] = {
        0,
        1,
        -1,
        1 << 7,
        -(1 << 7),
        1 << 14,
        -(1 << 14)
    };

    int16_t svi_found[7];
    CHECK_EQUAL (0, BOutWrite (bout, Bsq<7, SVI, SVI, SVI, SVI, SVI, SVI, 
                                            SVI> (),
                           Args (args, &svi_expected[0], &svi_expected[1],
                                 &svi_expected[2], &svi_expected[3],
                                 &svi_expected[4], &svi_expected[5],
                                 &svi_expected[6])))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<7, SVI, SVI, SVI, SVI, SVI, SVI, 
                                           SVI>(),
                          Args (args, &svi_found[0], &svi_found[1], 
                                &svi_found[2], &svi_found[3], 
                                &svi_found[4], &svi_found[5],
                                &svi_found[6])))
    CHECK_EQUAL (svi_expected[0], svi_found[0])
    CHECK_EQUAL (svi_expected[1], svi_found[1])
    CHECK_EQUAL (svi_expected[2], svi_found[2])
    CHECK_EQUAL (svi_expected[3], svi_found[3])
    CHECK_EQUAL (svi_expected[4], svi_found[4])
    CHECK_EQUAL (svi_expected[5], svi_found[5])
    CHECK_EQUAL (svi_expected[6], svi_found[6])

    PRINT_HEADING ("Testing UVI...")

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
    
    PRINT_HEADING ("\n  - Testing SVI...\n")

    static const int32_t sv4_expected[] = { 0, 1, -1, 1 << 7, -(1 << 7), 
                                            1 << 14, -(1 << 14),
                                            1 << 21, -(1 << 21),
                                            1 << 28, -(1 << 28) };
    int32_t sv4_found[11];
    CHECK_EQUAL (0, BOutWrite (bout, Bsq<11, SVI, SVI, SVI, SVI, SVI, SVI, 
                                            SVI, SVI, SVI, SVI, SVI> (),
                           Args (args, &sv4_expected[0], &sv4_expected[1],
                                 &sv4_expected[2], &sv4_expected[3],
                                 &sv4_expected[4], &sv4_expected[5],
                                 &sv4_expected[6], &sv4_expected[7],
                                 &sv4_expected[8], &sv4_expected[9],
                                 &sv4_expected[10])))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<11, SVI, SVI, SVI, SVI, SVI, SVI, 
                                           SVI, SVI, SVI, SVI, SVI> (),
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

    PRINT_HEADING ("Testing UVI...")

    static const uint32_t uv4_expected[] = { 
        0, 1, 1 << 7, 1 << 14, 1 << 21, 1 << 28 };
    uint32_t uv4_found[6];
    CHECK_EQUAL (0, BOutWrite (bout, Bsq<6, UVI, UVI, UVI, UVI, UVI, UVI> (),
                               Args (args, &uv4_expected[0], &uv4_expected[1],
                                     &uv4_expected[2], &uv4_expected[3],
                                     &uv4_expected[4], &uv4_expected[5])))
    CHECK_EQUAL (0, BOutRead (bout, Bsq<6, UVI, UVI, UVI, UVI, UVI, UVI> (),
                            Args (args, &uv4_found[0], &uv4_found[1],
                                  &uv4_found[2], &uv4_found[3],
                                  &uv4_found[4], &uv4_found[5])))
    CHECK_EQUAL (uv4_expected[0], uv4_found[0])
    CHECK_EQUAL (uv4_expected[1], uv4_found[1])
    CHECK_EQUAL (uv4_expected[2], uv4_found[2])
    CHECK_EQUAL (uv4_expected[3], uv4_found[3])
    CHECK_EQUAL (uv4_expected[4], uv4_found[4])
    CHECK_EQUAL (uv4_expected[5], uv4_found[5])
    
    PRINT_HEADING ("Testing SV8...")

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

     PRINT_HEADING ("Testing UV8...")

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
    
    uintptr_t buffer_b[kBufferWords];
    printf ("\n  - Running OpTests in address ranges: [0x%p:0x%p]\n",
            &buffer[0], &buffer[kBufferWords-1]);

    This a;
    expr = ExprInit (buffer, kBufferSize, kStackHeight, &a,
                     buffer_b, kBufferSize);
    PrintExpr (expr, slot);

    PRINTF ("\n    Testing Expr...\n")

    uint8_t io_number_ = 98; //< ASCII:'b'
    BIn * bin  = ExprBIn  (expr);
    bout = ExprBOut (expr);
    
    ExprRingBell (expr);
    ExprAckBack (expr);
    result = BOutWrite (bout,
                        Bsq<4, ADR, UI1, STR, Parent::kTextBufferSize, 
                                ADR> (),
                        Args (args, Address <'A', 'A', 'A'> (), 
                              &io_number_, "Test", 
                              Address<BS, CR> ()));
    
    CHECK (result == nullptr)

    enum { kCharCount = 2048 };
    char text[kCharCount + 1];
    BOutPrint (bout, text, text + kCharCount);

    SlotClear (slot);
    PrintExpr (expr, slot);

    //Slot slot (bin, bout);
    // Bypass handshake for testing purposes.

    ExprUnpack (expr);//, &slot);
    PrintExpr (expr, slot);
    PRINTF ("\n Done with Op tests.")

    PRINT_PAUSE ("\n\nDone Testing SEAM_1_3! ({:-)-+=<")
}
#undef PRINT_PAUSE
#undef PRINTF
#else
void TestSeam1_3 () {}
#endif      //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
