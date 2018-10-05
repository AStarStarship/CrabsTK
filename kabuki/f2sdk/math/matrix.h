/** Kabuki Toolkit
    @file    ~/kt/math/matrix.h
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

class Matrix
/*< A matrix of numbers. */
{
    public:

    Matrix ();
    /*< Constructs an empty matrix. */

    ~Matrix ();
    /*< Destructor. */

    _::Printer& Print (_::Printer&& printer);
    /*< Prints this object to the terminal. */

    private:

    /// Write me!
};

}       //< namespace _
