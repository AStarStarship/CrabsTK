/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Glyph.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D.Vector_f interface.
*/

#include "_G/Glyph.h"
using namespace _G;

long Glyph::GetUID () { return uID; }

string Glyph::GetKey ()
{
    return key;
}

string Glyph::GetDescription ()
{
    return description;
}

string Glyph::ToString ()
{
    return "";
}
