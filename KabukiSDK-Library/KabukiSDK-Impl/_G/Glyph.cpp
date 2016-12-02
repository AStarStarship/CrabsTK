/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Glyph.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D::Vector_f interface.
*/

#include "_G/Glyph.hpp"
using namespace _G;

long Glyph::getUID () { return uID; }

string Glyph::getKey ()
{
    return key;
}

string Glyph::getDescription ()
{
    return description;
}

string Glyph::toString ()
{
    return "";
}
