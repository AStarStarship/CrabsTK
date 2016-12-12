/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Layer.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the Layer class.
*/

#include <_G/Layer.hpp>

namespace _G {

void remove ()
{
    Prev.Next = Next.Next);
    Next.Prev = Prev;
}

void draw (Canvas& c)
{
    element.draw (c);
    next.draw (c);
}

}   //< _G
