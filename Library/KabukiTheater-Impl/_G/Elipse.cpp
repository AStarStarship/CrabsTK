/** Kabuki Theater
    @file       /.../Source/KabukiTheater-Impl/_G/Elipse.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D::Vector_f interface.
*/

#include "_G/Elipse_f.hpp"

namespace _G {

Elipse_f::Elipse_f (_2D::Point_f position, _2D::Vect_f dimensions)
:   Entity_f (relativePosition, dimensions)
{

}

float Elipse_f::getRX ()
{
    return rx;
}

void Elipse_f::setRX (float value)
{
    if (value < 0.0) 
    {
        rx = value * -1.0;
        return;
    }
    rx = value;
}

float Elipse_f::getRY ()
{
    return ry;
}

void Elipse_f::setRY (float value)
{
    if (value < 0.0) 
    {
        ry = value * -1.0;
        return;
    }
    ry = value;
}

}   //< _G
