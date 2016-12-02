/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G2D/Triangle.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#pragma once

#include <_2D/Point.hpp>
#include "Cell.hpp"

namespace _G {

class _KabukiSDK Triangle
/*< A simple triangle for drawing shapes with.
    We don't really care if this class is slow. It's just for simple drawing.
*/
{
    public:
    
    /**  */
    Triangle ();

    Triangle (_2D::Point_i initOne, _2D::Point_i initTwo, _2D::Point_i initThree);

    Triangle (int x0, int y0, int x1, int y1, int x2, int y2);

    void draw (const Cell& C);

    void draw (const Cell& C, _G::Color Color);
    /*< Draws the triangle on the cell with a different color. */

    private:

    _G::Color_i color;
};

}
