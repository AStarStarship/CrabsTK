/** Kabuki Theater
    @version 0.x
    @file    ~/source/kabuki-theater-impl/_/TESTS/chinese_room_tests.cpp
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

#include <chinese_room/include/module.h>
using namespace _;

enum {
    /** @var  ErrorListSize 
       @todo Test with small and large sizes. */
    kErrorListSize       = 4,
    
    /** @var  NumLoopIterations 
       @todo Test with small and large sizes. */
    kNumLoopIterations   = 3,      //< The number of times to test each type with random data.
    
    /** @var  kSlotSize 
       @todo Test with small and large sizes. */
    kSlotSize           = 128,
    
    /** @var  StackSize 
       @todo Test with small and large sizes. */
    kStackSize          = 3,

    /** @var  StackSize
        @todo Test with small and large sizes. */
    kRoomSize           = 3,
};

/* Test Operation for multiple unit tests.
    The name Child does not mean anything other than it is a child. */
class Child : public Operation {
    public:
    
    /** Chinese Room operations. */
    const Set* Star (char_t index, B* io) override {
        void* args[2];
        const Set* error;

        static const Set this_member = { "Child",
            NumMembers (2),
            FirstMember ('A'),
            "A child Operation." };
        if (!index) return &this_member;

        switch (index) {
            case 'A': {
                static const Set m1 = { "FloatTests",
                    Esc<2, FLT, STX, kStringBufferSize> (),
                    Esc<2, FLT, STX> (),
                    "Description of functionA." };
                if (!io) return &m1;

                if (error = Read (io, m1.params, Args (args, &io_number_,
                                                       io_string_)))
                    return error;

                return Write (io, m1.result, Args (args, &io_number_,
                                                   io_string_));
            }
            case 'B': {
                static const Set m2 = { "SignedIntegerTests",
                    Esc<2, FLT, STX, kStringBufferSize> (),
                    Esc<2, FLT, STX> (),
                    "Description of functionB." };

                if (!io) return &m2;

                if (error = Read (io, m2.params, Args (args, &io_number_,
                                                       io_string_)))
                    return ReadError ();

                return Write (io, m2.result, Args (args, &io_number_,
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

/** Test child Operation. */
class Root : public Operation {
    public:

    // Interprocess operations.
    const Set* Star (char_t index, B* io) override {
        void* args[2];
        const Set* error;

        static const Set this_member = { "Root",
            NumMembers (4),
            FirstMember ('A'),
            "Root scope device." };

        if (!index) return &this_member;

        switch (index) {
            case 'A': {
                if (!io) return child_a.Star (0, io);
                return Push (io, &child_a);
            }
            case 'B': {
                if (!io) return child_b.Star (0, io);
                return Push (io, &child_b);
            }
            case 'C': {
                static const Set m3 = { "FloatTests",
                    Esc<2, FLT, STX, kStringBufferSize> (),
                    Esc<2, FLT, STX> (),
                    "Description of functionA." };
                if (!io) return &m3;

                if (error = Read (io, m3.params, Args (args, &io_number_,
                                                       io_string_)))
                    return error;

                return Write (io, m3.result, Args (args, &io_number_,
                                                   io_string_));
            }
            case 'D': {
                static const Set m4 = { "SignedIntegerTests",
                    Esc <2, FLT, STX, kStringBufferSize> (),
                    Esc <2, FLT, STX> (),
                    "Description of functionB." };

                if (!io) return &m4;

                if (error = Read (io, m4.params, Args (args, &io_number_,
                                                  io_string_)))
                    return error;

                return Write (io, m4.result, Args (args, &io_number_,
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

/** A test room that can fit in 1KB of RAM. */
class TestRoom : public Room {
    public:

    TestRoom ():
        Room (kRoomSize)
    {
        
    }


};
