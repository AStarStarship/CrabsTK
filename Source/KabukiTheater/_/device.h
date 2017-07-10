/** The Chinese Room
    @version 0.x
    @file    /.../device.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_IDEVICE_H
#define CHINESEROOM_IDEVICE_H

#include "rx.h"
#include "member.h"

namespace _ {

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
        const Member* Op (Rx* rx, Tx& tx, char index) override
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

                    if (io->read (m1.rx_header, args (argv, &ioNumber, ioString))) 
                    return readError ();

                    foo ();

                    if (Write (io, m1.tx_header, param.args (&ioNumber, ioString)))
                        return writeError ();
                    return &m1;
                }
                case 65: {
                    static const Member m2 = { "bar",
                        Rx<2, FLT, STR, StringBufferSize>::Header,
                        Tx<2, FLT, STR>::Header,
                        "Description of bar." };
                
                    if (!io) return &m2;

                    if (Read (io, m2.rx_header, param.args (&ioNumber, ioString)))
                        return readError ();

                    bar ();

                    if (io->write (m2.tx_header, param.args (&ioNumber, ioString)))
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
        char ioString[StringBufferSize];//< Example string.
    };
    @endcode
*/
struct Device {
    /** I2P operations. */
    virtual const Member* Op (Rx* rx, Tx& tx, char index) = 0;
};

/** Gets the number of members of the given device. */
inline uintptr_t GetNumMembers (Device* d) {
    if (d == nullptr) return 0;
    const Member* m = d->Op (nullptr, Log (), '?');
    return m == nullptr ? 0 : reinterpret_cast<uintptr_t> (m->rx_header);
}

}       //< namespace
#endif  //< CHINESEROOM_IDEVICE_H
