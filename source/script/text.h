/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/text.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
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

#pragma once
#include <stdafx.h>

#ifndef HEADER_FOR_SCRIPT_TEXT
#define HEADER_FOR_SCRIPT_TEXT

#include "expr.h"

#if USING_SCRIPT_TEXT

namespace _ {

/** A fixed-size strand for writing text too.
    @warning Each size of Text uses up some ROM. This class works good when you 
             just use the default template parameter of kTextLengthMax. */
template <size_t kSize_ = SCRIPT_TEXT_SIZE>
class Text: public Strand, public Operand {
#undef SCRIPT_TEXT_SIZE
    public:

    enum {
        kSize = kSize_, //< Size of the buffer in bytes.
    };

    Text () :
        Strand (kSize, buffer_) {
        // Nothing to do here ({:-D)-+=<
    }

    /** Script Ops.
        @param index The index of the expression.
        @param expr  The Expr to read and write from.
        @return      Returns nil upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const Op* Star (wchar_t index, Expr* expr) {
        static const Op kThis = { "Text",
            OpFirst ('A'), OpFirst ('B'),
            "A text message.", 0
        };
        void* args[1];

        switch (index) {
            case '?': return ExprOperand (expr, kThis);
            case 'A': {
                static const Op kOpA = { "read",
                    Params<1, STR, kSize> (), Params<0> (),
                    "Reads the message.", kOpOperation, 0
                };
                if (!expr) return &kOpA;
                return ExprArgs (expr, Params<1, STR, kSize> (),
                                 Args (args, strand_.GetBegin ()));
            }
            case 'B': {
                static const Op kOpB = { "write",
                    Params<1, ADR, kAddressLengthMax> (), Params<0> (),
                    "Writes the message to the given ADR.", kOpOperation, 0
                };
                char address[kAddressLengthMax];

                if (!expr) return &kOpB;
                return ExprArgs (expr, Params<1, STR, kSize> (),
                                 Args (args, strand_.GetBegin ()));
            }
        }
        return nullptr;
    }

    /** Overloaded operator= clones the other Text. */
    Text& operator= (const Text& other) {
        return Strand::SetEquals (other);
    }

    private:

    char buffer_[kSize]; //< Statically allocated Strand buffer.
};      //< class Text
}       //< namespace _
#endif  //< USING_SCRIPT_TEXT
#endif  //< HEADER_FOR_SCRIPT_TEXT
