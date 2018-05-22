/** Kabuki Toolkit
    @version 0.x
    @file    ~/tests/seam_1/test_seam_1.h
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

#pragma once
#include <stdafx.h>

#include <assembly.h>

#if RUN_UNIT_TESTS

#ifndef HEADER_FOR_GLOBAL
#define HEADER_FOR_GLOBAL

#include "../../../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "../../../cpputest/include/CppUTest/TestHarness.h"

#include "../../library/crabs/global.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4

using namespace _;

/* Test Operand for multiple unit tests.
    The name ChildOperand does not mean anything other than it is a child.  */
class ChildOperand : public Operand {
    public:
    
    // Chinese Room operations.
    virtual const Op* Star (wchar_t index, Expr* expr) {
        void* args[2];

        static const Op This = { "Child",
            OpFirst ('A'), OpLast ('B'),
            "A child Operand." };

        switch (index) {
            case '?': return &This;
            case 'A': {
                static const Op OpA = { "SignedVarintTests",
                    Params<2, SVI, STR, kBufferSize> (),
                    Params<2, SVI, STR> (),
                    "Description of function \'A\'.", 0 };
                if (!expr) return &OpA;

                if (ExprArgs (expr, OpA, Args (args, &test_svi_,
                                               test_str_)))
                    return expr->result;
                    
                // Function logic here

                return ExprResult (expr, OpA, Args (args, &test_svi_,
                                                       test_str_));
            }
            case 'B': {
                static const Op OpB = { "UnsignedVarintTests",
                    Params<2, UVI, STR, kBufferSize> (),
                    Params<2, UVI, STR> (),
                    "Description of function \'B\'.", 0 };
                if (!expr) return &OpB;

                if (ExprArgs (expr, OpB, Args (args, &test_uvi_, test_str_)))
                    return expr->result;

                return ExprResult (expr, OpB, Args (args, &test_uvi_,
                                                    test_str_));
            }
        }
        return nullptr;
    }
    
    private:

    enum { kBufferSize = 16 };   //< Example string buffer size.

    int  test_svi_;              //< Test SVI.
    uint test_uvi_;              //< Test UVI.
    char test_str_[kBufferSize]; //< Test STR.
};

// Test child Operand.
class Parent : public Operand {
    public:

    enum {
        kBufferSize = 16         //< Example string buffer size.
    };

    // Interprocess operations.
    virtual const Op* Star (wchar_t index, Expr* expr) {
        void* args[2];

        static const Op This = { "Parent",
            OpFirst ('A'), OpLast ('D'),
            "Root scope device.", 0 };

        switch (index) {
            case '?': return &This;
            case 'A': {
                if (!expr) return child_a.Star ('?', expr);
                return Push (expr, &child_a);
            }
            case 'B': {
                if (!expr) return child_b.Star ('?', expr);
                return Push (expr, &child_b);
            }
            case 'C': {
                static const Op OpC = { "UnsignedVarintTests",
                    Params<2, UVI, STR, kBufferSize> (),
                    Params<2, UVI, STR> (),
                    "Description of functionA.", 0 };
                if (!expr) return &OpC;

                if (ExprArgs (expr, OpC, Args (args, &svi_,
                                                       io_string_)))
                    return expr->result;
                  // function call here
                return ExprResult (expr, OpC, Args (args, &svi_,
                                                    io_string_));
            }
            case 'D': {
                static const Op OpD = { "SignedVarintTests",
                    Params<2, SVI, STR, kBufferSize> (),
                    Params<2, SVI, STR> (),
                    "Description of functionB.", 0 };

                if (!expr) return &OpD;

                if (ExprArgs (expr, OpD, Args (args, &svi_, 
                                                     io_string_)))
                    return expr->result;

                return ExprResult (expr, OpD, Args (args, &svi_,
                                   io_string_));
            }
        }
        return nullptr;
    }

    private:
    
    ChildOperand child_a,                 //< ChildOperand Expr.
                 child_b;                 //< ChildOperand Expr.
    int          svi_;                    //< Example variable.
    uint         uvi_;                    //< Example variable.
    char         io_string_[kBufferSize]; //< Example string.
};

// A test room that can fit in 1KB of RAM. 
class This : public Room {
    public:

    enum {
        kRoomSize = 1024
    };

    This ():
        Room ("Test")
    {
        
    }

    // Interprocess operations.
    virtual const Op* Star (wchar_t index, Expr* expr) {
        static const Op This = { "Room",
            OpFirst ('A'), OpLast ('A'),
            "Root scope device.", 0 };

        switch (index) {
            case '?': return &This;
            case 'A': {
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
};
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 5

#endif  //< HEADER_FOR_GLOBAL
#endif  //< RUN_UNIT_TESTS
