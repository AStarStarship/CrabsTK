/** The Chinese Room
    @version 0.9
    @file    /.../Source/Library/ChineseRoom-Tests/TerminalAndMirrorTests.cpp
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright 2016(C) Cale McCollough <cale.mccollough@gmail.com>

                          All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
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

#include <ChineseRoom/ASCII.h>
#include <ChineseRoom/Mirror.h>
#include <ChineseRoom/args.h>

using namespace _;

enum { 
    ErrorListSize       = 4,
    NumLoopIterations   = 3,      //< The number of times to test each type with random data.
    TerminalSize        = 128,
    StackSize           = 3,
};

class Device: public IDevice
//< Test IDevice for multiple unit tests.
{
    public:

    void functionA() {}                //< Example dummy function A.
    void functionB() {}                //< Example dummy function B.

    const Member* op(Terminal* io, byte index) override
        //< Remote proceedure call using switch statement example; note the only RAM used is one pointer per parameter.
    {
        void* arg[2];

        switch(index)
        {
            case '?': {
                static const Member m0 = { "ChineseRoomExample", setNumMembers(2), nullptr, 
                    "Description of ChineseRoomExample." };
                // 63 is ASCII '?'
                return &m0;
            }
            case 'A': {
                static const Member m1 = { "FloatTests",            //< The key of the member.
                    Params<2, FLT, STR, StringBufferSize>::Header,  //< The rx header with buffer size for string.
                    Params<2, FLT, STR>::Header,                    //< Note: There is no buffer size for tx values!
                    "Description of functionA." };
                if(!io) return &m1;

                if(io->read(m1.rxHeader, args (arg, &ioNumber, ioString))) return readError();

                functionA();

                if(io->write(m1.txHeader, args (arg, &ioNumber, ioString))) return writeError();
                return &m1;
            }
            case 'B': {
                static const Member m2 = { "SignedIntegerTests",    //< Duplicate function for example purposes.
                    Params<2, FLT, STR, StringBufferSize>::Header,  //< Note the C++11 variadic template ensures that 
                    Params<2, FLT, STR>::Header,                    //< only one copy of the header is in ROM! :-)
                    "Description of functionB." };

                if(!io) return &m2;

                if(io->read(m2.rxHeader, args (arg, &ioNumber, ioString))) return readError();

                functionB();

                if(io->write(m2.txHeader, args (arg, &ioNumber, ioString))) return writeError();
                return &m2;
            }
        }
        return nullptr;
    }

    private:

    _Noncopyable_(Device)

        enum {
        StringBufferSize = 16           //< Example string buffer size.
    };

    float ioNumber;                     //< Example varaible.
    char ioString[StringBufferSize];    //< Example string.
};

TEST_GROUP(TerminalTests)
{
    void setup()
    {
        // Init stuff
    }

    void teardown()
    {
        printf("\n\nTerminalTests completed.\n\n");
        system("PAUSE");
    }

};

TEST(TerminalTests, FirstTest)
{
    //const char* result;

    /// Testing methology:
    /// The plan is to test all the primitive Print and Scan functions in one 
    /// test using a single header with randomly generated data, then running 
    /// the test multiple times.
    /*
    static const uint_t TestA[] = { 1, STX, 100,  };

    byte buffer[TerminalSize];
    
    Terminal t(buffer, TerminalSize);

    for(int i = 0; i < NumLoopIterations; ++i)
    {
        
    }
    
    FAIL("Pass me!");
    */
}

