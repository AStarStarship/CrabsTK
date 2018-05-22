/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/operand.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR_CRABS_OPERAND
#define HEADER_FOR_CRABS_OPERAND

#include "slot.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4

namespace _ {

struct KABUKI Expr;

/** Interface for an abstract A*B Op operand.
    A Script Operand is an object that is being operated on.
    Let A be a set of states in a Chinese Room state machine.
    Let B be a set of bytes in a set of Abstract Serial Ports.

    @code
    +----------- Result: The set of fnewe states
    | +--------- Op
    | | +------- Set of fnewe states
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

    @see _::Console for example of how to use Operand interface.
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

#if USING_PRINTER
/** Queries the given Operand Op Header.
    @param  expr The expression to write the query to. Set to nil to return
                 op.
    @param  op   The Op header.
    @return Returns nil upon success and an error Op upon failure. 
KABUKI const Op* OperandQuery (Expr* expr, const Op* op);*/

/** Queries the operand for the given ??? @todo fix me */
Slot& OperandQuery (Operand* root, const char* address, Slot& key);

/** Prints the Operand to the Text.
    @param  text     The Text to print to.
    @param  operand The Operand to print.
    @return text. */
KABUKI Printer PrintOperand (Printer slot, Operand* operand);
#endif

}   //< namespace _

#if USING_PRINTER
/** Overloaded operator<< prints the given operand to the text. */
inline _::Printer& operator<< (_::Printer& printer, _::Operand* operand) {
    return PrintOperand (printer, operand);
}

#endif
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 5
#endif  //< HEADER_FOR_CRABS_OPERAND
