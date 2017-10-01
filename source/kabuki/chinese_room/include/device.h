/** The Chinese Room
    @version 0.x
    @file    ~/chinses_room/include/device.h
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

#ifndef CHINESE_ROOM_DEVICE_H
#define CHINESE_ROOM_DEVICE_H

#include "uniprinter.h"
#include "member.h"

namespace _ {

struct Verifier;

/** Interface for a device that can be manipulated by a Terminal.
    When a device is selected by a Terminal, the caller can then call functions 
    of that device. The Terminal has a stack of devices that it then pushes the 
    Device* on top of. This object is now selected, and agents can now call 
    functions of this object via the Device Control (DC).

    @code
    // Example remote procedure call using Device interface.
    class DeviceExample : public Device {
        //NONCOPYABLE (DeviceExample)
        public:

        void foo () {}     //< Example dummy foo.
        void bar () {}     //< Example dummy bar.

        // I2P operations.
        const Member* Op (byte index, Verifier* io) override
        {
            void* argv[2];    //< An array of 2 void* for the Rpc arguments.

            switch (index)
            {
                case '?': { 
                    static const Member m0 = 
                    { 
                        "ChineseRoomExample", ConvertNumMembers (2), nullptr, 
                        "Description of ChineseRoomExample." 
                    };

                    // 63 is ASCII '?'
                    return &m0;
                }
                case 64: {
                        static const Member m1 = { "foo",
                        Rx<2, FLT, STR, StringBufferSize>::Header,
                        Tx<2, FLT, STR>::Header,
                        "Description of foo." };

                    // 66 is ASCII 'A'
                    if (!io) return &m1;

                    if (io->read (m1.rx_header, args (argv, &ioNumber, 
                                                      ioString))) 
                    return readError ();

                    foo ();

                    if (Write (io, m1.tx_header, param.args (&ioNumber, 
                                                             ioString)))
                        return writeError ();
                    return &m1;
                }
                case 65: {
                    static const Member m2 = { "bar",
                        Rx<2, FLT, STR, StringBufferSize>::Header,
                        Tx<2, FLT, STR>::Header,
                        "Description of bar." };
                
                    if (!io) return &m2;

                    if (Read (io, m2.rx_header, param.args (&ioNumber, 
                                                            ioString)))
                        return readError ();

                    bar ();

                    if (io->write (m2.tx_header, param.args (&ioNumber, 
                                                             ioString)))
                        return writeError ();
                    return &m2;
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
struct Device {
    /** Interprocess operations.
         There are a total of 224 possible Members indexes 32-255, not including
         the Device itself. To query a member, pass in nullptr for io and index 
         of the Member you want to query, and if it is valid, the 
         implementation should return a valid const Member*; pending the
         developer didn't mess up the implementation.
         @param index The index of the operation.
         @param io The Uniprinter for the IO slot.
         @return Returns null upon success, a Member header upon query, and an 
                 error_t ticket upon Read-Write failure.
    */
    virtual const Member* Op (byte index, Verifier* io) = 0;
};

/** Gets the number of members of the given device. */
KABUKI uintptr_t ToUInt (Device* d);

/** Returns true if the given tx_header from a Member is a non-printable ASCII 
    char, meaning it's a Device. */
KABUKI bool IsDevice (const Member* member);

/** Prints the given Device to the console. */
KABUKI void Print (Device* d);

KABUKI void PrintAddress (const byte* address, Device* root);

}       //< namespace _
#endif  //< CHINESE_ROOM_DEVICE_H
