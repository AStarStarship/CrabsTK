/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Cell.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the Cell class _KabukiSDK.
*/

#pragma once

#include "Color.hpp"
#include "Font.hpp"
#include "Rect.hpp"

#include <string>

namespace _G {
    
class _KabukiSDK Cell_f
/** A graphics cell to draw on. 

*/
{
    public:
    
    Cell_f (int LeftEdge, int TopEdge, int Width, int Height);
    /*<  */

    void drawText (string S, int X, int Y);
    /*<  */

    void drawText (string S, int X, int Y, float Angle);
    /*<  */

    inline void setColor (Color Color);
    /*< Sets the Color_i_i. */
	
    inline void drawLine (Color color, int startX, int startY, int endX, int endY);

    inline void drawClippedLine (int colorValue, int contextOffset, int lineDelta, 
        int endingOffset);

    inline void drawClippedLine (int colorValue1, int colorValue2, int contextOffset, int lineDelta, int endingOffset);

    inline void drawClippedLine (int red, int green, int blue, int contextOffset, int lineDelta, int endingOffset);

    inline void drawClippedLine (int red, int green, int blue, int alpha, int contextOffset, int lineDelta, 
        int endingOffset);
    /*< Draws a line in 32-bit color. */

    inline void drawVerticalLine (Color color, int startX, int startY, int length);

    inline void drawClippedVerticalLine (int colorValue, int contextOffset, int endingOffset);
    /*< Draws a preclipped in 8-bit color. */

    inline void drawClippedVerticalLine (int colorValue1, int colorValue2,
        int contextOffset, int endingOffset);
    /*< Draws a preclipped in 16-bit color. */

    inline void drawClippedVerticalLine (int red, int green, int blue,
        int contextOffset, int endingOffset);
    /*< Draws a preclipped in 24-bit color. */

    inline void drawClippedVerticalLine (int red, int green, int blue, int alpha,
        int contextOffset, int endingOffset);
    /*< Draws a preclipped in 32-bit color. */

    inline void drawHorizontalLine (Color color, int startX, int startY, int length);

    inline void drawClippedHorizontalLine (int colorValue, int contextOffset, int endingOffset);
    /*< Draws a preclipped horizontal line in 24-bit color. */

    inline void drawClippedHorizontalLine (int colorValue1, int colorValue2,
        int contextOffset, int endingOffset);
    /*< Draws a preclipped horizontal line in 16-bit color. */

    inline void drawClippedHorizontalLine (int red, int green, int blue,
        int contextOffset, int endingOffset);
    /*< Draws a preclipped horizontal line in 24-bit color. */

    inline void drawClippedHorizontalLine (int red, int green, int blue, int alpha,
        int contextOffset, int endingOffset);
    /*< Draws a preclipped horizontal line in 32-bit color. */
    
    inline void drawImage (Image& I, int LeftEdge, int TopEdge);
    /*<  */
    
    inline void drawImage (Image& I, int LeftEdge, int TopEdge, int newWidth, int newHeight);
    /*<  */

    inline void drawTriangle (Color color, int x1, int y1, int x2, int y2, int x3, int y3);
    /*< draws a triangle on the screen */

    inline void fillTriangle (Color color, int x1, int y1, int x2, int y2, int x3, int y3);
    /*< Fills the triangle with the given color. */

    inline void drawBottomTriangle (Color color, int x1, int y1, int x2, int y2, int x3, int y3);
    /*< Fills the bottom of the given triangle with the given color. */
    
    inline void drawLine (float StartX, float StartY, float EndX, float EndY);
    /*<  */
    
    inline void drawLine (Color Color, int StartX, int StartY, int EndX, int EndY);

    /*<  */
    
    inline void drawRectangle (Color Color, int LeftEdge, int TopEdge, int Width, int Height);
    /*<  */
    
    inline void drawRectangle (Color Color, int LeftEdge, int TopEdge, int Width, int Height, int thickness);
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
    
    inline void fill (_2D::Polygon_f& Shape);
    /*<  */
    
    inline void draw (_2D::Polygon_f& Shape);
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
    
    inline void setXORMode (Color Color);
    /*<  */
    
    inline void translate (int X, int Y);
    /*<  */
    
    private:

    float leftEdge,                 //< The left edge of the cell.
        topEdge,                    //< The top edge of the cell.
        width,                      //< The width of the cell.
        height;                     //< The height of the cell.
    
    Color backgroundColor,        //< The background color.   
        forgroundColor;             //< The forground color.

    Font& font;                     //< The font.
};

}   //< _G
