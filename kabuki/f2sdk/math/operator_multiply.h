/** Kabuki Toolkit
    @file    ~kt/math/operator_multiply.h
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

/** A fractional number.
*/
class Multiply
{
    public:

    /** Constructs an object with the given numberator and demonitor. */
    Multiply (float Numberator, float Denominator);

    /** Gets the numberator. */
    float GetNumberator ();

    /** Sets the numberator. */
    void SetNumberator (float Value);

    /** Gets  the denominator. */
    float GetDenominator ();

    /** Sets the denominator. */
    void SetDenominator (float Value);

    /** Prints this object to the terminal. */
    _::Printer& Print (_::Printer& printer);

    private:

    float numberator,   //< The numberator.
          denominator;  //< The denominator.
};

}
