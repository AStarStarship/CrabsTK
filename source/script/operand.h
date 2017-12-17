/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/operand.h
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

#ifndef HEADER_FOR_SCRIPT_OPERAND
#define HEADER_FOR_SCRIPT_OPERAND

#include "operation.h"

namespace _ {

struct KABUKI Expression;

/** Interface for an abstract A*B Operation operand.
    A Script Operand is an object that is being operated on.
    Let A be a set of states in a Chinese Room state machine.
    Let B be a set of bytes in a set of Abstract Serial Ports.

    @code
    +----------- Result: The set of finite states
    | +--------- Operation
    | | +------- Set of finite states
    | | | +----- Star Operation
    | | | | +--- Operands (input values)
    | | | | |
    v v v v v
    A = A * B   
        |<->|   Operation
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

        virtual const Operand* Star (uint index, Expression* expr) {
            void* args[2];    //< An array of 2 void* for the Operation.

            switch (index)
            {
                case '?': { 
                    static const Operation o_header = 
                    { 
                        "ChineseRoomExample", ConvertNumOperands (2), nullptr, 
                        "Description of ChineseRoomExample.", 0
                    };

                    // 63 is ASCII '?'
                    return &o_header;
                }
                case 64: {
                    static const Operation o_A = { "foo",
                        Params<2, FLT, STR, kTextBufferSize> (),
                        Params<2, FLT, STR> (),
                        "Description of foo." };

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
                    static const Operation o_B = { "bar",
                        Params<2, FLT, STR, kTextBufferSize> (),
                        Params<2, FLT, STR> (),
                        "Description of bar."
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

        // Handles Script Commands.
        virtual const char* Do (const char* text, const char* strand_end) = 0;

        private:

        float io_number_;                   //< Example variable.
        byte io_string_[kTextBufferSize]; //< Example char.
    };
    @endcode
*/

struct KABUKI Operand {

    /** Abstract Script Operation(s).
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const Operation* Star (uint index, Expression* expr) = 0;

    /** Handles Script Text Commands.
        @param text     Beginning of the Text buffer. 
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* Sudo (const char* text, const char* strand_end) = 0;
};

/** Returns the name of the given Operand. */
KABUKI const char* OperandName (Operand* op);

/** Queries the given Operand Operation Header.
    @param  expr      The expression to write the query to. Set to nil to return
                      operation.
    @param  operation The Operation header.
    @return Returns nil upon success and an error Operation upon failure. */
KABUKI const Operation* OperandQuery (Expression* expr,
                                      const Operation* operation);

/** Gets the number of operations in the given expressions. */
KABUKI uintptr_t OperandCount (Operand* op);

#if SCRIPT_USING_TEXT
/** Prints the Operand to the Text.
    @param  txt     The Text to print to.
    @param  operand The Operand to print.
    @return txt. */
KABUKI Text& OperandPrint (Operand* operand, Text& txt);
#endif  //< SCRIPT_USING_TEXT

}   //< namespace _

#if SCRIPT_USING_TEXT
#include "text.h"

/** Overloaded operator<< prints the given operand to the txt. */
KABUKI _::Text& operator<< (_::Text& txt, _::Operand* operand);

#endif  //< SCRIPT_USING_TEXT
#endif  //< HEADER_FOR_SCRIPT_OPERAND
