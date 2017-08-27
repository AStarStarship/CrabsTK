/** Kabuki Theater
    @file       /.../Source/KabukiTheater-Impl/_G/Caption.hpp
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

#include "_G/Cell.hpp"
#include "_G/Caption_f.hpp"

namespace _G {

Caption::Caption () :
    Entity_f (0,0,0,0)
{
    lrMargin = 5;
    tbMargin = 5;
    
    captionText = new TextArea (0,0,0,0);
    SetBorderMode (Cell.FillRoundCorners);
}

Caption::Caption (int captOriginX, int captOriginY) :
    Entity_f (captOriginX, captOriginY,0,0)
{
    origin.x = captOriginX;
    origin.y = captOriginY;
    captionText = new TextArea (origin.x, origin.y,0,0);
    SetBorderMode (Cell.FillRoundCorners);
}

void Caption::setText (const string& newString)
{
}

void Caption::Caption_f::setVerticalMargin (int Value)
{
    if (Value < 0) return;
    tbMargin = Value;
}

void Caption::Caption_f::setHorizontalMargin (int Value)
{
    if (Value < 0) return;
    lrMargin = Value;
}

void Caption_f::Update ()
{
    captionText.Set_left_edge (Left_edge_int () + lrMargin;
    captionText.Set_top_edge (Top_edge_int () + tbMargin;
    SetWidth (captionText.Left_edge_int () + lrMargin);
    SetHeight (captionText.Bottom_edge_int () + tbMargin);
}

void Caption_f::draw (Cell& c)
{
    if (isVisible)
     return;

    Rectangle.draw (C);
    captionText.draw (C);
}

string Caption_f::toString ()
{
    return captionText.ToString ();
}
}
