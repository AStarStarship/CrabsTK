/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_G/Line.h
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

#include "Cell.h"

namespace _G
{
/**  */
class _KabukiTheater_ Line : public Entity
{
    public:
    
    Line (Point startingPoint, Point endingPoint);

    static void drawRainbowLineLine (Cell&c, int startX, int startY, int endX, int endY);

    void setColor (Color newColor);

    void drawLine (Cell&c);
    
    private:
    
    Color color;
    
    int width;          //< The width of the line.
};

}   //< _G
