/** The Chinese Room
    @version 0.x
    @file    /.../utils.h
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

#ifndef CHINESEROOM_UTILS_H
#define CHINESEROOM_UTILS_H

#include "config.h"

namespace _ {

/*< Gets a random number. */
template<typename T>
inline T GetRandom () {
    srand (time (NULL));
    T random_number = static_cast<T>(rand ());
    return random_number;
}

/*< Creates a buffer from dynamic memory. */
template<typename T, typename U>
inline T* New (U const size, U min_size) {
    if (size < min_size) return nullptr;
    byte* buffer;
    try {
        buffer = new byte[size];
#if DEBUG
        memset (buffer, '\0', size);
#endif
        return reinterpret_cast<T*>(buffer);
    } catch (...) {
        return nullptr;
    }
}
/** Deletes the given dynamic memory. */
inline void Delete (void* buffer) {
    if (buffer == nullptr) return;
    delete [] buffer;
}

/** Creates/Gets a static buffer of the specified size. */
template<uint_t kBufferSize>
inline byte* Buffer () {
    static byte buffer[kBufferSize];
    return buffer;
}

/** Creates/Gets one of n static buffers of the specified size. */
template<uint_t kBufferNumber, uint_t kBufferSize>
inline byte* Buffer () {
    static byte buffer[kBufferSize];
    return buffer;
}

/** reinterpret_cast(s) a the given base and offset to an object pointer. */
template<typename T>
inline T* PointTo (void* base, uint_t offset) {
    return reinterpret_cast<T*>(reinterpret_cast<byte*>(base) + offset);
}

/** Returns the number to add to word-align the given pointer to a uint_t-bit 
    boundary. */
template<typename T>
inline uintptr_t WordAlignOffset (void* ptr) {
    uintptr_t value = reinterpret_cast<uintptr_t> (ptr),
        mask = sizeof (T) - 1;
    return (value & mask);
}

/** Copies a string from the source to the destination. */
inline void CopyString (char* dest, const char* source) {
    try
    {
        char c = *source;
        ++source;
        while (c != 0) {
            *dest = c;
            ++dest;
            c = *source;
            ++source;
        }
        *dest = '\0';
    }
    catch (...)
    {
        printf ("\n\nError: Attempted to write out of bounds!\n\n");
    }
}

/** Converts a single byte a one-byte hex representation. */
inline byte NibbleToLowerCaseHex (byte b) {
    if (b > 15) return 'f';
    if (b >  9)
        return b + ('a' - 10);
    return b + '0';
}

/** Converts a single byte a one-byte hex representation. */
inline byte NibbleToUpperCaseHex (byte b) {
    if (b > 15) return 'F';
    if (b >  9) return b + ('A' - 10);
    return b + '0';
}

/** Converts a single byte a two-byte hex representation. */
inline uint16_t ToLowerCaseHex (byte b) {
    uint16_t value = NibbleToLowerCaseHex (b & 0xf);
    value = value << 8;
    value |= NibbleToLowerCaseHex (b >> 4);
    return value;
}

/** Converts a single byte a two-byte hex representation. */
inline uint16_t ToUpperCaseHex (byte b) {
    uint16_t value = NibbleToUpperCaseHex (b & 0xf);
    value = value << 8;
    value |= NibbleToUpperCaseHex (b >> 4);
    return value;
}

/** Converts a single hex byte a byte.
    @return Returns -1 if c is not a hex byte.
*/
inline int ToByte (byte c) {
    //printf ("toByte (byte c): %c ", c);
    if (c < '0') return -1;
    if (c >= 'a')
    {
        if (c > 'f') return -1;
        //printf ("output!: %i \n", c - ('a' - 10));
        return c - ('a' - 10);
    }
    if (c >= 'A')
    {
        if (c > 'F') return -1;
        return c - ('A' - 10);
    }
    if (c > '9') return -1;
    //printf ("output: %i \n", c - ('a' - 10));
    return c - '0';
}

/** Converts a single byte into a two-byte hex representation.
    @return Returns -1 if c is not a hex byte.
*/
inline int ToByte (uint16_t h) {
    // Note: This works but the upper and lower are switched... pay no 
    // attention to the man behind the curtain #HandWave
    //printf ("toByte (uint16_t c): %c%c\n", (byte) (h << 8), (byte)h);
    int lowerValue = ToByte (static_cast<byte> (h >> 8));
    //printf (" lowerValue: %i \n", lowerValue);
    if (lowerValue < 0) return -1;

    int upperValue = ToByte (static_cast<byte> (h));
    if (upperValue < 0) return -1;
    //printf (" upperValue: %i \nlowerValue | (upperValue << 4): %i\n",
    //	upperValue, lowerValue | (upperValue << 4));

    //PrintLine ();
    return lowerValue | (upperValue << 4);
}

/** Calculates the difference between the begin and end address. */
inline uintptr_t Diff (void* begin, void* end) {
    return static_cast<uintptr_t>(reinterpret_cast<byte*> (end) -
        reinterpret_cast<byte*> (begin));
}

/** Gets a randomly generated 32-bit hash. */
template<typename T>
inline uint_t GetRandomHash () {
    srand (time (0));
    return static_cast<T> (rand ());
}

/** Creates a duplicate copy of the given string using dynamic memory. */
inline const char* DuplicateString (const char* s) {
    size_t length = strlen (s);
    char* buffer = new char[length + 1];
    strcpy_s (buffer, length + 1, s);
    return buffer;
}

/** Destroys the duplicate string created by const byte* duplicate
    (const byte*). */
inline void DestroyDuplicate (const byte* s) {
    if (s == nullptr) return;
}

/** Hashes a single byte. */
inline hash16_t Hash16 (char c, hash16_t hash) {
    //PrintHex (c);
    hash16_t cprime = c * 65521;
    return cprime + hash;
}

/** Hashes the given string using the primeHash function. */
inline hash16_t Hash16 (const char* s, hash16_t hash = 65521) {
    byte c = *s;
    while (c) {
        hash = Hash16 (c, hash);
        ++s;
        c = *s;
    }
    return hash;
}

/** Hashes a single byte. */
inline hash32_t Hash32 (char c, hash32_t hash) {
    hash32_t cprime = c * 4294967291;
    return cprime + hash;
}

/** Hashes the given string using the primeHash function. */
inline hash16_t Hash32 (const char* s, hash32_t hash = 4294967291) {
    byte c = *s;
    while (c) {
        hash = Hash32 (c, hash);
        ++s;
        c = *s;
    }
    return hash;
}

/** Hashes a single byte. */
inline hash64_t Hash64 (char c, hash64_t hash) {
    hash64_t cprime = c * 18446744073709551557;
    return cprime + hash;
}

/** Hashes the given string using the primeHash function. */
inline hash64_t Hash64 (const char* s, hash64_t hash = 18446744073709551557) {
    byte c = *s;
    while (c) {
        hash = Hash64 (c, hash);
        ++s;
        c = *s;
    }
    return hash;
}

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
    std::cout << '|';
    for (int i = 0; i < 79; ++i) std::cout << '_';
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

}       //< namespace _

#endif  //< CHINESEROOM_UTILS_H
