/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/operation.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
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

#ifndef CHINESE_ROOM_OPERATION_H
#define CHINESE_ROOM_OPERATION_H

#include "evaluation.h"
#include "array.h"

namespace _ {

/** Key/name string, params, result, and optional description of a data set.
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
    const char   * metadata;   //< Pointer to the metadata string.
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
    class Expr : public Operand {
        public:

        enum {
            kStringBufferSize = 16       //< Example string buffer size.
        };

        void foo () {}     //< Classical foo.
        void bar () {}     //< Some people drink too much :-)

        virtual const Operand* Star (int index, Expression* expr) {
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
                        Params<2, FLT, STX, kStringBufferSize> (),
                        Params<2, FLT, STX> (),
                        "Description of foo." };

                    // 66 is ASCII 'A'
                    if (!expr) return &o_A;

                    if (a->read (o_A.params, args (args, &ioNumber, 
                                                   ioString))) 
                    return readError ();

                    foo ();  // are you?

                    return Write (expr, o_A.result, param.args (&ioNumber, 
                                                                ioString)));
                }
                case 65: {
                    static const Operation o_B = { "bar",
                        Params<2, FLT, STX, kStringBufferSize> (),
                        Params<2, FLT, STX> (),
                        "Description of bar."
                    };
                
                    if (!io) return &o_B;

                    if (Read (io, o_B.params, param.args (&ioNumber, 
                                                            ioString)))
                        return readError ();

                    bar ();
                    
                    return Write (o_B.result, param.args (&ioNumber, 
                                                          ioString)));
                }
            }
            return nullptr;
        }

        private:

        float ioNumber;                 //< Example variable.
        byte ioString[kStringBufferSize];//< Example string.
    };
    @endcode
*/

struct KABUKI Operand {
    /** An A*B abstract algebra Script Expression.
        
        @param index The index of the expression.
        @param io    The Bin for the IO slot.
        @return      Returns null upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const Operation* Star (int index, Expression* expr) = 0;
};

/** Converts the value to a pointer. */
KABUKI const uint_t* NumOperations (std::uintptr_t value);

/** Converts the given value to a pointer. */
KABUKI const uint_t* FirstOperation (uint_t value);

/** Returns the number of members an Star has. */
KABUKI uintptr_t Index (const void* ptr);

/** Returns the number of members a Star has. */
KABUKI uintptr_t CountCoperations (const Operation* op);

/** Prints the given Set to the std::out 
    Quote: Wikipedia "In mathematics an operand is the object of a mathematical
    operation, i.e. it is the quantity that is operated on." */
KABUKI void OperationPrint (const Operation* op);



/** Gets the number of operations in the given expressions. */
KABUKI uintptr_t ToUInt (Operand* op);

/** Returns true if the given Operand is a Operation (true) or Expression 
    (false). */
KABUKI bool IsGroup (const Operation* op);

/** Prints the given Star to the console. */
KABUKI void OperandPrint (Operand* operand);

}   //< namespace _
#endif  //< CHINESE_ROOM_OPERATION_H
