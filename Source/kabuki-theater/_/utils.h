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
    static byte buffer[kBufferSize + 1];
    return buffer;
}

/** Creates/Gets one of n static buffers of the specified size. */
template<uint_t kBufferNumber, uint_t kBufferSize>
inline byte* Buffer () {
    static byte buffer[kBufferSize + 1];
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

}       //< namespace _

#endif  //< CHINESEROOM_UTILS_H
