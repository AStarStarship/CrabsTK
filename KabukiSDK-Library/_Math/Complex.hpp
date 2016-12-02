/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Math/Complex.hpp
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
#include <KabukiSDK-Config.hpp>

namespace _Math {

class _KabukiSDK Complex_f
/** An imaginary number stored as double types. */
{
    public:

    float a,        //< Vector A, imagined on the x-axis.
        b;          //< Vector B, imagined on the y-axis.

    Complex_f ();
    /*< Constructs an uninitialized imaginary number. */

    Complex_f (float initA, float initB);
    /*< Constructs an imaginary number from the given values. */

    float calcAngle ();
    /*< Gets the angle between A and B. */

    float calcMagnitude ();
    /*< Gets the magnitude of A and B. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

class _KabukiSDK Complex_d
/** An imaginary number stored as double types. */
{
    public:

    double a,      //< Vector A, imagined on the x-axis.
        b;          //< Vector B, imagined on the y-axis.

    Complex_d ();
    /*< Constructs an uninitialized imaginary number. */

    Complex_d (double initA, double initB);
    /*< Gets and sets the A value. */

    double calcAngle ();
    /*< Gets the angle between A and B. */

    double calcMagnitude ();
    /*< Gets the magnitude of A and B. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};