TEST(TerminalTests, SignedIntegerTests)
{
    void* arg[5];

    static const int8_t  testSI1Data[] = { -1, 0, 1};
    static const int16_t testSI2Data[] = { ~0, -1, 0, 1,(~0) >> 1 };
    static const int32_t testSI4Data[] = { ~0, -1, 0, 1,(~0) >> 1 };
    static const int64_t testSI8Data[] = { ~0, -1, 0, 1,(~0) >> 1 };

    int8_t  testSI1Array[3];
    int16_t testSI2Array[5];
    int32_t testSI4Array[5];
    int16_t testSI8Array[5];

    Terminal* t = createTerminal(TerminalSize, StackSize);

    t->write(Params<3, SI1, SI1, SI1>::Header, args (arg, &testSI1Data[0], &testSI1Data[1], &testSI1Data[2]));
    t->write(Params<5, SI2, SI2, SI2, SI2, SI2>::Header, args (arg, &testSI2Data[0], &testSI2Data[1], &testSI2Data[2], 
        &testSI2Data[3], &testSI2Data[4]));
    t->write(Params<5, SI4, SI4, SI4, SI4, SI4>::Header, args (arg, &testSI4Data[0], &testSI4Data[1], &testSI4Data[2], 
        &testSI4Data[3], &testSI4Data[4]));
    t->write(Params<5, SI8, SI8, SI8, SI8, SI8>::Header, args (arg, &testSI8Data[0], &testSI8Data[1], &testSI8Data[2], 
        &testSI8Data[3], &testSI8Data[4]));

    t->read(Params<5, SI8, SI8, SI8, SI8, SI8>::Header, args (arg, &testSI8Array[0], &testSI8Array[1], &testSI8Array[2], 
        &testSI8Array[3], &testSI8Array[4]));
    for(int i = 0; i < 5; ++i)
        CHECK_EQUAL(testSI8Data[i], testSI8Array[i]);

    t->read(Params<5, SI4, SI4, SI4, SI4, SI4>::Header, args (arg, &testSI4Array[0], &testSI4Array[1], &testSI4Array[2], 
        &testSI4Array[3],     &testSI4Array[4]));
    for(int i = 0; i < 5; ++i)
        CHECK_EQUAL(testSI4Data[i], testSI4Array[i]);

    t->read(Params<5, SI2, SI2, SI2, SI2, SI2>::Header, args (arg, &testSI2Array[0], &testSI2Array[1], &testSI2Array[2], 
        &testSI2Array[3], &testSI2Array[4]));
    for(int i = 0; i < 5; ++i)
        CHECK_EQUAL(testSI2Data[i], testSI2Array[i]);

    t->read(Params<3, SI1, SI1, SI1>::Header, args (arg, &testSI1Array[0], &testSI1Array[1], &testSI1Array[2]));
    for(int i = 0; i < 3; ++i)
        CHECK_EQUAL(testSI1Data[i], testSI1Array[i]);

    destroy(t);

    printf("\nDone with IntegerTests\n");
}

TEST(TerminalTests, FloatingPointTests)
{
    void* arg[3];

    static const float  testFloatData[]  = { -1.0, 0.0, 1.0};
    static const double testDoubleData[] = { -1.0, 0.0, 1.0 };

    float  testFloatArray[3];
    double testDoubleArray[3];

    Terminal* t = createTerminal(TerminalSize, StackSize);

    t->write(Params<3, FLT, FLT, FLT>::Header, args (arg, &testFloatData[0],  &testFloatData[1],  &testFloatData[2]));
    t->write(Params<3, DBL, DBL, DBL>::Header, args (arg, &testDoubleData[0], &testDoubleData[1], &testDoubleData[2]));

    t->read(Params<3, FLT, FLT, FLT>::Header, args (arg, &testFloatArray[0], &testFloatArray[1], &testFloatArray[2]));
    for(int i = 0; i < 5; ++i)
        CHECK_EQUAL(testFloatData[i], testFloatArray[i]);

    t->read(Params<3, DBL, DBL, DBL>::Header, args (arg, &testDoubleArray[0], &testDoubleArray[1], &testDoubleArray[2]));
    for(int i = 0; i < 5; ++i)
        CHECK_EQUAL(testDoubleData[i], testDoubleArray[i]);

    destroy(t);

    printf("\nDone with FloatingPointTests\n");
}

TEST(TerminalTests, StringTests)
{
    void* arg[2];

    printParams(Params<2, STR, STR>::Header);

    const char testString1[] = "aaaaaaaa\0",
        testString2[] = "bbbbbbbb\0";

    printf ("\n&testring1: %p\n", testString1);

    char string1[TerminalSize],
        string2[TerminalSize];

    Terminal* t = createTerminal(TerminalSize, StackSize);

    ticket_t e;
    
    e = t->write(Params<2, STR, STR>::Header, args (arg, testString1, testString2));

    CHECK_EQUAL(0, e)

    e = t->read(Params<2, STR, STR>::Header, args (arg, string1, string2));

    CHECK_EQUAL(0, e)
    
    STRCMP_EQUAL(testString1, string1)
    STRCMP_EQUAL(testString2, string2)
    
    destroy(t);

    printf("\nDone with StringTests\n");
}
