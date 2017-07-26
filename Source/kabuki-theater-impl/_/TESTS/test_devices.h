/** Kabuki Theater
    @version 0.x
    @file    /.../source/kabuki-theater-impl/_/TESTS/test_devices.h
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

#include <_/console.h>

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

    // I2P operations.
    const Member* Op (byte index, Uniprinter* io) override {
        void* args[2];
        const Member* error;

        static const Member this_member = { "Child",
            NumMembers (2),
            FirstMember ('A'),
            "A child Device." };
        if (index < ' ') return &this_member;

        switch (index) {
            case 'A':
            {
                static const Member m1 = { "FloatTests",
                    Esc<2, FLT, STX, kStringBufferSize> (),
                    Esc<2, FLT, STX> (),
                    "Description of functionA." };
                if (!io) return &m1;

                if (error = Read (io, m1.rx_header, Args (args, &io_number_,
                                                              io_string_)))
                    return error;

                return Write (io, m1.tx_header, Args (args, &io_number_,
                                                      io_string_));
            }
            case 'B':
            {
                static const Member m2 = { "SignedIntegerTests",
                    Esc<2, FLT, STX, kStringBufferSize> (),
                    Esc<2, FLT, STX> (),
                    "Description of functionB." };

                if (!io) return &m2;

                if (error = Read (io, m2.rx_header, Args (args, &io_number_,
                                                 io_string_)))
                    return ReadError ();

                return Write (io, m2.tx_header, Args (args, &io_number_,
                                                      io_string_));
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

// Test child Device.
class Root : public Device {
    public:

    // Interprocess operations.
    const Member* Op (byte index, Uniprinter* io) override {
        void* args[2];
        const Member* error;

        static const Member this_member = { "Root",
            NumMembers (4),
            FirstMember ('A'),
            "Root scope device." };

        if (index < ' ')
            return &this_member;

        switch (index) {
            case 'A':
            {
                return Push (io, &child_a);
            }
            case 'B':
            {
                return Push (io, &child_b);
            }
            case 'C':
            {
                static const Member m3 = { "FloatTests",
                    Esc<2, FLT, STX, kStringBufferSize> (),
                    Esc<2, FLT, STX> (),
                    "Description of functionA." };
                if (!io) return &m3;

                if (error = Read (io, m3.rx_header, Args (args, &io_number_,
                                                  io_string_)))
                    return error;

                return Write (io, m3.tx_header, Args (args, &io_number_,
                                                       io_string_));
            }
            case 'D':
            {
                static const Member m4 = { "SignedIntegerTests",
                    Esc <2, FLT, STX, kStringBufferSize> (),
                    Esc <2, FLT, STX> (),
                    "Description of functionB." };

                if (!io) return &m4;

                if (error = Read (io, m4.rx_header, Args (args, &io_number_,
                                                  io_string_)))
                    return error;

                return Write (io, m4.tx_header, Args (args, &io_number_,
                                                      io_string_));
            }
        }
        return nullptr;
    }

    private:
    
    Child child_a,                          //< Child object in index 'A'.
        child_b;                            //< Child object in index 'B'

    enum {
        kStringBufferSize = 16              //< Example string buffer size.
    };

    float io_number_;                       //< Example variable.
    char io_string_[kStringBufferSize];     //< Example string.
};
