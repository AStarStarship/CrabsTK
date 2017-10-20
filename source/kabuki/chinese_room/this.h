/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki-impl/TESTS/chinese_room_tests.cpp
    @author  Cale McCollough <calemccollough.github.expr>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.expr>;
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

#include "../chinese_room/include/module.h"
using namespace _;

enum {
    /** @var  ErrorListSize 
       @todo Test with small and large sizes. */
    kErrorListSize       = 4,
    
    /** @var  NumLoopIterations 
       @todo Test with small and large sizes. */
    kNumLoopIterations   = 3,  //< The number of times to test each type with random data.
    
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
    The name ChildObject does not mean anything other than it is a child. */
class ChildObject : public Operable {
    public:
    
    /** Chinese Room operations. */
    virtual const Operation* Star (int index, Expression* expr) {
        void* args[2];
        const Evaluation* error;

        static const Operation this_member = { "ChildObject",
            NumMembers (2), FirstMember ('A'),
            "A child Operation." };
            
        if (!index) return &this_member;

        switch (index) {
            case ' ' {
                // push operation.
            }
            case 'A': {
                static const Operation m1 = { "FloatTests",
                    Esc<2, FLT, STX, kStringBufferSize> (),
                    Esc<2, FLT, STX> (),
                    "Description of functionA.", 0 };
                if (!expr) return &m1;

                if (error = Read (expr, m1.params, Args (args, &io_number_,
                                                       io_string_)))
                    return error;
                    
                // Function logic here

                return Write (expr, m1.result, Args (args, &io_number_,
                                                   io_string_));
            }
            case 'B': {
                     const Evaluation m2 = { "SignedIntegerTests",
                    Esc<2, FLT, STX, kStringBufferSize> (),
                    Esc<2, FLT, STX> (),
                    "Description of functionB." };

                if (!expr) return &m2;

                if (error = Read (expr, m2.params, Args (args, &io_number_,
                                                       io_string_)))
                    return ReadError ();

                return Write (expr, m2.result, Args (args, &io_number_,
                                                   io_string_));
            }
            case ascii::DEL
        }
        return nullptr;
    }
    
    private:

    enum {
        kStringBufferSize = 16          //< Example string buffer size.
    };

    float io_number_;                   //< Example variable.
    char io_string_[kStringBufferSize]; //< Example string.
};

/** Test child Operation. */
class Root : public Operation {
    public:

    // Interprocess operations.
    const Evaluation* Star (int index, Expression* expr) override {
        void* args[2];
        const Evaluation* error;

        static const Evaluation this_member = { "Root",
            NumMembers (4),
            FirstMember ('A'),
            "Root scope device." };

        if (!index) return &this_member;

        switch (index) {
            case 'A': {
                if (!expr) return child_a.Star (0, expr);
                return Push (expr, &child_a);
            }
            case 'B': {
                if (!expr) return child_b.Star (0, expr);
                return Push (expr, &child_b);
            }
            case 'C': {
                static const Evaluation m3 = { "FloatTests",
                    Esc<2, FLT, STX, kStringBufferSize> (),
                    Esc<2, FLT, STX> (),
                    "Description of functionA." };
                if (!expr) return &m3;

                if (error = Read (expr, m3.params, Args (args, &io_number_,
                                                       io_string_)))
                    return error;

                return Write (expr, m3.result, Args (args, &io_number_,
                                                   io_string_));
            }
            case 'D': {
                static const Evaluation m4 = { "SignedIntegerTests",
                    Esc <2, FLT, STX, kStringBufferSize> (),
                    Esc <2, FLT, STX> (),
                    "Description of functionB." };

                if (!expr) return &m4;

                if (error = Read (expr, m4.params, Args (args, &io_number_,
                                                         io_string_)))
                    return error;

                return Write (expr, m4.result, Args (args, &io_number_,
                                                     io_string_));
            }
        }
        return nullptr;
    }

    private:
    
    ChildObject child_a,                      //< ChildObject Expression in index 'A'.
          child_b;                      //< ChildObject Expression in index 'B'

    enum {
        kStringBufferSize = 16          //< Example string buffer size.
    };

    float io_number_;                   //< Example variable.
    char  io_string_[kStringBufferSize];//< Example string.
};

/** A test room that can fit in 1KB of RAM. */
class This : public Room {
    public:

    This ():
        Room (kRoomSize)
    {
        
    }


};
