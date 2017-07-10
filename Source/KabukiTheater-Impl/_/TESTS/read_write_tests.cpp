/** The Chinese Room
    @version 0.9
    @file    /.../Source/KabukiTheater-Impl/_/TESTS/read_write_tests.cpp
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright 2017 (C) Cale McCollough <cale.mccollough@gmail.com>

                          All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

#include <_/args.h>
#include <_/rx.h>

using namespace _;

enum { 
    ErrorListSize       = 4,
    NumLoopIterations   = 3,      //< The number of times to test each type with random data.
    kSlotSize           = 128,
    StackSize           = 3,
};

TEST_GROUP (ReadWriteTests)
{
    void setup () {
        // Init stuff
    }

    void teardown () {
        printf ("\n\nTerminalTests completed.\n\n");
        system ("PAUSE");
    }

};

TEST (ReadWriteTests, FirstTest) {
    //const char* result;

    /// Testing mythology:
    /// The plan is to test all the primitive Print and Scan functions in one 
    /// test using a single header with randomly generated data, then running 
    /// the test multiple times.
    //static const uint_t TestA[] = { 1, STX, 100,  };

    //byte buffer[kSlotSize];
    
    //Terminal t (buffer, kSlotSize);

    //for (int i = 0; i < NumLoopIterations; ++i)
    //{
        
    //}
    
    //FAIL ("Pass me!");
}

TEST (ReadWriteTests, SignedIntegerTests) {

    printf ("\nRunning SignedIntegerTests...\n");

    void* args[5];
    static const int8_t  si1_expected[] = { -1, 0, 1};
    static const int16_t si2_expected[] = { ~0, -1, 0, 1, (~0) >> 1 };
    static const int32_t si4_expected[] = { ~0, -1, 0, 1, (~0) >> 1 };
    static const int64_t si8_expected[] = { ~0, -1, 0, 1, (~0) >> 1 };

    int8_t  si1_found[3];
    int16_t si2_found[5];
    int32_t si4_found[5];
    int16_t SI8_found[5];

    Tx* tx = ConstructTx (kSlotSize);

    Write (tx, Params<3, SI1, SI1, SI1> (), 
           Args (args, &si1_expected[0], &si1_expected[1], &si1_expected[2]));
    Write (tx, Params<5, SI2, SI2, SI2, SI2, SI2> (), 
           Args (args, &si2_expected[0], &si2_expected[1], &si2_expected[2],
                 &si2_expected[3], &si2_expected[4]));
    Write (tx, Params<5, SI4, SI4, SI4, SI4, SI4> (), 
           Args (args, &si4_expected[0], &si4_expected[1], &si4_expected[2],
                 &si4_expected[3], &si4_expected[4]));
    Write (tx, Params<5, SI8, SI8, SI8, SI8, SI8> (), 
           Args (args, &si8_expected[0], &si8_expected[1], &si8_expected[2],
                 &si8_expected[3], &si8_expected[4]));

    Read (tx, Params<5, SI8, SI8, SI8, SI8, SI8> (), 
          Args (args, &SI8_found[0], &SI8_found[1], &SI8_found[2],
        &SI8_found[3], &SI8_found[4]));
    for (int i = 0; i < 5; ++i)
        CHECK_EQUAL (si8_expected[i], SI8_found[i]);

    Read (tx, Params<5, SI4, SI4, SI4, SI4, SI4> (), 
          Args (args, &si4_found[0], &si4_found[1], &si4_found[2],
        &si4_found[3],     &si4_found[4]));
    for (int i = 0; i < 5; ++i)
        CHECK_EQUAL (si4_expected[i], si4_found[i]);

    Read (tx, Params<5, SI2, SI2, SI2, SI2, SI2> (), 
          Args (args, &si2_found[0], &si2_found[1], &si2_found[2],
        &si2_found[3], &si2_found[4]));
    for (int i = 0; i < 5; ++i)
        CHECK_EQUAL (si2_expected[i], si2_found[i]);

    Read (tx, Params<3, SI1, SI1, SI1> (), 
          Args (args, &si1_found[0], &si1_found[1], &si1_found[2]));
    for (int i = 0; i < 3; ++i)
        CHECK_EQUAL (si1_expected[i], si1_found[i]);

    Destroy (tx);

    printf ("\nDone with IntegerTests\n");
}

TEST (ReadWriteTests, FloatingPointTests) {

    printf ("\nRunning FloatingPointTests...\n");

    void* args[3];

    static const float  testFloatData[]  = { -1.0, 0.0, 1.0};
    static const double testDoubleData[] = { -1.0, 0.0, 1.0 };

    float  testFloatArray[3];
    double testDoubleArray[3];

    Tx* tx = ConstructTx (255);

    Write (tx, Params<3, FLT, FLT, FLT> (), 
           Args (args, &testFloatData[0], &testFloatData[1], 
                 &testFloatData[2]));
    Write (tx, Params<3, DBL, DBL, DBL> (), 
           Args (args, &testDoubleData[0], &testDoubleData[1], 
                 &testDoubleData[2]));

    Read (tx, Params<3, FLT, FLT, FLT> (), 
          Args (args, &testFloatArray[0], &testFloatArray[1], 
                &testFloatArray[2]));
    for (int i = 0; i < 5; ++i)
        CHECK_EQUAL (testFloatData[i], testFloatArray[i]);

    Read (tx, Params<3, DBL, DBL, DBL> (), 
          Args (args, &testDoubleArray[0], &testDoubleArray[1], 
                &testDoubleArray[2]));
    for (int i = 0; i < 5; ++i)
        CHECK_EQUAL (testDoubleData[i], testDoubleArray[i]);

    Destroy (tx);

    printf ("\nDone with FloatingPointTests\n");
}

TEST (ReadWriteTests, StringTests) {

    printf ("\nRunning StringTests...\n");

    void* args[2];

    PrintParams (Params<2, STX, STX> ());

    const char testString1[] = "aaaaaaaa\0",
        testString2[] = "bbbbbbbb\0";

    printf ("\n&testring1: %p\n", testString1);

    char string1[kSlotSize],
        string2[kSlotSize];

    Tx* tx = ConstructTx (255);

    ticket_t e;
    
    e = Write (tx, Params<2, STX, STX> (), Args (args, testString1, testString2));

    CHECK_EQUAL (0, e)

    e = Read (tx, Params<2, STX, STX> (), Args (args, string1, string2));

    CHECK_EQUAL (0, e)
    
    STRCMP_EQUAL (testString1, string1)
    STRCMP_EQUAL (testString2, string2)
    
    Destroy (tx);

    printf ("\nDone with StringTests\n");
}
