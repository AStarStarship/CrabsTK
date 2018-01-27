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
template <size_t kSize_ = kTextSize>
class Text: public Token, public Operand {
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
            "A text message.", '(', ')', nullptr
        };
        void* args[1];

        switch (index) {
            case '?': return ExprQuery (expr, kThis);
            case 'A': {
                static const Op kOpA = { "Read",
                    _::Bsq<1, STR, kSize> (), _::Bsq<0> (),
                    "Reads the message.", '(', ')', nullptr
                };
                if (!expr) return &kOpA;
                return ExprArgs (expr, kOpA,
                                 Args (args, buffer_));
            }
            case 'B': {
                static const Op kOpB = { "Write",
                    _::Bsq<1, ADR, kAddressLengthMax> (), _::Bsq<0> (),
                    "Writes the message to the given ADR.", '(', ')', ' ', nullptr
                };
                char address[kAddressLengthMax];

                if (!expr) return &kOpB;
                return ExprArgs (expr, kOpB,
                                 Args (args, buffer_));
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
