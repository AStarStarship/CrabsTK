/** Kabuki Software Development Kit
    @file    /.../Library/_G/SwRenderer.hpp
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

#include <KabukiSDK-Config.hpp>

#include "Cell.hpp"

namespace _G {

bool drawLine (Cell& c, Color color, float x0, float y0, float x1, float y1);
bool drawLine (Cell& c, Color color, _2D::Point one, _2D::Point two);

void drawTriangle (Cell& c, Color color, float x0, float y0, float x1, float y1, float x2, float y2);
void drawTriangle (Cell& c, Color color, _2D::Point one, _2D::Point two, _2D::Point three);

void drawTopTriangle (Cell& c, Color color, float x0, float y0, float x1, float y1, float x2, float y2);
void drawTopTriangle (Cell& c, Color color, _2D::Point one, _2D::Point two, _2D::Point three);

void drawBottomTriangle (Cell& c, Color color, float x0, float y0, float x1, float y1, float x2, float y2);
void drawBottomTriangle (Cell& c, Color color, _2D::Point one, _2D::Point two, _2D::Point three);

}   //< _G