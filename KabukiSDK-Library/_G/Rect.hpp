/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G2D/Rectangle.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D.Vector_f interface.
*/

#include <string>

#include "_2D/Point_f.h"
#include "_2D/Rect_f.h"

namespace _G {
    
/*<  */
class _G_API Rect_f: public _2D.Rect_f, public Shape_f
{
    public:
    
    Rect_f (_2D::Point_f relativePosition, _2D::Point_f dimensions);
    /*<  */

    static void Draw (const Cell& C, Color_i Color, int LeftEdge, int TopEdge, int Width, int Height);
    /*<  */

    static void Fill (const Cell& C, Color_i Color, int LeftEdge, int TopEdge, int Width, int Height);
    /*<  */

    static void FillClipped (const Cell& C, Color_i Color, int LeftEdge, int TopEdge, int Width, int Height);
    /*<  */

    static void DrawRounded (const Cell& C, Color_i Color, int LeftEdge, int TopEdge, int Width, int Height);
    /*<  */

    static void FillRounded (const Cell& C, Color_i Color, int LeftEdge, int TopEdge, int Width, int Height);
    /*<  */

    void Draw (const Cell& C);
    /*<  */

    void Fill (const Cell& C);
    /*<  */
    
    void Print (I2P::Terminal& Slot);
    /*< Prints this object to a terminal. */
    
    private:

    Color color;
};
}   //< namespace _G
