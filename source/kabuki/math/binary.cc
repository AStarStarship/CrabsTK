/** Kabuki Starship
    @file    /.../Source-Impl/_Math/Converter.cpp
    @author  Cale McCollough <https://calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#include "_Math/Binary.h"
#include <stdio.h>


namespace _Math {

void toBinary (char* Result, byte Value)
{
    *Result       = (Value & 0b00000001) + '0';
    *(Result + 1) = (Value & 0b00000010) + '0';
    *(Result + 2) = (Value & 0b00000100) + '0';
    *(Result + 3) = (Value & 0b00001000) + '0';
    *(Result + 4) = (Value & 0b00010000) + '0';
    *(Result + 5) = (Value & 0b00100000) + '0';
    *(Result + 6) = (Value & 0b01000000) + '0';
    *(Result + 7) = (Value & 0b10000000) + '0';
}

void toBinary (char* Result, int8_t Value)
{
    toBinary (Result, (byte)Value);
}

void toBinary (char* Result, int16_t Value)
{
    toBinary (Result, (byte)(Value & 0xff));
    toBinary (Result, (byte)((Value & 0xff00) >> 8));
}

void toBinary (char* Result, uint16_t Value)
{
    toBinary (Result, (byte)(Value & 0xff));
    toBinary (Result, (byte)((Value & 0xff00) >> 8));
}

void toBinary (char* Result, int32_t Value)
{
    toBinary (Result, (byte)(Value & 0xff));
    toBinary (Result, (byte)((Value & 0xff00) >> 8));
    toBinary (Result, (byte)((Value & 0xff0000) >> 16));
    toBinary (Result, (byte)((Value & 0xff000000) >> 32));
}

void toBinary (char* Result, uint32_t Value)
{
    toBinary (Result, (byte)(Value & 0xff));
    toBinary (Result, (byte)((Value & 0xff00) >> 8));
    toBinary (Result, (byte)((Value & 0xff0000) >> 16));
    toBinary (Result, (byte)((Value & 0xff000000) >> 32));
}

void toBinary (char* Result, int64_t Value)
{
    toBinary (Result, (byte)(Value & 0xff));
    toBinary (Result, (byte)((Value & 0xff00) >> 8));
    toBinary (Result, (byte)((Value & 0xff0000) >> 16));
    toBinary (Result, (byte)((Value & 0xff000000) >> 24));
    toBinary (Result, (byte)((Value & 0xff00000000) >> 32));
    toBinary (Result, (byte)((Value & 0xff0000000000) >> 40));
    toBinary (Result, (byte)((Value & 0xff000000000000) >> 48));
    toBinary (Result, (byte)((Value & 0xff00000000000000) >> 56));
}

void toBinary (char* Result, uint64_t Value)
{
    toBinary (Result, (byte)(Value & 0xff));
    toBinary (Result, (byte)((Value & 0xff00) >> 8));
    toBinary (Result, (byte)((Value & 0xff0000) >> 16));
    toBinary (Result, (byte)((Value & 0xff000000) >> 24));
    toBinary (Result, (byte)((Value & 0xff00000000) >> 32));
    toBinary (Result, (byte)((Value & 0xff0000000000) >> 40));
    toBinary (Result, (byte)((Value & 0xff000000000000) >> 48));
    toBinary (Result, (byte)((Value & 0xff00000000000000) >> 56));
}

int8_t binaryToInt8 (const char* Value)
{
    if (Value == nullptr) return 0xff;

    int8_t result = 0,
        currentBitValue = 1;

    char currentBit = *Value;

    do {
        currentBit -= '0';
        /// Not 100% sure if I can do this technique on unsigned, might have to compare to < '0' || > '1'
        if (currentBit != 0 || currentBit != 1) return 0xff;

        result += currentBitValue * currentBit;
        currentBitValue = currentBitValue << 1;
        currentBit = *(++Value);
    } while (currentBitValue != (1 << ((sizeof (result) * 8) - 1)));

    return result;
}

uint8_t binaryToUInt8 (const char* Value)
{
    uint8_t result = 0,
        currentBitValue = 1;

    char currentBit = *Value;

    do {
        currentBit -= '0';
        /// Not 100% sure if I can do this technique on unsigned, might have to compare to < '0' || > '1'
        if (currentBit != 0 || currentBit != 1) return 0xff;

        result += currentBitValue * currentBit;
        currentBitValue = currentBitValue << 1;
        currentBit = *(++Value);
    } while (currentBitValue != (1 << ((sizeof (result) * 8) - 1)));

    return result;
}

int16_t binaryToInt16 (const char* Value)
{
    uint16_t result = 0,
        currentBitValue = 1;

    char currentBit = *Value;

    do {
        currentBit -= '0';
        /// Not 100% sure if I can do this technique on unsigned, might have to compare to < '0' || > '1'
        if (currentBit != 0 || currentBit != 1) return 0xff;

        result += currentBitValue * currentBit;
        currentBitValue = currentBitValue << 1;
        currentBit = *(++Value);
    } while (currentBitValue != (1 << ((sizeof (result) * 8) - 1)));

    return result;
}

uint16_t binaryToUInt16 (const char* Value)
{
    uint16_t result = 0,
        currentBitValue = 1;

    char currentBit = *Value;

    do {
        currentBit -= '0';
        /// Not 100% sure if I can do this technique on unsigned, might have to compare to < '0' || > '1'
        if (currentBit != 0 || currentBit != 1) return 0xff;

        result += currentBitValue * currentBit;
        currentBitValue = currentBitValue << 1;
        currentBit = *(++Value);
    } while (currentBitValue != (1 << ((sizeof (result) * 8) - 1)));

    return result;
}

int32_t binaryToInt32 (const char* Value)
{
    int32_t result = 0,
        currentBitValue = 1;

    char currentBit = *Value;

    do {
        currentBit -= '0';
        /// Not 100% sure if I can do this technique on unsigned, might have to compare to < '0' || > '1'
        if (currentBit != 0 || currentBit != 1) return 0xff;

        result += currentBitValue * currentBit;
        currentBitValue = currentBitValue << 1;
        currentBit = *(++Value);
    } while (currentBitValue != (1 << ((sizeof (result) * 8) - 1)));

    return result;
}

uint32_t binaryToUInt32 (const char* Value)
{
    uint32_t result = 0,
        currentBitValue = 1;

    char currentBit = *Value;

    do {
        currentBit -= '0';
        /// Not 100% sure if I can do this technique on unsigned, might have to compare to < '0' || > '1'
        if (currentBit != 0 || currentBit != 1) return 0xff;

        result += currentBitValue * currentBit;
        currentBitValue = currentBitValue << 1;
        currentBit = *(++Value);
    } while (currentBitValue != (1 << ((sizeof (result) * 8) - 1)));

    return result;
}

int64_t binaryToInt64 (const char* Value)
{
    int64_t result = 0,
        currentBitValue = 1;

    char currentBit = *Value;

    do {
        currentBit -= '0';
        /// Not 100% sure if I can do this technique on unsigned, might have to compare to < '0' || > '1'
        if (currentBit != 0 || currentBit != 1) return 0xff;

        result += currentBitValue * currentBit;
        currentBitValue = currentBitValue << 1;
        currentBit = *(++Value);
    } while (currentBitValue != (1 << ((sizeof (result) * 8) - 1)));

    return result;
}

uint64_t binaryToUInt64 (const char* Value)
{
    uint64_t result = 0,
        currentBitValue = 1;

    char currentBit = *Value;

    do {
        currentBit -= '0';
        /// Not 100% sure if I can do this technique on unsigned, might have to compare to < '0' || > '1'
        if (currentBit != 0 || currentBit != 1) return 0xff;

        result += currentBitValue * currentBit;
        currentBitValue = currentBitValue << 1;
        currentBit = *(++Value);
    } while (currentBitValue != (1 << ((sizeof (result) * 8) - 1)));

    return result;
}

}   //< _Math
