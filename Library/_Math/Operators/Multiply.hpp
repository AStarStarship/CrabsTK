/** Kabuki Theater
    @file    /.../KabukiTheater/_2D/Fraction.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <FreeI2P.hpp>
#include <KabukiTheater-Config.hpp>

namespace _Math {

class Fraction_f
/*< A fractional number.
*/
{
    public:

    Fraction_f (float Numberator, float Denominator);
    /*< Constructs an object with the given numberator and demonitor. */

    float GetNumberator ();
    /*< Gets the numberator. */
    
    void SetNumberator (float Value);
    /*< Sets the numberator. */

    float GetDenominator ();
    /*< Gets  the denominator. */
    
    void SetDenominator (float Value);
    /*< Sets the denominator. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */

    private:

    float numberator,           //< The numberator.
        denominator;            //< The denominator.
};

}
