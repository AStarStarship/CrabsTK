/** Kabuki Starship
    @file       /.../Source/_Math/Matrix.h
    @author  Cale McCollough <https://calemccollough.github.io>
    @license Copyright 2016 (C) Cale McCollough <cale.mccollough@gmail.com>

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

class Matrix
/*< A matrix of numbers. */
{
    public:

    Matrix ();
    /*< Constructs an empty matrix. */

    ~Matrix ();
    /*< Destructor. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */

    private:

    /// Write me!
};

}   //  _Math
