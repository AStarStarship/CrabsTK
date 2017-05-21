/** Kabuki Theater
    @file    /.../Source-Impl/_Math/Converter.cpp
    @author  Cale McCollough <https://calemccollough.github.io>
    @author  Cale McCollough <cale.mccollough@gmail.com>
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

#include "_Math/Hex.hpp"

namespace _Math {

char hexToChar (byte Value)
{
    if (Value > 15) return '!';
    if (Value > 10) return Value + ('a' - 10);
    return Value + '0';
}

void hexToString (char* Result, int Value)
{
    if (Result == nullptr) return;
    ; // Write me!
}

void hexToString (char* Result, uint8_t Value)
{ 
    if (Result == nullptr) return;
    ; // Write me!
}

int hexToToInt (string binaryHex)
{/*
    if (binaryHex.length () > 4)
        return 15;
    
    int amount=128,
        Value = 0;
    
    for (int i = 0; i < 4; i++)
    {
        if (binaryHex.opconst char* (i,i+1).equals ("1"))
        Value += amount;
        amount/=2;
    }
    return amount;
    */
    return 0;
}

int hexToInt (char Value)
{
    switch (Value)
    {
        case '0':   return 0;
        case '1':   return 1;
        case '2':   return 2;
        case '3':   return 3;
        case '4':   return 4;
        case '5':   return 5;
        case '6':   return 6;
        case '7':   return 7;
        case '8':   return 8;
        case '9':   return 9;
        case 'A':   return 10;
        case 'B':   return 11;
        case 'C':   return 12;
        case 'D':   return 13;
        case 'E':   return 14;
        case 'F':   return 15;
        case 'a':   return 10;
        case 'b':   return 11;
        case 'c':   return 12;
        case 'd':   return 13;
        case 'e':   return 14;
        case 'f':   return 15;
        default:    return '!';
    }
    return 0;
}

}   //  _Math
