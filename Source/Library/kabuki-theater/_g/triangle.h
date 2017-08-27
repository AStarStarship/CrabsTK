/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_G/Triangle.hpp
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

#include <_Math/Point2D.hpp>
#include "Cell.hpp"

namespace _G {

template<typename Type>
class _KabukiTheater_ Triangle
/*< A simple triangle for drawing shapes with.
    We don't really care if this class is slow. It's just for simple drawing.
*/
{
    public:
    
    Triangle ();
    /*<  */

    Triangle (_2D::Point initOne, _2D::Point initTwo, _2D::Point initThree);

    Triangle (Type x0, Type y0, Type x1, Type y1, Type x2, Type y2);

    void draw (Cell& C);

    void draw (Cell& C, _G::Color Color);
    /*< Draws the triangle on the cell with a different color. */

    private:

    _G::Color color;
};

}
