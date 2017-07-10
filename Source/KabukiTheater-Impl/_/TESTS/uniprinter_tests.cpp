/** The Chinese Room
    @version 0.9
    @file    /.../Source/KabukiTheater-Impl/_/TESTS/uniprinter_tests.cpp
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

#include <_/terminal.h>

using namespace _;

enum { 
    ErrorListSize       = 4,
    NumLoopIterations   = 3,      //< The number of times to test each type with random data.
    kSlotSize           = 128,
    StackSize           = 3,
};

// Test Device for multiple unit tests.
template<typename uint_t>
class Gadget: public Device
{
    public:

    void functionA () {}                //< Example dummy function A.
    void functionB () {}                //< Example dummy function B.

    // Remote procedure call using switch statement example; note the only RAM used is one pointer per parameter.
    const Member* Op (Rx* rx, Tx& tx, char index) override
    {
        void* args[2];

        switch (index)
        {
            case '?': {
                return RomMember<"ChineseRoomExample", 2,
                                 "Description of ChineseRoomExample."
                                 >;
            }
            case 'A': {
                static const Member m1 = { "FloatTests",
                    Params<2, FLT, STX, kStringBufferSize> (),
                    Params<2, FLT, STX> (),
                    "Description of functionA." };
                if (!io) return &m1;

                if (io->read (m1.rxHeader, Args (args, &io_number_, 
                                                 io_string_))) 
                    return ReadError ();

                functionA ();

                if (io->write (m1.txHeader, Args (args, &io_number_,
                                                  io_string_)))
                    return WriteError ();
                return &m1;
            }
            case 'B': {
                static const Member m2 = { "SignedIntegerTests",
                    Params<2, FLT, STX, kStringBufferSize> (),
                    Params<2, FLT, STX> (),
                    "Description of functionB." };

                if (!io) return &m2;

                if (io->read (m2.rxHeader, Args (args, &io_number_, 
                                                 io_string_))) 
                    return ReadError ();

                functionB ();

                if (io->write (m2.txHeader, Args (args, &io_number_, 
                                                  io_string_))) 
                    return writeError ();
                return &m2;
            }
        }
        return nullptr;
    }

    private:

    //NONCOPYABLE (Device)

        enum {
        kStringBufferSize = 16              //< Example string buffer size.
    };

    float io_number_;                       //< Example variable.
    char io_string_[kStringBufferSize];     //< Example string.
};

TEST_GROUP (UniprinterTests)
{
    void setup ()
    {
        // Init stuff
    }

    void teardown ()
    {
        printf ("\n\nTerminalTests completed.\n\n");
        system ("PAUSE");
    }

};

TEST (UniprinterTests, FirstTest)
{
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