/** Kabuki Theater
    @file       /.../Source/KabukiTheater-Impl/_G/Rect.hpp
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

#include <string>

#include <_Math/Point.hpp>
#include <_Math/Rect.hpp>

namespace _G {
    
template<typename Type>
class _KabukiTheater_ Rect: public _2D::Rect<Type>
/*< A graphical 2D rectangle. */
{
    public:
    
    Rect (_2D::Point_f relativePosition, _2D::Point_f dimensions);
    /*<  */

    void draw (Cell& C);
    /*<  */

    void fill (Cell& C);
    /*<  */
    
    inline void print (Terminal& io);
    /*< Prints this object to a terminal. */
    
    private:

    Color bgColor,      //< The background color.
        fillColor;      //< The fill color.
};
}   //  _G
