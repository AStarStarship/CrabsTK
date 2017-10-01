/** Kabuki Theater
    @file    /.../Source/_2D/Add.h
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

#include <KabukiTheater-Config.h>

namespace _Math {

template<typename Type>
class Add
/*< Operator Add adds two numbers together.
*/
{
    public:

    Add (Type Numberator, Type Denominator);
    /*< Constructs an object with the given numberator and demonitor. */

    Type getNumberator ();
    /*< Gets the numberator. */
    
    void setNumberator (Type Value);
    /*< Sets the numberator. */

    Type getDenominator ();
    /*< Gets the denominator. */
    
    void setDenominator (Type Value);
    /*< Sets the denominator. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */

    private:

    Type numberator,           //< The numberator.
        denominator;            //< The denominator.
};

}
