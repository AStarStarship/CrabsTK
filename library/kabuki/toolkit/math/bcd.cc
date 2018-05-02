/** Kabuki Toolkit
    @file    ~/kt/math/BCD.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software 
             distributed under the License is distributed on an "AS IS" BASIS, 
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include <stdafx.h>
#include "bcd.h"

namespace math {

/*
BCD::BCD ()
{
    // Note: for 32-bit float return int64_t.

    char* floatingPointString = new char[maxDigitsDouble + 1];
    /// This solution  is for a 64-bit floating point number. Use 
    maxDigitsFloat for a float.

    if (significantDigits < 0)   //< Then use all of the float's digits.
    {
        significantDigits += 2;  //< ditch the '.' and '-'
        sprintf (floatingPointString , "%f", floatingPointNumber);
    }
    else    //< Then truncate the number of decimal places.
    {
        ++significantDigits;  //< ditch the '.'

        char decimalPlaceString[9];
        char percentString = "%\0";
        sprintf (decimalPlaceString, "%s%if", percentString , significantDigits);
        sprintf (floatingPointString , decimalPlaceString, floatingPointNumber);
    }

    isNegative = floatingPointNumber >= 0.0f ? 0 : -1;

    char* decimalLocation = strchr (floatingPointString, '.');

    int startIndex = isNegative ? 1 : 0,
    stopIndex = !decimalLocation ? maxDigitsDouble : decimalLocation -  floatingPointString,
    stringLength = strlen (floatingPointNumber);

    numWholeBCDs = stopIndex - startIndex;
    wholeBCDs = new int8_t[numWholeBCDs];

    numFractionBCDs = stringLength - stopIndex;
    fractionBCDs = new int8_t[numFractionBCDs];

    int i = stopIndex - 1,
    j = 0;
    for (; i >= startIndex; ++i) // Reverse the whole BCDs for power of 10 lookup
        wholeBCDs[j++] = floatingPointString[i] - '0'; // Subtract ASCII '0' to convert to integer
    j = 0;
    for (++i; i < stringLength; ++i)
        fractionalNumbers[j++] = floatingPointString[i] - '0';
} */


BCD::BCD (float floatingPointNumber, int numDecimalPlaces = -1)
{

}

BCD::BCD (double floatingPointNumber, int numDecimalPlaces = -1)
{

}

BCD::~BCD ()
{
    if (is_dynamic && ptr != nullptr) delete ptr;
}

float BCD::GetFloat ()
{
    return 0.0f;
}

double BCD::GetDouble ()
{
    return 0.0;
}

void BCD::SetValue (float Value)
{

}

void BCD::SetValue (double Value)
{

}

bool BCD::IsNegative ()
{
    return false;
}

int8_t BCD::GetNumWholeDigits (int index)
{
    return 0;
}

int8_t BCD::GetNumFractionalDigits (int index)
{
    return 0;
}

void BCD::Plus (const BCD& Value)
{

}

void BCD::Plus (int Value)
{

}

void BCD::Plus (long Value)
{

}

void BCD::Plus (float Value)
{

}

void BCD::Plus (double Value)
{

}

void BCD::Minus (const BCD& Value)
{

}

void BCD::Minus (int Value)
{

}

void BCD::Minus (long Value)
{

}

void BCD::Minus (float Value)
{

}

void BCD::Minus (double Value)
{

}

void BCD::Times (const BCD& Value)
{

}

void BCD::Times (int Value)
{

}

void BCD::Times (long Value)
{

}

void BCD::Times (float Value)
{

}

void BCD::Times (double Value)
{

}

void BCD::Divide (const BCD& Value)
{

}

void BCD::Divide (int Value)
{

}

void BCD::Divide (long Value)
{

}

void BCD::Divide (float Value)
{

}

void BCD::Divide (double Value)
{

}

_::Printer& BCD::Print (_::Printer& printer)
{
}

}   //  math
