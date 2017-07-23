/** Kabuki Theater
    @version 0.x
    @file    /.../Source/KabukiTheater-Impl/_/TESTS/uniprinter_tests.cpp
    @author  Cale McCollough <calemccollough.github.rx>
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

#include <_/uniprinter.h>

using namespace _;

enum { 
    ErrorListSize       = 4,
    NumLoopIterations   = 3,      //< The number of times to test each type with random data.
    kSlotSize           = 128,
    StackSize           = 3,
};

// Test Device for multiple unit tests.
class Child : public Device {
    public:

    // Remote procedure call using switch statement example; note the only RAM used is one pointer per parameter.
    const Member* Op (Rx* rx, Tx* tx, byte index) override {
        void* args[2];

        switch (index) {
            case '?':
            {
                static const Member m_device = { "Child",
                    NumMembers (2),
                    0,
                    "A child Device." };
                if (!rx) return &m_device;
            }
            case 'A':
            {
                static const Member m1 = { "FloatTests",
                    Esc<2, FLT, STX, kStringBufferSize> (),
                    Esc<2, FLT, STX> (),
                    "Description of functionA." };
                if (!rx) return &m1;

                if (Read (rx, m1.rx_header, Args (args, &io_number_,
                                                 io_string_)))
                    return ReadError ();

                if (Write (tx, m1.tx_header, Args (args, &io_number_,
                                                  io_string_)))
                    return WriteError ();
                return &m1;
            }
            case 'B':
            {
                static const Member m2 = { "SignedIntegerTests",
                    Esc<2, FLT, STX, kStringBufferSize> (),
                    Esc<2, FLT, STX> (),
                    "Description of functionB." };

                if (!rx) return &m2;

                if (Read (rx, m2.rx_header, Args (args, &io_number_,
                                                 io_string_)))
                    return ReadError ();

                if (Write (tx, m2.tx_header, Args (args, &io_number_,
                                                  io_string_)))
                    return WriteError ();
                return &m2;
            }
        }
        return nullptr;
    }

    private:

    enum {
        kStringBufferSize = 16              //< Example string buffer size.
    };

    Root root;                              //< The I2P root Device scope.

    float io_number_;                       //< Example variable.
    char io_string_[kStringBufferSize];     //< Example string.
};

// Test child Device.
class Root : public Device {
    public:

                                         // Remote procedure call using switch statement example; note the only RAM used is one pointer per parameter.
    virtual const Member* Op (Rx* rx, Tx* tx, byte index) {
        static const Member this_member = { "Root",
            NumMembers (2),
            0,
            "Root scope device." };

        if (rx == nullptr)
            return &this_member;

        void* args[2];
        switch (index) {
            case ' ':
            {
                
            }
            case 'B':
            {

            }
            case 'C':
            {
                static const Member m1 = { "FloatTests",
                    Esc<2, FLT, STX, kStringBufferSize> (),
                    Esc<2, FLT, STX> (),
                    "Description of functionA." };
                if (!rx) return &m1;

                if (Read (rx, m1.rx_header, Args (args, &io_number_,
                                                  io_string_)))
                    return ReadError ();

                if (Write (tx, m1.tx_header, Args (args, &io_number_,
                                                   io_string_)))
                    return WriteError ();
                return &m1;
            }
            case 'D':
            {
                static const Member m2 = { "SignedIntegerTests",
                    Esc <2, FLT, STX, kStringBufferSize> (),
                    Esc <2, FLT, STX> (),
                    "Description of functionB." };

                if (!rx) return &m2;

                if (Read (rx, m2.rx_header, Args (args, &io_number_,
                                                  io_string_)))
                    return ReadError ();

                if (Write (tx, m2.tx_header, Args (args, &io_number_,
                                                   io_string_)))
                    return WriteError ();
                return &m2;
            }
        }
        return nullptr;
    }

    private:

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
        std::cout << "\n+ Running UniprinterTests...\n";
    }

    void teardown ()
    {
        std::cout << "  UniprinterTests completed.\n";
        system ("PAUSE");
    }

};

TEST (UniprinterTests, UniprinterTestsOne)
{
    std::cout << "Running UniprinterTests...\n";
    
    Root root;
    Uniprinter* up = UniprinterInit (Buffer<255> (), 255, 4, &root);
}
