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

#include "_/room.h"
#include "_/record_table.h"
#include "_/book.h"

namespace _ {

/** Prints the given byte in Hex.
    This function prints the hex in big endian. */
inline void PrintHex (byte c) {
    uint16_t chars = ToUpperCaseHex (c);
    putchar ((char)chars);
    putchar ((char)(chars >> 8));
    putchar (' ');
}

/** Prints the given string center in the given width.
If string is too small to fit in the width, function will print as much of
the string as it has room for with a "..." If the given width is less than
5, than
*/
inline void PrintCentered (const char* s, int width) {
    if (width < 2) {
        //? Not sure if this is an error.
        return;
    }
    // We need to leave at least one space to the left and right of 
    int length = strlen (s);
    if (length < width - 2) {
        // We need to write the ....
        if (length < 4) {
            // Then we're just going to write the first few letters.
            for (; length >= 0; --length) {
                std::cout << '\n';
            }
        }
    }
    int offset = (width - length) >> 1; //< >> 1 to /2
    for (int i = 0; i < offset; ++i)
        std::cout << '\n';
    printf (s);
    for (offset = width - length - offset; offset <= width; ++offset)
        std::cout << '\n';
}

/** Prints the given char to the stdout if it is printable, and prints SOH. */
inline void PrintChar (char c) {
    if ((c > 0 && c < ' ') || c == 127) {
        std::cout << 1;
        return;
    }
    std::cout << c;
}

/** Prints a line with the given token and number of columns to the debug
    stream. */
inline void PrintLine (char token = '-', int column_width = 80) {
    std::cout << '\n';
    for (int i = 0; i < column_width; ++i)
        std::cout << token;
    std::cout << '\n';
}

inline void PrintLine (const char* start_string, char token, int length = 79) {
    std::cout << '\n' << start_string;
    for (int i = 0; i < 79; ++i)
        std::cout << token;
    std::cout << '\n';
}

/** Prints a vertical tab with the given number of rows. */
inline void PrintLines (int numRows = 10) {
    std::cout << '\r';
    for (int i = 0; i < numRows - 1; ++i)
        std::cout << '\n';
}

/** Prints out the contents of the address to the debug stream. */
inline void PrintMemory (const void* address, size_t size) {
    printf ("|%i", 0);
    // Print columns
    for (int i = 8; i <= 66; i += 8)
        printf ("%8i", i);
    std::cout << '\n';
    std::cout << '|';
    for (int i = 0; i < 65; ++i)
        std::cout << '_';
    std::cout << '\n';

    const char* chars = reinterpret_cast<const char*> (address);
    const char* end = reinterpret_cast<const char*> (address) + size;
    char temp;
    while (chars < end) {
        std::cout << '|';
        for (int i = 0; i < 64; ++i) {
            temp = *chars;
            if (chars >= end)
                temp = 'x';
            PrintChar (temp);
            ++chars;
        }
        printf ("| 0x%x\n", reinterpret_cast<uintptr_t> (chars) - 64);
    }
    std::cout << '|';
    for (int i = 0; i < 64; ++i)
        std::cout << '_';
    printf ("| 0x%x\n", reinterpret_cast<uintptr_t> (chars) + size);
}

/** Prints the given value in hex format. */
template<typename Type>
inline void PrintHex (Type value) {
    char buffer[sizeof (Type) * 2 + 1];
    sprintf_s (buffer, "%x", &value);
    std::cout << "0x";

    int length = strlen (buffer);
    for (int i = 0; i < length; ++i)
        std::cout << '0';
    std::cout << buffer;
}

/** Creates the format string for a key-value pair where the key is
    right-aligned to the column_width. */
inline char CreateKeyValueFormatString (char* s, char column_width, char type) {
    char hundreds = (column_width / 100),
        decimal = (column_width % 10),
        tens = (column_width - hundreds - decimal) / 10;
    s[0] = '%';
    if (column_width <  10) {
        s[1] = decimal + '0';
        s[2] = 's';
        s[3] = ':';
        s[4] = ' ';
        s[5] = '0';
        s[6] = 'x';
        s[7] = '%';
        s[8] = 'p';
        s[9] = '\n';
        return 0;
    } else if (column_width < 100) {
        s[1] = tens + '0';
        s[2] = decimal + '0';
        s[3] = 's';
        s[4] = ':';
        s[5] = ' ';
        s[6] = '0';
        s[7] = 'x';
        s[8] = '%';
        s[9] = 'p';
        s[10] = '\n';
        return 0;
    }
    s[1] = hundreds + '0';
    s[2] = tens + '0';
    s[3] = decimal + '0';
    s[4] = 's';
    s[5] = ':';
    s[6] = ' ';
    s[7] = '0';
    s[8] = 'x';
    s[9] = '%';
    s[10] = 'p';
    s[11] = '\n';
    return 0;
}

/** */
template<char kColumnWidth>
inline void PrintHex (const char* header, void* value) {
    inline char formatString[12],
        nullTermChar = CreateKeyValueFormatString (formatString, kColumnWidth, 'p');
    printf (formatString, header, value);
}

/** */
template<char kColumnWidth, typename Type>
inline void PrintSignedHex (const char* header, Type value) {
    inline char formatString[12],
        nullTermChar = CreateKeyValueFormatString (formatString, kColumnWidth, 'i');
    printf ("formatString: %s\n", formatString);
    printf (formatString, header, value);
}

/** */
template<char kColumnWidth, typename Type>
inline void PrintUnsignedHex (const char* header, Type value) {
    inline char formatString[12],
        nullTermChar = CreateKeyValueFormatString (formatString, kColumnWidth, 'u');
    printf (formatString, header, value);
}

/** Prints an 80-char line of the number repeating with an underscore i.e. 1_1_... */
inline void PrintNumberLine (int index) {
    std::cout << '\n';
    enum { MaxBufferSize = (sizeof (int) == 2) ? 7 : (sizeof (int) == 4) ? 11 : 128 };
    char buffer[MaxBufferSize];
    sprintf_s (buffer, MaxBufferSize, "%u", index);
    int length = strlen (buffer),
        i,
        lettersLeft = 80 % (length + 1);

    for (i = 0; i < 80; i += (length + 1))
        printf ("%s_", buffer);
    for (int j = 0; j < lettersLeft; ++j)
        std::cout << buffer[j];
    std::cout << '\n';
}

/** Prints an 80-char line of the string repeating with an underscore
    i.e. s_s_... */
inline void PrintStringLine (const char* s) {
    PrintLine ();
    int length = strlen (s),
        i;

    for (i = 0; i < 80; i += (length + 1))
        std::cout << s << '_';
    for (int j = 0; j < 80 % (length + 1); ++j)
        std::cout << s[j];
    PrintLine ();
}

/** Prints the given value to the console and prompts the user to press any key
    to continue. */
template<typename Type, const char* format>
inline Type PrintReturn (Type value) {
    printf (format, value);
    return value;
}

/** Prints the array starting at the base_ptr with the given numElements with a
    header and given format. */
template<typename Type>
inline void PrintArray (const char* header, const char* format, Type* base_ptr,
                        Type numElements) {
    PrintLine ();
    std::cout << header << ": numElements: " << numElements << '\n';
    for (Type i = 0; i < numElements; ++i) {
        printf (format, i);
        std::cout << ": ";
        printf (format, base_ptr[i]);
        std::cout << '\n';
    }
    std::cout << '\n';
    PrintLine ();
}

/** Prints an error message and pauses the system. */
inline void PrintPause (const char* s) {
#if DEBUG
    std::cout << "\n" << s << "\n";
    system ("PAUSE");
#endif
}

/** Prints a new line. */
inline void PrintNL () {
    std::cout << '\n';
}

/** Prints a bunch of new lines followed by the given message and a line of
    the specified token. */
inline void PrintLineBreak (const char* message, int top_bottom_margin,
                            char c = '-', int num_columns = 80) {
    PrintLines (top_bottom_margin);
    std::cout << message;
    PrintLine (c, num_columns);
}

inline void Print (Rx* rx) {
    if (rx == nullptr) return;
    uint_t size = rx->size;
    PrintLine ('_');
    printf ("| Rx %p: size: %u, start: %u, stop: %u, read: %u\n", rx, size,
            rx->start, rx->stop, rx->read);
    PrintMemory (SocketBaseAddress (rx), size);
}

/** Prints the given Tx to the stdout. */
inline void Print (Unityper* tx) {
    if (tx == nullptr) {
        PrintLine ('_');
        printf ("| Tx null\n");
        return;
    }
    uint_t size = tx->size;
    PrintLine ('_');
    printf ("| Tx 0x%p: size: %u, start: %u, stop: %u, read: %u\n", tx, size,
            tx->start, tx->stop, tx->read);
    PrintMemory (SocketBaseAddress (tx), size);
}

/** Prints the given Device to the console. */
inline void Print (Device* d) {
    const Member* m = d->Op (0, nullptr);
    PrintLine ("|", '-');
    std::cout << "| Device:      ";
    if (m == nullptr) {
        std::cout << "null\n";
        PrintLine ("|", '_');
        return;
    }
    std::cout << m->key
        << "\n| NumMembers:  " << GetNumMembers (m)
        << "\n| Description: " << m->description << '\n';
    byte i = Index (m->tx_header),
        stop_index = i + Index (m->rx_header);
    std::cout << "| FirstMember: " << i << " LastMember: "
        << stop_index;
    PrintLine ("|", '-');
    for (; i < stop_index; ++i) {
        m = d->Op (i, nullptr);
        if (m != nullptr) {
            std::cout << "| " << i << ": " << m->key << '\n'
                << "| rx_header:   ";
            PrintEsc (m->rx_header);
            std::cout << "| tx_header:   ";
            PrintEsc (m->tx_header);
            std::cout << "| Description: " << m->description;
            if (i == stop_index)
                PrintLine ("|", '_');
            else
                PrintLine ("|", '-');
        } else {
            std::cout << "| " << i << ": null\n";
        }
    }
    system ("PAUSE");
}

/** Prints the given Uniprinter to the console. */
inline void Print (Uniprinter* up) {
    if (up == nullptr)
        return;
    PrintLine ('_');
    printf ("| Uniprinter: 0x%p", up);
    PrintLine ("|", '_');
    std::cout << "| type:          " <<
        (up->type == -1) ? "interprocess no dynamic memory." :
        (up->type == 0) ? "no dynamic memory" :
        (up->type == 1) ? "dynamic memory" :
        (up->type == 2) ? "dynamic memory" : "Invalid type";

    std::cout << "\n| bytes_left:    " << up->bytes_left
        << "\n| tx_offset:     " << up->tx_offset
        << "\n| rx_offset:     " << up->rx_offset
        << "\n| stack_height:  " << up->stack_height
        << "\n| stack_size:    " << up->stack_size
        << "\n| num_verifying: " << up->num_verifying
        << "\n| states_size:   " << up->states_size
        << "\n| num_states:    " << up->num_states;
    Print (up->device);
    std::cout << "| header: ";
    PrintEsc (const_cast<const uint_t*>(up->header));
    std::cout << "\n| Scan Stack: " << up->num_verifying << '\n';
    const uint_t** headers = (const uint_t**)up->headers;
    for (uint_t i = 0; i < up->stack_height; ++i) {
        std::cout << "| ";
        PrintEsc (headers[i]);
    }
}

/** Prints the given Door to the console. */
inline void Print (Door* d) {

}

/** Prints the given Book to the console. */
template<typename TIndex, typename TKey, typename TData, typename THash>
inline void Print (Book<TIndex, TKey, TData, THash>* book) {

}

inline void Print (Book2* book) {   
    return Print<byte, uint16_t, uint16_t, hash16_t> (book);
}

/** Prints this object out to the console. */
inline void Print (RecordTable* rt) {
    if (rt == nullptr) return;
    byte num_keys = rt->num_keys,
        max_keys = rt->max_keys,
        collision_index,
        temp;
    uint16_t size = rt->size,
        pile_size = rt->pile_size;
    PrintLine ('_');
    printf ("| RecordTable: %p\n| num_keys: %u max_keys: %u  "
            "pile_size: %u  size: %u", rt, num_keys,
            max_keys, pile_size, size);
    putchar ('\n');
    putchar ('|');
    for (int i = 0; i < 79; ++i) putchar ('_');
    putchar ('\n');

    hash16_t* hashes = reinterpret_cast<hash16_t*>(reinterpret_cast<byte*>(rt) +
                                                   sizeof (RecordTable));
    uint16_t* key_offsets = reinterpret_cast<uint16_t*>(hashes + max_keys);
    byte* indexes = reinterpret_cast<byte*> (key_offsets + max_keys),
        *unsorted_indexes = indexes + max_keys,
        *collission_list = unsorted_indexes + max_keys,
        *cursor;
    char* keys = reinterpret_cast<char*> (rt) + size - 1;

    printf ("| %3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset",
            "hash_e", "hash_u", "hash_s", "index_u", "collisions");
    putchar ('|');
    for (int i = 0; i < 79; ++i)
        putchar ('_');
    putchar ('\n');

    for (int i = 0; i < max_keys; ++i) {
        // Print each record as a row.
        // @todo Change max_keys to num_keys after done debugging.
        collision_index = indexes[i];
        printf ("| %3i %9s %7u %9x %9x %9x %9u %10u: ", i,
                keys - key_offsets[i], key_offsets[i],
                Hash16 (keys - key_offsets[i]),
                hashes[unsorted_indexes[i]], hashes[i],
                unsorted_indexes[i], collision_index);

        if (collision_index != kNoCollidingRecords && i < num_keys) {
            // Print collisions.
            cursor = &collission_list[collision_index];
            temp = *cursor;
            ++cursor;
            printf ("%u", temp);
            while (temp != kNoCollidingRecords) {
                temp = *cursor;
                ++cursor;
                if (temp == kNoCollidingRecords)
                    break;
                printf (", %u", temp);
            }
        }

        putchar ('\n');
    }
    putchar ('|');
    for (int i = 0; i < 79; ++i) putchar ('_');
    putchar ('\n');

    PrintMemory (rt, size);
    putchar ('\n');
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

    /** Sets up the Room. */
    ticket_t Init () override {
        return 0;
    }

    /** Main process loop. */
    ticket_t Loop () override {
        return 0;
    }

    /** ChineseRoom operations. */
    const Member* Op (byte index, Uniprinter* io) override {
        return 0;
    }

    private:

    // 2-to-4-byte vtable pointer here in memory!
  
};

}       //< namespace Kabuki
#endif  //< CHINESEROOM_CONSOLE_H
