/** The Chinese Room
    @version 0.x
    @file    /.../console.h
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

#ifndef CHINESEROOM_CONSOLE_H
#define CHINESEROOM_CONSOLE_H

#include "room.h"
//#include "symbol_table.h"
#include "book.h"

namespace _ {

/** Reads a single char from the keyboard 
    @param header The header to print. */
INLINE char ReadChar (const char* header) {
    std::cout << header;
    char c = getchar ();
    std::cout << '\n';
    return c;
}

/** Reads an int from the keyboard.
    @param header The header to print. 
    @param number The number to read. */
INLINE int ReadInt (const char* header) {
    int number;
    std::cout << header;
    std::cin.clear ();
    std::cin >> number;
    return number;
}

/** Reads a float from the keyboard. 
    @param header The header to print. 
    @param number The number to read. */

INLINE float ReadFloat (const char* header) {
    float number;
    std::cout << header;
    std::cin.clear ();
    std::cin >> number;
    return number;
}

/** Reads a new string from the keyboard.
    @header The header to print.
    @return Returns a new string that you have to delete. */
INLINE char* ReadString (const char* header, int buffer_size) {
    if (buffer_size < 0)
        return nullptr;
    char* buffer = new char[buffer_size];
    std::cout << header;
    std::cin.get (buffer, buffer_size, '\n');
    std::cin.clear ();
    std::cin.ignore (buffer_size, '\n');

    return buffer;
}

/** Reads a new string from the keyboard.
    @param  header      The header to print.
    @param  buffer      A buffer to read keyboard input to.
    @param  buffer_size The size of the buffer.
    @return Returns a new string that you have to delete. */
INLINE void ReadString (const char* header, char* buffer, int buffer_size) {
    if (buffer == nullptr)
        return;
    if (buffer_size < 0)
        return;
    std::cout << header;
    std::cin.get (buffer, buffer_size, '\n');
    std::cin.clear ();
    std::cin.ignore (buffer_size, '\n');
}
  
/** A text Console.
    @todo Write me!
*/
template<int kConsoleNumber>
class Console: public Room
{
    public:

    /** Constructs a Console. */
    Console ()
    :   Room (0) {
    
    }

    /** Destructor. */
    virtual ~Console () {

    }

    /** Sets io the Room. */
    ticket_t Init () override {
        return 0;
    }

    /** Main process loop. */
    ticket_t Loop () override {
        return 0;
    }

    /** Chinese room operations. */
    const Member* Op (byte index, Verifier* io) override {
        return 0;
    }

    private:

    // 2-to-4-byte vtable pointer here in memory!
  
};

}       //< namespace _

#endif  //< CHINESEROOM_CONSOLE_H
