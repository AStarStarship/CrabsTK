/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Entity.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D::Vector_f interface.
*/

#pragma once

#include <_2D/Rect.hpp>
#include "Cell.hpp"

namespace _G {

class _KabukiSDK Entity_f
/**  */
{
    public:
    
    Entity_f (string Name, float X = 0.0f, float Y = 0.0f, float Width = 0.0f, float Height = 0.0f);
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
