/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/a.h
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

#include "../include/module_all.h"
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
    kRoomSize           = 1024,
};

/* Test Operand for multiple unit tests.
    The name ChildOperand does not mean anything other than it is a child. */
class ChildOperand : public Operand {
    public:
    
    /** Chinese Room operations. */
    virtual const Operation* Star (int index, Expression* expr) {
        void* args[2];

        static const Operation this_op = { "Child",
            NumOperations (2), FirstOperation ('a'),
            "A child Operand." };

        switch (index) {
            case '?': return &this_op;
            case 'a': {
                static const Operation op_a = { "SignedIntegerTests",
                    BSeq<2, UI1, STR, kStringBufferSize> (),
                    BSeq<2, UI1, STR> (),
                    "Description of functionA.", 0 };
                if (!expr) return &op_a;

                if (Args (expr, op_a.params, 
                          Args (args, &test_ui1_, test_str_)))
                    return expr->result;
                    
                // Function logic here

                return Result (expr, op_a.result, Args (args, &test_ui1_,
                                                       test_str_));
            }
            case 'b': {
                static const Operation op_b = { "FloatTests",
                    BSeq<2, FLT, STR, kStringBufferSize> (),
                    BSeq<2, FLT, STR> (),
                    "Description of functionB.", 0 };
                if (!expr) return &op_b;

                if (Args (expr, op_b.params, Args (args, &test_flt_,
                          test_str_)))
                    return expr->result;

                return Result (expr, op_b.result, Args (args, &test_flt_,
                                                       test_str_));
            }
            case _::DEL: {

                break;
            }
        }
        return nullptr;
    }
    
    private:

    enum {
        kStringBufferSize = 16          //< Example string buffer size.
    };

    uint8_t test_ui1_;                  //< Text UI1.
    float test_flt_;                    //< Test FLT.
    char test_str_[kStringBufferSize]; //< Test STR.
};

/** Test child Operand. */
class Parent : public Operand {
    public:

    enum {
        kStringBufferSize = 16          //< Example string buffer size.
    };

    // Interprocess operations.
    virtual const Operation* Star (int index, Expression* expr) {
        void* args[2];

        static const Operation this_op = { "Parent",
            NumOperations (4), FirstOperation ('a'),
            "Root scope device." };

        switch (index) {
            case '?': return &this_op;
            case 'a': {
                if (!expr) return child_a.Star ('?', expr);
                return Push (expr, &child_a);
            }
            case 'b': {
                if (!expr) return child_b.Star ('?', expr);
                return Push (expr, &child_b);
            }
            case 'c': {
                static const Operation op_c = { "FloatTests",
                    BSeq<2, FLT, STR, kStringBufferSize> (),
                    BSeq<2, FLT, STR> (),
                    "Description of functionA." };
                if (!expr) return &op_c;

                if (Args (expr, op_c.params, Args (args, &io_number_,
                                                       io_string_)))
                    return expr->result;

                return Result (expr, op_c.result, Args (args, &io_number_,
                                                   io_string_));
            }
            case 'd': {
                static const Operation m4 = { "SignedIntegerTests",
                    BSeq<2, FLT, STR, kStringBufferSize> (),
                    BSeq<2, FLT, STR> (),
                    "Description of functionB." };

                if (!expr) return &m4;

                if (Args (expr, m4.params, Args (args, &io_number_,
                                                 io_string_)))
                    return expr->result;

                return Result (expr, m4.result, Args (args, &io_number_,
                                                      io_string_));
            }
        }
        return nullptr;
    }

    private:
    
    ChildOperand child_a,               //< ChildOperand Expression in index 'A'.
          child_b;                      //< ChildOperand Expression in index 'B'
    float io_number_;                   //< Example variable.
    char  io_string_[kStringBufferSize];//< Example string.
};

/** A test room that can fit in 1KB of RAM. */
class This : public Room {
    public:

    enum {
        kRoomSize = 1024
    };

    This ():
        Room (buffer_, kRoomSize)
    {
        
    }

    // Interprocess operations.
    virtual const Operation* Star (int index, Expression* expr) {
        static const Operation this_op = { "Room",
            NumOperations (1), FirstOperation ('a'),
            "Root scope device.", 0 };

        switch (index) {
            case '?': return &this_op;
            case 'a': {
                if (!expr) return parent.Star ('?', nullptr);
                return Push (expr, &parent);
            }
            default: {
            }
        }
        return nullptr;
    }

    private:

    Parent parent;

    uintptr_t buffer_[(kRoomSize / sizeof (uintptr_t)) + 1];  //< 
};
