/** Kabuki Toolkit
    @file    ~/kt/math/Vect.h
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

#pragma once

#include <pch.h>

#include "config.h"

namespace _ {

/** Binary coded decimal for precise math.
    This BCD class uses optional dynamic memory. The is_dynamic flag if true 
    will trigger the destructor on the ptr. */
class KABUKI_TOOLKIT BCD {
    public:

    enum { 
        maxDigitsFloat = 3 + FLT_MANT_DIG - FLT_MIN_EXP,
        maxDigitsDouble = 3 + DBL_MANT_DIG - DBL_MIN_EXP
    };

    /** Constructs a BCD from a float. */
    BCD (float floatingPointNumber, int numDecimalPlaces = -1);

    /** Constructs a BCD from a double. */
    BCD (double floatingPointNumber, int numDecimalPlaces = -1);

    /** Destructor destructs if dynamic memory is used. */
    ~BCD ();

    /** Gets the BCD value as a float. */
    float GetFloat ();

    /** Gets the BCD value as a double. */
    double GetDouble ();

    /** Sets the BCD value to the new Value. */
    void SetValue (float value);

    /** Sets the BCD value to the new Value. */
    void SetValue (double value);

    /** Returns true of this BCD is negative. */
    bool IsNegative ();

    /** Gets the number of whole BCDs. */
    int8_t GetNumWholeDigits (int index);

    /** Gets the number of fractional digits. */
    int8_t GetNumFractionalDigits (int index);

    /** Adds the given Value to this BCD. */
    void Plus (const BCD& value);

    /** Adds the given Value to this BCD. */
    void Plus (int value);

    /** Adds the given Value to this BCD. */
    void Plus (long value);

    /** Adds the given Value to this BCD. */
    void Plus (float value);

    /** Adds the given Value to this BCD. */
    void Plus (double value);

    /** Subtracts the given Value to this BCD. */
    void Minus (const BCD& value);

    /** Subtracts the given Value to this BCD. */
    void Minus (int value);

    /** Subtracts the given Value to this BCD. */
    void Minus (long value);

    /** Subtracts the given Value to this BCD. */
    void Minus (float value);

    /** Subtracts the given Value to this BCD. */
    void Minus (double value);

    /** Multiples the given Value to this BCD. */
    void Times (const BCD& value);

    /** Multiples the given Value to this BCD. */
    void Times (int value);

    /** Multiples the given Value to this BCD. */
    void Times (long value);

    /** Multiples the given Value to this BCD. */
    void Times (float value);

    /** Multiples the given Value to this BCD. */
    void Times (double value);

    /** Divides the given Value to this BCD. */
    void Divide (const BCD& value);

    /** Divides the given Value to this BCD. */
    void Divide (int value);

    /** Divides the given Value to this BCD. */
    void Divide (long value);

    /** Divides the given Value to this BCD. */
    void Divide (float value);

    /** Divides the given Value to this BCD. */
    void Divide (double value);

    /** Prints the BCD to the terminal. */
    _::Printer& Print (_::Printer& printer);

    private:

    char  is_dynamic_,        //< Flag for if using dynamic memory.
          whole_digit_count_, //< The number of whole digits.
          digit_count_,       //< The total length of the BCD string.
          sign_,              //< Flag for it is negative ('-') or not (0).
          buffer_;
};

} // namespace math
