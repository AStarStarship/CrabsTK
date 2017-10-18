/** Kabuki Starship
    @file    /.../Source/KabukiTheater-Impl/_G/Entity.h
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

#include <_Math/Rect2D.h>
#include "Cell.h"

namespace _G {

template<typename Type>
class _KabukiTheater_ Entity
/**  */
{
    public:
    
    Entity_f (string Name, Type X = 0, Type Y = 0, Type Width = 0, Type Height = 0);
    /*< Constructs an entity from the given bounds and name. */

    virtual _2D::Rect_i getBounds () = 0;
    /*< Returns the bounds of the entity. */

    string getName ();
    /*< Gets the name of the string. */

    void setName  (string Name);
    /*< Sets the name of the string. */

    Rect_f& getBounds ();
    /*< Returns the rectuangular counts of the entity. */

    virtual void draw (Cell& c) = 0;
    /*<  */
};

}   //< _G
