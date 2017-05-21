/** Kabuki Theater
    @file    /.../Source/_Math/Vect.hpp
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

#pragma once

#include <float.hpp>


#include <KabukiTheater-Config.hpp>

namespace _Math {

class _KabukiTheater_ BCD
/*< Binary coded decimal for precise math.
    This BCD class uses optional dynamic memory. The isDynamic flag if true 
    will trigger the destructor on the ptr. */
{
    public:

    enum { 
        maxDigitsFloat = 3 + FLT_MANT_DIG - FLT_MIN_EXP,
        maxDigitsDouble = 3 + DBL_MANT_DIG - DBL_MIN_EXP
    };

    BCD (float floatingPointNumber, int numDecimalPlaces = -1);
    /*< Constructs a BCD from a float. */

    BCD (double floatingPointNumber, int numDecimalPlaces = -1);
    /*< Constructs a BCD from a double. */

    ~BCD ();
    /*< Destructor destructs if dynamic memory is used. */

    float getFloat ();
    /*< Gets the BCD value as a float. */

    double getDouble ();
    /*< Gets the BCD value as a double. */

    void setValue (float Value);
    /*< Sets the BCD value to the new Value. */

    void setValue (double Value);
    /*< Sets the BCD value to the new Value. */

    bool isNegative ();
    /*< Returns true of this BCD is negative. */

    int8_t getNumWholeDigits (int index);
    /*< Gets the number of whole BCDs. */

    int8_t getNumFractionalDigits (int index);
    /*< Gets the number of fractional digits. */

    void plus (const BCD& Value);
    /*< pluss the given Value to this BCD. */

    void plus (int Value);
    /*< pluss the given Value to this BCD. */

    void plus (long Value);
    /*< pluss the given Value to this BCD. */

    void plus (float Value);
    /*< pluss the given Value to this BCD. */

    void plus (double Value);
    /*< pluss the given Value to this BCD. */

    void minus (const BCD& Value);
    /*< Subtracts the given Value to this BCD. */

    void minus (int Value);
    /*< Subtracts the given Value to this BCD. */

    void minus (long Value);
    /*< Subtracts the given Value to this BCD. */

    void minus (float Value);
    /*< Subtracts the given Value to this BCD. */

    void minus (double Value);
    /*< Subtracts the given Value to this BCD. */

    void times (const BCD& Value);
    /*< Multiples the given Value to this BCD. */

    void times (int Value);
    /*< Multiples the given Value to this BCD. */

    void times (long Value);
    /*< Multiples the given Value to this BCD. */

    void times (float Value);
    /*< Multiples the given Value to this BCD. */

    void times (double Value);
    /*< Multiples the given Value to this BCD. */

    void divide (const BCD& Value);
    /*< divides the given Value to this BCD. */

    void divide (int Value);
    /*< divides the given Value to this BCD. */

    void divide (long Value);
    /*< divides the given Value to this BCD. */

    void divide (float Value);
    /*< divides the given Value to this BCD. */

    void divide (double Value);
    /*< divides the given Value to this BCD. */

    inline void print (Terminal& io);
    /*< Prints the BCD to the terminal. */

    private:

    byte* ptr;              //< Pointer to the BCD string.

    byte isDynamic,         //< Flag for if using dynamic memory.
        numWholeDigits,     //< The number of whole digits.
        totalLength,        //< The total length of the BCD string.
        numIsNegative;      //< Flag for it is negative or not (0 for positive and '-' for negative.)
};

} // namespace _Math
