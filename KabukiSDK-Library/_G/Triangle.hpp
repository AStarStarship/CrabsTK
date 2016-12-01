/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G2D/Triangle.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#include "_2D/Triangle.h"
#include "Cell.h"

namespace _G {

class _G_API Triangle: public _2D::Triangle
/*< A simple triangle for drawing shapes with.
    We don't really care if this class is slow. It's just for simple drawing.
*/
{
    public:
    
    /**  */
    Triangle ():
        ()
    {
    }

    Triangle (_2D.Point_i initOne, _2D.Point_i initTwo, _2D.Point_i initThree):
        base (initOne, initTwo, initThree)
    {

    }

    Triangle (int x1, int y1, int x2, int y2, int x3, int y3):
        base (x1, y1, x2, y2, x3, y3);

    void Draw (const Cell& C);

    void Draw (const Cell& C, _G::Color Color);
    /*< Draws the triangle on the cell with a different color. */

    private:

    _G::Color_i color;
};
}
