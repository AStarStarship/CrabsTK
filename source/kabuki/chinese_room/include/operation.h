/** The Chinese Room
    @version 0.x
    @file    ~/chinese_room/include/operation.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESE_ROOM_OPERATION_H
#define CHINESE_ROOM_OPERATION_H

#include "subset.h"
#include "set.h"

namespace _ {

struct Set;

/** Interface for a device that can be manipulated by a Terminal.
    When a device is selected by a Terminal, the caller can then call functions 
    of that device. The Terminal has a stack of devices that it then pushes the 
    Star* on top of. This object is now selected, and agents can now call 
    functions of this object via the Star Control (DC).

    @code
    // Example remote procedure call using Star interface.
    class DeviceExample : public Operation {
        //NONCOPYABLE (DeviceExample)
        public:

        void foo () {}     //< Example dummy foo.
        void bar () {}     //< Example dummy bar.

        // I2P operations.
        const Set* Star (char_t index, B* io) override
        {
            void* argv[2];    //< An array of 2 void* for the Rpc arguments.

            switch (index)
            {
                case '?': { 
                    static const Set m0 = 
                    { 
                        "ChineseRoomExample", ConvertNumMembers (2), nullptr, 
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

                    if (io->read (s1.params, args (argv, &ioNumber, 
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

                    if (io->write (s2.result, param.args (&ioNumber, 
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
struct Operation {
    /** An abstract algebra operation.
        There are a total of 224 possible Members indexes 32-255, not including 
        the Star itself. To query a member, pass in nullptr for io and index 
        of the Set you want to query, and if it is valid, the 
        implementation should return a valid const Set*; pending the 
        developer didn't mess up the implementation.
        @param index The index of the operation.
        @param io    The B for the IO slot.
        @return      Returns null upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure.
    */
    virtual const Set* Star (char_t index, B* io) = 0;
};

/** Gets the number of members of the given device. */
KABUKI uintptr_t ToUInt (Operation* d);

/** Returns true if the given result from a Set is a non-printable ASCII 
    char, meaning it's a Star. */
KABUKI bool IsDevice (const Set* member);

/** Prints the given Star to the console. */
KABUKI void Print (Operation* d);

KABUKI void PrintAddress (const byte* address, Operation* root);

}       //< namespace _
#endif  //< CHINESE_ROOM_STAR_H
