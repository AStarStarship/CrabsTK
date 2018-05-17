/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/memory.h
    @author  Cale McCollough <https://calemccollough.github.io>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License. */

#pragma once
#include <stdafx.h>

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

#ifndef HEADER_FOR_CRABS_MEMORY
#define HEADER_FOR_CRABS_MEMORY

#include "printer.h"
#include "align.h"

namespace _ {

/** Creates a dynamic word-aligned buffer of the given size in bytes. */
KABUKI uintptr_t* MemoryCreate (uintptr_t size_bytes);

/** Destroys a dynamic buffer. */
KABUKI void MemoryDestroy (uintptr_t* buffer);

/** Returns the given pointer for use with uncooperative overloaded
    operators. */
inline const void* Pointer (const void* pointer) {
    return pointer;
}

/** Converts the pointer to a std::uintptr_t. */
inline std::uintptr_t MemoryValue (const void* value) {
    return reinterpret_cast<std::uintptr_t> (value);
}

/** Converts the std::uintptr_t to a pointer. */
inline void* MemoryValue (std::uintptr_t value) {
    return reinterpret_cast<void*> (value);
}

/** Converts the std::uintptr_t to a pointer. */
inline const void* MemoryValueConst (std::uintptr_t value) {
    return reinterpret_cast<const void*> (value);
}

inline char* MemoryAdd (void* a, void* b) {
    return (char*)((uintptr_t)a + (uintptr_t)b);
}

template<typename T>
inline bool MemoryIsNaN (T value) {
    static const T nan = (sizeof (T) == 8) 
                       ? (T)0xFF
                       : sizeof (T) == 4 ? (T)0xFFFF
                       : sizeof (T) == 2 ? (T)0xFFFFFFFF
                       : sizeof (T) == 1 ? (T)0xFFFFFFFFFFFFFFFF
                       : 0;
    return value == nan;
}

/* Returns the inverse of the given value.
For code metadata purposes. */
template<typename T>
T MemoryMax () {
    return ~(T)0;
}

/** Creates/Gets one of n static buffers of the specified size. */
template<int    kBufferNumber = 0,
         size_t kBufferSize   = kBufferSizeDefault>
inline uintptr_t* BufferN () {
    static uintptr_t buffer[(kBufferSize / sizeof (uintptr_t)) + 1];
    return buffer;
}

/** Creates/Gets one of n static buffers of the specified size. */
template<typename T = uintptr_t,
    size_t kBufferSize = kBufferSizeDefaultWords>
inline T* BufferT () {
    static T buffer[(kBufferSize / sizeof (uintptr_t)) + 1];
    return buffer;
}

/** Creates/Gets one of n static buffers of the specified size. */
template<typename T = uintptr_t,
    int    kBufferNumber = 0,
    size_t kBufferSize = kBufferSizeDefaultWords>
inline T* BufferTN () {
    static T buffer[(kBufferSize / sizeof (uintptr_t)) + 1];
    return buffer;
}

/** reinterpret_cast(string) a the given base and offset to an object pointer.
*/
template<typename T>
inline T* MemoryOffset (void* base, uint_t offset) {
    return reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

/** Calculates the difference between the begin and end address. */
inline intptr_t MemorySize (void* begin, void* end) {
    return reinterpret_cast<char*> (end) - reinterpret_cast<char*> (begin);
}

/** Calculates the difference between the begin and end address. */
inline intptr_t MemorySize (const void* begin, const void* end) {
    return reinterpret_cast<const char*> (end) -
           reinterpret_cast<const char*> (begin);
}

/** Overwrites the memory with zeros functionally identical to memset. */
KABUKI void MemoryClear (void* address, size_t size);

/** Copies the source to the target functionally identical to memcpy.
    @param  write     Beginning of the write buffer.
    @param  write_end End of the write buffer.
    @param  start     Beginning of the read buffer.
    @param  stop      End of the read buffer.
    @return Pointer to the last byte written or nil upon failure. */
KABUKI char* MemoryCopy (void* write, void* write_end, const void* start,
                         const void* stop);

/** Copies the source to the target functionally identical to memcpy.
    @param  write     Beginning of the write buffer.
    @param  write_end End of the write buffer.
    @param  read      Beginning of the read buffer.
    @param  size      Number of bytes to copy.
    @return Pointer to the last byte written or nil upon failure. */
KABUKI char* MemoryCopy (void* write, void* write_end, const void* memory,
                         size_t size);

/** Copies the source to the target functionally identical to memcpy.
    @param  write     Beginning of the write buffer.
    @param  write_end End of the write buffer.
    @param  read      Beginning of the read buffer.
    @param  size      Number of bytes to copy.
    @return Pointer to the last byte written or nil upon failure. */
KABUKI char* MemoryCopy (void* write, void* write_end, const void* read,
                         size_t size);

/** Copies the source to the target functionally identical to memcpy.
    @param  write     Beginning of the write buffer.
    @param  write_end End of the write buffer.
    @param  read      Beginning of the read buffer.
    @param  size      Number of bytes to copy.
    @return Pointer to the last byte written or nil upon failure. */
KABUKI char* MemoryCopy (void* write, void* write_end, const void* read,
                         const void* read_end, size_t size);

char* MemoryCopy (void* write, size_t write_size, const void* read,
                  size_t read_size);

/** Prints out the contents of the address to the debug stream.
    @param begin    The beginning of the read buffer.
    @param text_end The end of the read buffer.
    @param text     The beginning of the write buffer.
    @param text_end The end of the write buffer.
    @return          Null upon failure or a pointer to the byte after the last 
                     byte written. */
KABUKI char* PrintMemory (const void* begin, const void* end, char* text,
                          char* text_end, char delimiter = 0);

/** Prints out the contents of the address to the debug stream.
    @param begin    The beginning of the read buffer.
    @param text_end The end of the read buffer.
    @param text     The beginning of the write buffer.
    @param text_end The end of the write buffer.
    @return          Null upon failure or a pointer to the byte after the last 
                     byte written. */
inline char* PrintMemory (const void* begin, size_t size, char* text,
                          char* text_end, char delimiter = 0) {
    return PrintMemory (begin, reinterpret_cast<const char*> (begin) + size,
                        text, text_end, delimiter);
}

inline Printer& PrintMemory (Printer& printer, const void* begin, size_t size) {
}

/** Utility class for printing a text line with operator<<. */
class Memory {
    public:

    const char* begin, //< Begin of the socket.
              * end;   //< End of the socket.

    /** Constructor. */
    Memory (const char* begin, const char* end);

    /** Constructor. */
    Memory (const char* begin, intptr_t size);
};

}       //< namespace _

inline _::Printer& operator<< (_::Printer& print, _::Memory memory) {
    char* cursor = _::PrintMemory (memory.begin, memory.end, print.cursor, print.end);
    if (!cursor) {
        return print;
    }
    print.cursor = cursor;
    return print;
}

#endif  //< HEADER_FOR_CRABS_MEMORY
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
