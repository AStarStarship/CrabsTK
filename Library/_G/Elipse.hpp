/** Kabuki Theater
    @file       /.../Source/KabukiTheater-Impl/_G/Elipse.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt.
*/

#pragma once

#include "Cell.hpp"
#include <_2D/Point.hpp>

namespace _G {

class _KabukiTheater Elipse_f
/**  */
{
    public:

    float x,        //< The left edge.
        y,          //< The top edge.
    
    Elipse_f (_2D::Point_f relativePosition, _2D::Vect_f dimensions);
    /*< Constructor. */

    float getRX ();
    /*< Gets the radius of the rectangle in the x direction. */

    void setRX (float value);
    /*< Sets the radius of the rectangle in the x direction. 
        @warning If value < 0.0, value will be multiplied by -1.0. */

    float getRY ();
    /*< Gets the radius of the rectangle in the x direction. */

    void setRY (float value);
    /*< Sets the radius of the rectangle in the x direction.
        @warning If value < 0.0, value will be multiplied by -1.0. */

    inline void print (Terminal& io);
    /*< Prints this object to a terminal. */

    private:

    float rx,       //< The radius of the rectangle in the x direction.
        ry;         //< The radius of the rectangle in the y direction.
};

}   //< _G
