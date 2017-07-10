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

/*< Creates a buffer from dynamic memory. */
template<typename T, typename U>
static T* New (U const size, U min_size) {
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

template<uint_t kBufferNumber, uint_t kBufferSize>
inline byte* GetBuffer () {
    static byte buffer[kBufferSize];
    return buffer;
}

/** Creates a  
template<typename T, uint_t kMinSize>
static T* Construct (uint_t size) {
    byte* buffer = NewBuffer (size, kMinSize)
    if (buffer == nullptr) return nullptr;
    T* t = nullptr;//new (buffer) T (); //< This line wont work for some reason???
    return t;
}

template<typename T, uint_t kMinSize>
inline T* Construct (byte* buffer, uint_t size) {
    if (size < kMinSize) return nullptr;
    if (buffer == nullptr) return nullptr;
#if DEBUG
    memset (buffer, '\0', size);
#endif
    T* t = nullptr;//new (buffer) T (); //< This line wont work for some reason???
    return t;
}*/

template<typename T>
inline T* PointTo (void* base, uint_t offset) {
    return reinterpret_cast<T*>(reinterpret_cast<byte*>(base) + offset);
}

/** Deletes the given dynamic memory. */
inline void Destroy (void* t) {
    if (t == nullptr) return;
    delete[] reinterpret_cast<byte*> (t);
}

/** Calculates the used ring buffer space.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size The size of the buffer.
*/
inline uint_t CalcRingSegmentLength (byte* start, byte* stop, uint_t size) {
    uint_t delta_start_stop = static_cast<uint> (stop - start);
    return start < stop ? size - delta_start_stop : delta_start_stop;
}

/** Calculates the space left in the given ring buffer.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer.
*/
inline uint_t CalcRingBufferSpace (byte* start, byte* stop, uint_t size) {
    uint_t delta_start_stop = static_cast<uint> (stop - start);
    return start < stop ? delta_start_stop : size - delta_start_stop;
}

/** Returns the number to add to word-align the given pointer to a uint_t-bit 
    boundary. */
template<typename T>
inline uintptr_t WordAlignOffset (void* ptr) {
    uintptr_t value = reinterpret_cast<uintptr_t> (ptr),
        mask = sizeof (T) - 1;
    return (value & mask);
}

/** Hashes a single char. */
template<typename THash>
inline THash PrimeHash (char c, THash hash) {
    return hash * (kHashPrime + c);
}

/** Hashes the given string using the primeHash function. */
template<typename THash>
inline THash PrimeHash (const char* s, THash hash = kHashSeed) {
    char c = *s;
    while (c) {
        hash = PrimeHash<THash> (c, hash);
        ++s;
        c = *s;
    }
    return hash;
}

/** Copies a string from the source to the destination. */
inline void CopyString (char* dest, const char* source) {
    try
    {
        char c = *source;
        ++source;
        while (c != 0) {
            //putchar (c);
            //PrintHex ("", dest);
            //putchar ('\n');
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
        //system ("PAUSE");
    }
    //putchar ('\n');
}

/** Writes a block of memory to a file. */
inline void WriteToFile (const char* filename, void* address, size_t size) {
    std::ofstream f;
    f.open (filename);
    byte* ptr = reinterpret_cast<byte*> (address);

    if (!f.is_open ())
    {
        printf ("Error: Could not open file for writing!\n");
    }
    for (size_t i = 0; i < size; ++i)
        f << *ptr;
    ++ptr;
    f.close ();
}

/** Converts a single byte a one-byte hex representation. */
inline char NibbleToLowerCaseHex (byte b) {
    if (b > 15) return 'f';
    //printf ("Input: %u ", b);
    if (b >  9)
    {
        //printf ("Output: %c\n", (b & 0xf) + ('a' - 10));
        return b + ('a' - 10);
    }
    //printf ("Output: %c\n", b + '0');
    return b + '0';
}

/** Converts a single byte a one-byte hex representation. */
inline char NibbleToUpperCaseHex (byte b) {
    if (b > 15) return 'F';
    if (b >  9) return b + ('A' - 10);
    return b + '0';
}

/** Converts a single byte a two-byte hex representation. */
inline uint16_t ToLowerCaseHex (byte b) {
    //PrintLine ();
    //PrintNumberLine (b);
    //PrintLine ();

    //printf ("toLowerCaseHex: %u ", b);
    uint16_t value = NibbleToLowerCaseHex (b >> 4);
    //printf (" output << 8: %c ", (char) (value));
    value = value << 8;
    value |= NibbleToLowerCaseHex (b & 0xf);
    //printf (" output: %c\n", (char)nibbleToLowerCaseHex (b));
    return value;
}

/** Converts a single byte a two-byte hex representation. */
inline uint16_t ToUpperCaseHex (byte b) {
    uint16_t value = NibbleToUpperCaseHex (b >> 4);
    value = value << 8;
    value |= NibbleToUpperCaseHex (b & 0xf);
    return value;
}

/** Converts a single hex char a byte.
    @return Returns -1 if c is not a hex char.
*/
inline int ToByte (char c) {
    //printf ("toByte (char c): %c ", c);
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
    @return Returns -1 if c is not a hex char.
*/
inline int ToByte (uint16_t h) {
    //printf ("toByte (uint16_t c): %c%c\n", (char) (h << 8), (char)h);
    int lowerValue = ToByte (static_cast<char> (h));
    //printf (" lowerValue: %i \n", lowerValue);
    if (lowerValue < 0) return -1;

    int upperValue = ToByte (static_cast<char> (h >> 8));
    if (upperValue < 0) return -1;
    //printf (" upperValue: %i \nlowerValue | (upperValue << 4): %i\n",
    //	upperValue, lowerValue | (upperValue << 4));

    //PrintLine ();
    return lowerValue | (upperValue << 4);
}

/** Copies the given */
inline void CopyByteArrayToClipBoard (void* address, size_t size) {

    /*
    #include <conio.h>
    #include <stdio.h>
    #include <cstdlib>
    #include <Windows.h> // use < > for all system and library headers
    #include <winuser.h>
    #include <cmath>
    #include <iostream>
    #include <iomanip>
    #include <complex>
    #include <string>

    // 2. forward declaration (also see later)
    void toClipboard (HWND hwnd, const std::string &s);

    using namespace std;

    int main (){

        string AAA;

        cout <<"Please enter sentence: "; cin >> AAA;
        cout << endl;
        cout << endl;
        cout << "This has been copied to the clipboard: ";
        cout << AAA << endl;
        // 1. strlen takes a const char*, so have to call the strings c_str () method
        // (but it would be better to use len = AAA.length () instead)
        size_t len = strlen (AAA.c_str ());
        cout << len << " char (s)" << endl;
        // get desktop windows and the call toClipboard
        toClipboard (hwnd, AAA);
        cin.clear ();
        cin.ignore (255, '\n');
        cin.get ();

        return 0;
    }

    // 2. declare functions at file scope
    void toClipboard (HWND hwnd, const std::string &s){
    }
    #if PLATFORM == MINGW
    HWND hwnd = GetDesktopWindow ();
    OpenClipboard (hwnd);
    EmptyClipboard ();
    HGLOBAL hg = GlobalAlloc (GMEM_MOVEABLE, size);
    if (!hg)
    {
        CloseClipboard ();
        return;
    }
    memcpy (GlobalLock (hg), address, size);
    GlobalUnlock (hg);
    SetClipboardData (CF_TEXT, hg);
    CloseClipboard ();
    GlobalFree (hg);
    #elif PLATFORM == _OSX_
    #error Unsupported platform because you're OS is whack!
    #endif
    */
}

/** Calculates the difference between the start and stop GetAddresses. */
inline uintptr_t Diff (void* start, void* stop) {
    return static_cast<uintptr_t>(reinterpret_cast<byte*> (stop) -
        reinterpret_cast<byte*> (start));
}

/** Calculates the difference between the start and stop GetAddresses. */
//inline uintptr_t Diff (void* start, uint_t offset) {
//    return static_cast<uintptr_t>(reinterpret_cast<byte*> (stop) -
//                                  reinterpret_cast<byte*> (start));
//}

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

/** Destroys the duplicate string created by const char* duplicate
    (const char*). */
inline void DestroyDuplicate (const char* s) {
    if (s == nullptr) return;
}

}       //< namespace _

#endif  //< CHINESEROOM_UTILS_H
