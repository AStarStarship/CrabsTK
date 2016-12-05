/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Line.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _G::Line interface.
*/

#pragma once

#include "Cell.hpp"

namespace _G
{
/**  */
class _KabukiSDK Line : public Entity
{
    public:
    
    Line (Point startingPoint, Point endingPoint);

    static void drawRainbowLineLine (Cell&c, int startX, int startY, int endX, int endY);

    void setColor (Color newColor);

    void drawLine (Cell&c);
    
    private:
    
    Color color;
    
    int width;          //< The width of the line.
};

}   //< _G
