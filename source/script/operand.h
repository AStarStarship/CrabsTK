/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/operand.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
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

#ifndef HEADER_FOR_SCRIPT_OPERAND
#define HEADER_FOR_SCRIPT_OPERAND

#include "op.h"

namespace _ {

struct KABUKI Expr;

/** Interface for an abstract A*B Op operand.
    A Script Operand is an object that is being operated on.
    Let A be a set of states in a Chinese Room state machine.
    Let B be a set of bytes in a set of Abstract Serial Ports.

    @code
    +----------- Result: The set of finite states
    | +--------- Op
    | | +------- Set of finite states
    | | | +----- Star Op
    | | | | +--- Operands (input values)
    | | | | |
    v v v v v
    A = A * B   
        |<->|   Op
    |<----->|   Evaluation
    @endcode

    When a device is selected by a Slot, the caller can then call functions 
    of that device. The Slot has a stack of devices that it then pushes the 
    Star* on top of. This object is now selected, and agents can now call 
    functions of this object via the Data Controller (DC).

    @code
    class Example : public Operand {
        public:

        enum {
            kTextBufferSize = 16       //< Example char buffer size.
        };

        void foo () {}     //< Classical foo.
        void bar () {}     //< Some people drink too much :-)

        virtual const Operand* Star (wchar_t index, Expr* expr) {
            static const Op kThis = { 
                "ChineseRoomExample", ConvertNumOperands (2), nullptr, 
                "Description of ChineseRoomExample.", '}', ';', nullptr
            };

            void* args[2];    //< An array of 2 void* for the Op.

            switch (index)
            {
                case '?': { 

                    // 63 is ASCII '?'
                    return &kThis;
                }
                case 64: {
                    static const Op o_A = { "foo",
                        Bsq<2, FLT, STR, kTextBufferSize> (),
                        Bsq<2, FLT, STR> (),
                        "Description of foo.", '(', ';', nullptr };

                    // 66 is ASCII 'A'
                    if (!expr) return &o_A;

                    if (a->read (o_A.params, args (args, &io_number_, 
                                                   io_string_))) 
                    return readError ();

                    foo ();  // are you?

                    return Write (expr, o_A.result, param.args (&io_number_, 
                                                                io_string_)));
                }
                case 65: {
                    static const Op o_B = { "bar",
                        Bsq<2, FLT, STR, kTextBufferSize> (),
                        Bsq<2, FLT, STR> (),
                        "Description of bar.", '(', ';', nullptr
                    };
                
                    if (!io) return &o_B;

                    if (Read (io, o_B.params, param.args (&io_number_, 
                                                            io_string_)))
                        return readError ();

                    bar ();
                    
                    return Write (o_B.result, param.args (&io_number_, 
                                                          io_string_)));
                }
            }
            return nullptr;
        }

        private:

        float io_number_;                 //< Example variable.
        byte io_string_[kTextBufferSize]; //< Example char.
    };
    @endcode
*/

struct KABUKI Operand {

    /** Abstract Script Op(s).
        @param index The index of the expression.
        @param expr  The Expr to read and write from.
        @return      Returns nil upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const Op* Star (wchar_t index, Expr* expr) = 0;
};

/** Returns the name of the given Operand. */
KABUKI const char* OperandName (Operand* op);

/** Gets the number of ops in the given expressions. */
KABUKI uintptr_t OperandCount (Operand* op);

/** Returns the index of the given Op using the given slot.
    @param  key_begin Beginning of the key slot buffer.
    @param  key_end   End of the key slot buffer.
    @return A value printable Unicode char or invalid if the Operand doesn't 
            Contain the given key. */
KABUKI wchar_t OperandIndex (Operand* operand, char* key_begin, char* key_end);

#if USING_SCRIPT_TEXT
/** Queries the given Operand Op Header.
    @param  expr The expression to write the query to. Set to nil to return
                 op.
    @param  op   The Op header.
    @return Returns nil upon success and an error Op upon failure. 
KABUKI const Op* OperandQuery (Expr* expr, const Op* op);*/

/** Queries the */
Slot& OperandQuery (Operand* root, const char_t* address, Slot& key);

/** Prints the Operand to the Text.
    @param  text     The Text to print to.
    @param  operand The Operand to print.
    @return text. */
KABUKI Slot& OperandPrint (Operand* operand, Slot& slot);
#endif  //< USING_SCRIPT_TEXT

}   //< namespace _

#if USING_SCRIPT_TEXT 
/** Overloaded operator<< prints the given operand to the text. */
inline _::Slot& operator<< (_::Slot& slot, _::Operand* operand) {
    return slot << OperandPrint (operand, slot);
}
#endif  //< USING_SCRIPT_TEXT
#endif  //< HEADER_FOR_SCRIPT_OPERAND
