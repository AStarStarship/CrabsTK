/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/operation.h
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

#ifndef HEADER_FOR_SCRIPT_OPERATION
#define HEADER_FOR_SCRIPT_OPERATION

#include "config.h"

namespace _ {

/** Key/name char, params, result, and optional description of a data set.
    @code
    static const Operation this_evaluation = { "Key",
        Params<1, 2>::Header, Params<1, 2>::Header,
        "Description" };
    static const Operation member_device =   { "Key", 
        NumOperations (0), FirstOperation ('A'),
        "Description" };
    @endcode
*/
struct KABUKI Operation {
    const char   * name;       //< Pointer to the Operation name.
    const uint_t * params,     //< Pointer to the set parameters B-Seq.
                 * result;     //< Pointer to the set expression result B-Seq.
    const char   * metadata;   //< Pointer to the metadata char.
    const byte   * evaluation; //< Pointer to the evaluated B-Sequence.
};

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

        private:

        float io_number_;                   //< Example variable.
        byte io_string_[kTextBufferSize]; //< Example char.
    };
    @endcode
*/

struct KABUKI Operand {

    /** Handles Script Commands.
        @param text     Beginning of the Text buffer. 
        @param text_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* HandleText (const char* text,
                                    const char* text_end) = 0;

    /** An A*B abstract algebra Script Expression.
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const Operation* Star (uint index, Expression* expr) = 0;
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

/** Converts the value to a pointer. */
inline const uint_t* NumOperations (std::uintptr_t value) {
    return reinterpret_cast<const uint_t*> (value);
}

/** Converts the value to a pointer. */
inline std::uintptr_t PointerValue (const uint_t* value) {
    return reinterpret_cast<std::uintptr_t> (value);
}

/** Converts the given value to a pointer. */
inline const uint_t* OperationCount (uint_t count) {
    return reinterpret_cast<const uint_t*>(count);
}

/** Converts the given value to a pointer. */
inline uint OperationCount (const Operation& operation) {
    return reinterpret_cast<uint>(operation.params);
}

/** Converts the given value to a pointer. */
inline const uint_t* OperationFirst (uint_t value) {
    uintptr_t index = value;
    return reinterpret_cast<const uint_t*>(index);
}

/** Converts the given value to a pointer. */
inline uint OperationFirst (const Operation* operaion) {
    if (!operaion) {
        return 0;
    }
    return reinterpret_cast<uint> (operaion->params);
}

/** Converts the given value to a pointer. */
inline uint OperationLast (const Operation* operaion) {
    if (!operaion) {
        return 0;
    }
    return reinterpret_cast<uint> (operaion->params) +
           reinterpret_cast<uint> (operaion->result);
}

/** Gets the number of operations in the given expressions. */
KABUKI uintptr_t ToUInt (Operand* op);

/** Returns true if the given Operand is a Operation (true) or Expression 
    (false). */
KABUKI bool IsGroup (const Operation* op);

#if USE_MORE_ROM
/** Prints the given Set to the std::out
    Quote: Wikipedia "In mathematics an operand is the object of a mathematical
    operation, i.e. it is the quantity that is operated on." */
KABUKI void OperationPrint (const Operation* op);

/** Prints the given Operand to the console. */
KABUKI void OperandPrint (Operand* operand);
#endif  //< USE_MORE_ROM

/** Sequentially searches through the given Operand for an Operation the
    name.
    @param operand The Operand scope.
    @param name    The name to search for. */
KABUKI uint OperationFind (Operand* operand, const char* name);

/** Evaluated Nil Operation result. */
KABUKI const Operation* OperationNil ();

/** Evaluated Invalid Operation result. */
KABUKI const Operation* OperationInvalid ();

}   //< namespace _
#endif  //< HEADER_FOR_SCRIPT_OPERATION
