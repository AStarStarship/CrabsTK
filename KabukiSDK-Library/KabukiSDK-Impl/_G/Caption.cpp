/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Caption.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#include "_G/Cell.h"
#include "_G/Caption_f.h"

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

void Caption::SetText (const std::string& newString)
{
}

void Caption::Caption_f::SetVerticalMargin (int Value)
{
    if (Value < 0) return;
    tbMargin = Value;
}

void Caption::Caption_f::SetHorizontalMargin (int Value)
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

void Caption_f::Draw (const Cell& C)
{
    if (isVisible)
     return;

    Rectangle.Draw (C);
    captionText.Draw (C);
}

std::string Caption_f::ToString ()
{
    return captionText.ToString ();
}
}
