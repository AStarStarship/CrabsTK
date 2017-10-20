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

#include "error.h"
#include "set.h"

namespace _ {

struct Expression;

/** Interface for an A*B Star Operable.
    An Operable is not technically an expression in the mathematical sense
    but it gets it becomes one when it is implemented on an Abstract Stack
    Machine.

    

    @code
    +------------- Result
    | +----------- Operation
    | | +--------- Set of Finite States
    | | | +------- Operable
    | | | | +----- Arguments
    | | | | |
    v v v v v
    A = A * B
        |<->|
      Operable
    @endcode

    When a device is selected by a Terminal, the caller can then call functions 
    of that device. The Terminal has a stack of devices that it then pushes the 
    Star* on top of. This object is now selected, and agents can now call 
    functions of this object via the Star Control (DC).

    @code
    // Example remote procedure call using Star interface.
    class OperableExample : public Operable {
        public:

        void foo () {}     //< Example dummy foo.
        void bar () {}     //< Example dummy bar.

        // Script expressions.
        const Operable* Star (char_t index, Expression* expr) override
        {
            void* argv[2];    //< An array of 2 void* for the Rpc arguments.

            switch (index)
            {
                case '?': { 
                    static const Set m0 = 
                    { 
                        "ChineseRoomExample", ConvertNumOperables (2), nullptr, 
                        "Description of ChineseRoomExample." 
                    };

                    // 63 is ASCII '?'
                    return &m0;
                }
                case 64: {
                        static const Set s1 = { "foo",
                        Rx<2, FLT, STR, StringBufferSize>::Header,
                        Tx<2, FLT, STR>::Header,
                        "Description of foo." };

                    // 66 is ASCII 'A'
                    if (!io) return &s1;

                    if (a->read (s1.params, args (argv, &ioNumber, 
                                                      ioString))) 
                    return readError ();

                    foo ();

                    if (Write (io, s1.result, param.args (&ioNumber, 
                                                             ioString)))
                        return writeError ();
                    return &s1;
                }
                case 65: {
                    static const Set s2 = { "bar",
                        Rx<2, FLT, STR, StringBufferSize>::Header,
                        Tx<2, FLT, STR>::Header,
                        "Description of bar." };
                
                    if (!io) return &s2;

                    if (Read (io, s2.params, param.args (&ioNumber, 
                                                            ioString)))
                        return readError ();

                    bar ();

                    if (a->write (s2.result, param.args (&ioNumber, 
                                                             ioString)))
                        return writeError ();
                    return &s2;
                }
            }
            return nullptr;
        }

        private:

        enum {
            StringBufferSize = 16       //< Example string buffer size.
        };

        float ioNumber;                 //< Example variable.
        byte ioString[StringBufferSize];//< Example string.
    };
    @endcode
*/

struct Operable {
    /** An A*B abstract algebra expression.
        
        @param index The index of the expression.
        @param io    The Bin for the IO slot.
        @return      Returns null upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure.
    */
    virtual const Operation* Star (char_t index, Expression* expr) = 0;
};

/** Key/name string, params, result, and optional description of a data set.
    @code
    static const Operation this_evaluation = { "Key",
                                                Params<1, 2>::Header, Params<1, 2>::Header,
                                                "Description" };
    static const Operation member_device =   { "Key", setNumOperations (0), 0,
                                                "Description" };
    @endcode
*/
struct Operation {
    const char   * name;        //< Pointer to the Operation name.
    const uint_t * params,      //< Pointer to the set parameters B-Seq.
                 * result;      //< Pointer to the set expression result B-Seq.
    const char   * metadata;    //< Pointer to the metadata string.
    void         * evaluation;  //< Pointer to the evaluated set (if it exists).
};

/** Converts the value to a pointer. */
KABUKI uint_t* NumOperations (std::uintptr_t value);


/** Converts the given value to a pointer. */
KABUKI const uint_t* FirstOperation (uint_t value);

/** Returns the number of members an Star has. */
KABUKI uintptr_t Index (const void* ptr);

/** Returns the number of members a Star has. */
KABUKI uintptr_t CountCoperations (const Operation* eval);

/** Error flag for throwing Expression read error. */
KABUKI const Operation* ReadError ();

/** Error flag for throwing Expression write error. */
KABUKI const Operation* WriteError ();

/** Generates a Set from the given error ticket_t. */
KABUKI const Operation* ErrorTicket (ticket_t error);

/** Error flag for throwing Expression stack overflow error. */
KABUKI const Operation* StackOverflow ();

/** An error flag for an invalid op index. */
KABUKI const Operation* InvalidOperation ();

/** Prints the given Set to the std::out */
KABUKI void OperationPrint (const Operation* eval);

/** Gets the number of operations in the given expressions. */
KABUKI uintptr_t ToUInt (Operable* op);

/** Returns true if the given Operable is a Operation (true) or Expression 
    (false). */
KABUKI bool IsOperation (const Operable* op);

/** Prints the given Star to the console. */
KABUKI void OperablePrint (Operable* expr);

/** Prints the stack address of the expression to the std::out. */
KABUKI void PrintAddress (const byte* address, Operable* expr);

}   //< namespace _

#endif  //< CHINESE_ROOM_OPERATION_H
