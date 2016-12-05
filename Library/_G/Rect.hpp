/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Rect.hpp
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D::Vector_f interface.
*/

#include <string>

#include "_2D/Point.hpp"
#include "_2D/Rect.hpp"

namespace _G {
    
class _KabukiSDK Rect_f: public _2D::Rect_f
/*< A graphical 2D rectangle. */
{
    public:
    
    Rect_f (_2D::Point_f relativePosition, _2D::Point_f dimensions);
    /*<  */

    void draw (Cell& C);
    /*<  */

    void fill (Cell& C);
    /*<  */
    
    inline void print (Terminal& slot);
    /*< Prints this object to a terminal. */
    
    private:

    Color bgColor,      //< The background color.
        fillColor;      //< The fill color.
};
}   //  _G
