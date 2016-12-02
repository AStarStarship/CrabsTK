/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Cell.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _G::Cell class _KabukiSDK.
*/

#pragma once

#include "Color.hpp"
#include "Font.hpp"
#include "Rect.hpp"

#include <string>

namespace _G {
    
class _KabukiSDK Cell_f
/** A graphics cell to draw on. */
{
    public:
    
    Cell_f (int LeftEdge, int TopEdge, int Width, int Height);
    /*<  */

    void draw (string S, int X, int Y);
    /*<  */

    void draw (string S, int X, int Y, float Angle);
    /*<  */

    inline void setColor (Color_i Color);
    /*< Sets the _G::Color_i_i. */
    
    inline void drawImage (Image& I, int LeftEdge, int TopEdge
    /*<  */
    
    inline void drawImage (Image& I, int LeftEdge, int TopEdge, int newWidth, int newHeight);
    /*<  */

    inline void drawTriangle (_G::Color color, int x1, int y1, int x2, int y2, int x3, int y3);
    /*< draws a triangle on the screen */

    inline void fillTriangle (_G::Color color, int x1, int y1, int x2, int y2, int x3, int y3);
    /*< Fills the triangle with the given color. */

    inline void drawBottomTriangle (_G::Color color, int x1, int y1, int x2, int y2, int x3, int y3);
    /*< Fills the bottom of the given triangle with the given color. */
    
    inline void drawLine (float StartX, float StartY, float EndX, float EndY);
    /*<  */
    
    inline void drawLine (Color_i Color, int StartX, int StartY, int EndX, int EndY);

    /*<  */
    
    inline void drawRectangle (Color_i Color, int LeftEdge, int TopEdge, int Width, int Height);
    /*<  */
    
    inline void drawRectangle (Color_i Color, int LeftEdge, int TopEdge, int Width, int Height, int thickness);
    /*<  */
    
    inline void drawOval (int X, int Y, int Width, int Height);
    /*<  */
    
    inline void drawRoundedRectangle (int X, int Y, int Width, int Height, int ArcWidth, int ArcHeight);
    /*<  */
    
    inline void clearArea (int X, int Y, int Width, int Height);
    /*<  */
    
    inline void clipArea (int X, int Y, int Width, int Height
    /*<  */
    
    inline void copyArea (int X, int Y, int Width, int Height, int DX, int DY);
    /*<  */

    inline void drawArc (int X, int Y, int Width, int Height, int StartAngle, int ArcAngle);
    /*<  */
    
    inline void drawText (string thisText, int offset, int length, int X, int Y);
    /*<  */
    
    inline void drawPolygon (int* XPoints, int* YPoints, int NPoints);
    /*<  */
    
    inline void drawPolYline (int* XPoints, int* YPoints, int NPoints);
    /*<  */
    
    inline void drawRect (int X, int Y, int Width, int Height);
    /*<  */
    
    inline void fill (_2D::Polygon_i& Shape);
    /*<  */
    
    inline void draw (_2D::Polygon_i& Shape);
    /*<  */
    
    inline void fill3DRect (int X, int Y, int Width, int Height, bool Raised);
    /*<  */
    
    inline void fillArc (int X, int Y, int Width, int Height, int StartAngle, int ArcAngle);
    /*<  */
    
    inline void fillOval (int X, int Y, int Width, int Height);
    /*<  */
    
    inline void fillPolygon (int& xPoints, int& YPoints, int NPoints);
    /*<  */
    
    inline void fillRectangle (int X, int Y, int Width, int Height);
    /*<  */
    
    inline void fillRound3DRectangle (int X, int Y, int Width, int Height, int ArcWidth, int ArcHeight);
    /*<  */
    
    inline void setClip (int X, int Y, int Width, int Height);
    /*<  */
    
    inline void setXORMode (Color_i Color);
    /*<  */
    
    inline void translate (int X, int Y);
    /*<  */
    
    private:

    int leftEdge,                   //< The left edge of the cell.
        topEdge,                    //< The top edge of the cell.
        width,                      //< The width of the cell.
        height;                     //< The height of the cell.
    
    Color_i backgroundColor,        //< The background color.   
        forgroundColor;             //< The forground color.

    Font& font;                     //< The font.
};
}
