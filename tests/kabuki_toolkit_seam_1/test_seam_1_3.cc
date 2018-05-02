/** Kabuki Toolkit
    @version 0.x
    @file    ~/tests/test_seam_1_3.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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
    std::cout << '\n' << message << '\n';\
    for (int i = 80; i > 80; --i) std::cout << '-';\
    std::cout << '\n';

#define PRINT_SLOT print << slot << Dump ();
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#define PRINT_HEADING(message)
#define PRINT_SLOT
#endif

using namespace _;

void TestSeam1_3 () {

    printf ("\n    Testing SEAM_1_3... ");

    enum {
        kBufferSize      = 2048,
        kBufferWords     = kBufferSize / sizeof (uintptr_t),
        kStackHeight     = 8,
        kPrintBufferSize = 4096,
    };

    Printer print;
    
    uintptr_t buffer[kBufferWords],
              unpacked_buffer[kBufferWords];

    PRINT_HEADING ("Creating test Slot...")

    Slot slot (buffer, kBufferWords);
    print << slot << Dump ();

    PRINT_HEADING ("Testing Expr...")
    This root;
    Expr* expr = ExprInit (buffer, kBufferSize, 4, 
                           &root, unpacked_buffer,
                           kBufferWords);
    BOut* bout = ExprBOut (expr);
    PRINT_SLOT;
    
    void        * args[19];
    const uint_t* params         = Params <4, ADR, STR, 32, FLT, SI4> ();
    const char    stx_expected[] = "Hello world!\0";
    const int     si4_expected   = 1;
    const float   flt_expected   = 1.0f;

    char  stx_found[64];
    int   si4_found;
    float flt_found;

    const Op* result = ExprResult (expr, params,
                                   Args (args, "C", &stx_expected,
                                         &si4_expected, &flt_expected));
    PRINTF ("\n\n Printing...\n\n")
    print << expr << Dump ();

    PRINTF ("\n\n Attempting to print Expr\n\n")
    result = ExprArgs (expr, params,
                       Args (args, &stx_found, &si4_found, &flt_found));
    CHECK_EQUAL (0, result)
    print << expr << Dump ();


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

    bout = BOutInit (buffer, kBufferSize);
    
    CHECK_EQUAL (0, BOutWrite (bout, Params<2, STR, 6, STR, 6> (),
                               Args (args, expected_string1,
                                     expected_string2)))
    void** test = Args (args, found_string1, found_string2);
    PRINTF ("\n texpected_string1_start:%p texpected_string2_start:%p\n",
            &test[0], &test[1]);
    
    CHECK_EQUAL (0, BOutRead (bout, Params<2, STR, 5, STR, 5> (),
                              Args (args, found_string1, found_string2)))

    PRINTF ("\nExpected 1:%s Found 1:%s\nExpected 2:%s Found 2:%s",
            expected_string1, found_string1, expected_string2,
            found_string2)

    STRCMP_EQUAL (expected_string1, found_string1)
    //Print (bout);
    STRCMP_EQUAL (expected_string2, found_string2)

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

    PRINT_HEADING ("Testing UVI...")
    
    PRINT_HEADING ("\n  - Testing SVI...\n")

    static const int32_t svi_expected[] = { 0, 1, -1, 1 << 7, -(1 << 7), 
                                            1 << 14, -(1 << 14),
                                            1 << 21, -(1 << 21),
                                            1 << 28, -(1 << 28) };
    int32_t svi_found[11];
    CHECK_EQUAL (0, BOutWrite (bout, Params<11, SVI, SVI, SVI, SVI, SVI, SVI, 
                                            SVI, SVI, SVI, SVI, SVI> (),
                           Args (args, &svi_expected[0], &svi_expected[1],
                                 &svi_expected[2], &svi_expected[3],
                                 &svi_expected[4], &svi_expected[5],
                                 &svi_expected[6], &svi_expected[7],
                                 &svi_expected[8], &svi_expected[9],
                                 &svi_expected[10])))
    CHECK_EQUAL (0, BOutRead (bout, Params<11, SVI, SVI, SVI, SVI, SVI, SVI, 
                                           SVI, SVI, SVI, SVI, SVI> (),
                          Args (args, &svi_found[0], &svi_found[1], 
                                &svi_found[ 2], &svi_found[3],
                                &svi_found[ 4], &svi_found[5],
                                &svi_found[ 6], &svi_found[7],
                                &svi_found[ 8], &svi_found[9],
                                &svi_found[10])))
    CHECK_EQUAL (svi_expected[ 0], svi_found[ 0])
    CHECK_EQUAL (svi_expected[ 1], svi_found[ 1])
    CHECK_EQUAL (svi_expected[ 2], svi_found[ 2])
    CHECK_EQUAL (svi_expected[ 3], svi_found[ 3])
    CHECK_EQUAL (svi_expected[ 4], svi_found[ 4])
    CHECK_EQUAL (svi_expected[ 5], svi_found[ 5])
    CHECK_EQUAL (svi_expected[ 6], svi_found[ 6])
    CHECK_EQUAL (svi_expected[ 7], svi_found[ 7])
    CHECK_EQUAL (svi_expected[ 8], svi_found[ 8])
    CHECK_EQUAL (svi_expected[ 9], svi_found[ 9])
    CHECK_EQUAL (svi_expected[10], svi_found[10])

    PRINT_HEADING ("Testing UVI...")

    static const uint32_t uvi_expected[] = { 
        0, 1, 1 << 7, 1 << 14, 1 << 21, 1 << 28 };
    uint32_t uvi_found[6];
    CHECK_EQUAL (0, BOutWrite (bout, Params<6, UVI, UVI, UVI, UVI, UVI, UVI> (),
                               Args (args, &uvi_expected[0], &uvi_expected[1],
                                     &uvi_expected[2], &uvi_expected[3],
                                     &uvi_expected[4], &uvi_expected[5])))
    CHECK_EQUAL (0, BOutRead (bout, Params<6, UVI, UVI, UVI, UVI, UVI, UVI> (),
                            Args (args, &uvi_found[0], &uvi_found[1],
                                  &uvi_found[2], &uvi_found[3],
                                  &uvi_found[4], &uvi_found[5])))
    CHECK_EQUAL (uvi_expected[0], uvi_found[0])
    CHECK_EQUAL (uvi_expected[1], uvi_found[1])
    CHECK_EQUAL (uvi_expected[2], uvi_found[2])
    CHECK_EQUAL (uvi_expected[3], uvi_found[3])
    CHECK_EQUAL (uvi_expected[4], uvi_found[4])
    CHECK_EQUAL (uvi_expected[5], uvi_found[5])
    
    uintptr_t buffer_b[kBufferWords];
    printf ("\n  - Running OpTests in address ranges: [0x%p:0x%p]\n",
            &buffer[0], &buffer[kBufferWords-1]);

    This a;
    expr = ExprInit (buffer, kBufferSize, kStackHeight, &a,
                     buffer_b, kBufferSize);
    print << expr;

    PRINTF ("\n    Testing Expr...\n")

    int io_number_ = 98; //< ASCII:'b'
    BIn * bin  = ExprBIn  (expr);
    bout = ExprBOut (expr);
    
    ExprRingBell (expr);
    ExprAckBack (expr);
    result = BOutWrite (bout,
                        Params<4, ADR, SVI, STR, Parent::kBufferSize, 
                                ADR> (),
                        Args (args, Address <'A', 'A', 'A'> (), 
                              &io_number_, "Test", 
                              Address<BS, CR> ()));
    
    CHECK (result == nullptr)

    print << bout;

    slot.Clear ();
    print << expr;

    //Printer slot (bin, bout);
    // Bypass handshake for testing purposes.

    ExprUnpack (expr);//, &slot);
    print << expr;
    PRINTF ("\n Done with Op tests.")

    PRINT_PAUSE ("\n\nDone Testing SEAM_1_3! ({:-)-+=<")
}
#undef PRINT_PAUSE
#undef PRINTF
#else
void TestSeam1_3 () {}
#endif      //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
