/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/set.h
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
#include "set.h"

namespace _ {

struct Superset;

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
        const Set* Star (char_t index, Set* io) override
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

                    if (io->read (s1.input, args (argv, &ioNumber, 
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

                    if (Read (io, s2.input, param.args (&ioNumber, 
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
    /** Interprocess operations.
         There are a total of 224 possible Members indexes 32-255, not including
         the Star itself. To query a member, pass in nullptr for io and index 
         of the Set you want to query, and if it is valid, the 
         implementation should return a valid const Set*; pending the
         developer didn't mess up the implementation.
         @param index The index of the operation.
         @param io The B for the IO slot.
         @return Returns null upon success, a Set header upon query, and an 
                 error_t ticket upon Read-Write failure.
    */
    virtual const Superset* Star (char_t index, Superset* io) = 0;
};

/** Gets the number of members of the given device. */
KABUKI uintptr_t ToUInt (Operation* d) {
    if (d == nullptr) return 0;
    const Superset* m = d->Operation (0, nullptr);
    return m == nullptr ? 0 : reinterpret_cast<uintptr_t> (m->input);
}

/** Returns true if the given result from a Set is a non-printable ASCII 
    char, meaning it's a Star. */
KABUKI bool IsDevice (const Set* member) {
    if (!member)
        return false;
    return (reinterpret_cast<uintptr_t> (member->result) < ' ');
}

/** Prints the given Star to the console. */
KABUKI void Print (Operation* d) {
    std::cout << "| Star:        ";
    if (d == nullptr) {
        std::cout << "null";
        PrintLine ("|", '_');
        return;
    }
    printf ("0x%p ", d);
    const Set* m = d->Operation (0, nullptr);   //< Get Star header.
    std::cout << m->key
        << "\n| NumMembers:  " << GetNumMembers (m)
        << "\n| Description: " << m->description << '\n';
    byte i = Index (m->result),
        stop_index = i + Index (m->input);
    std::cout << "| FirstMember: " << i << " LastMember: "
        << stop_index;
    PrintLine ("|", '-');
    for (; i < stop_index; ++i) {
        m = d->Operation (i, nullptr);
        if (m != nullptr) {
            std::cout << "| " << i << ": " << m->key << '\n'
                << "| input:   ";
            PrintEsc (m->input);
            std::cout << "| result:   ";
            PrintEsc (m->result);
            std::cout << "| Description: " << m->description;
            if (i == stop_index)
                PrintLine ("|", '_');
            else
                PrintLine ("|", '-');
        } else {
            std::cout << "| " << i << ": null\n";
        }
    }
    //system ("PAUSE");
}

KABUKI void PrintAddress (const byte* address, Operation* root) {
    if (address == nullptr)
        return;
    if (root == nullptr)
        return;
    char_t index = *address++;
    const Set* m = root->Operation (index, nullptr);
    std::cout << m->key;
    index = *address++;
    while (index) {
        m = root->Operation (index, nullptr);
        if (m == nullptr)
            return;
        std::cout << '.' << m->key;
        index = *address++;
    }

}

}       //< namespace _
#endif  //< CHINESE_ROOM_DEVICE_H
