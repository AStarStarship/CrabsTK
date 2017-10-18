/** Kabuki Starship
    @file    /.../Source/KabukiTheater-Impl/_G/Canvas.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once

#include "Color.h"
#include "Font.h"
#include "Rect2D.h"
#include "Image.h"

namespace _G {


class _KabukiTheater_ Canvas
/** A graphics cell to draw on. 

*/
{
    public:
    
    Canvas (int leftEdge, int topEdge, int width, int height);
    /*<  */

    void drawText (std::string S, int x, int y);
    /*<  */

    void drawText (std::string S, int x, int y, float angle);
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
    
    inline void drawImage (Image& I, int leftEdge, int topEdge);
    /*<  */
    
    inline void drawImage (Image& I, int leftEdge, int topEdge, int newWidth, int newHeight);
    /*<  */

    inline void drawTriangle (Color color, int x1, int y1, int x2, int y2, int x3, int y3);
    /*< draws a triangle on the screen */

    inline void fillTriangle (Color color, int x1, int y1, int x2, int y2, int x3, int y3);
    /*< Fills the triangle with the given color. */

    inline void drawBottomTriangle (Color color, int x1, int y1, int x2, int y2, int x3, int y3);
    /*< Fills the bottom of the given triangle with the given color. */
    
    inline void drawLine (float startX, float startY, float endX, float endY);
    /*<  */
    
    inline void drawLine (Color color, int startX, int startY, int endX, int endY);

    /*<  */
    
    inline void drawRectangle (Color color, int leftEdge, int topEdge, int width, int height);
    /*<  */
    
    inline void drawRectangle (Color color, int leftEdge, int topEdge, int width, int height, int thickness);
    /*<  */
    
    inline void drawOval (int x, int y, int width, int height);
    /*<  */
    
    inline void drawRoundedRectangle (int x, int y, int width, int height, int ArcWidth, int ArcHeight);
    /*<  */
    
    inline void clearArea (int x, int y, int width, int height);
    /*<  */
    
    inline void clipArea (int x, int y, int width, int height);
    /*<  */
    
    inline void copyArea (int x, int y, int width, int height, int DX, int DY);
    /*<  */

    inline void drawArc (int x, int y, int width, int height, int StartAngle, int ArcAngle);
    /*<  */
    
    inline void drawText (std::string s, int offset, int length, int x, int y);
    /*<  */
    
    inline void drawPolygon (int* XPoints, int* YPoints, int NPoints);
    /*<  */
    
    inline void drawPolYline (int* XPoints, int* YPoints, int NPoints);
    /*<  */
    
    inline void drawRect (int x, int y, int width, int height);
    /*<  */
    
    inline void fill (_2D::Polygon& p);
    /*<  */
    
    inline void draw (_2D::Polygon& p);
    /*<  */
    
    inline void fill3DRect (int x, int y, int width, int height, bool raised);
    /*<  */
    
    inline void fillArc (int x, int y, int width, int height, int StartAngle, int ArcAngle);
    /*<  */
    
    inline void fillOval (int x, int y, int width, int height);
    /*<  */
    
    inline void fillPolygon (int& xPoints, int& YPoints, int NPoints);
    /*<  */
    
    inline void fillRectangle (int x, int y, int width, int height);
    /*<  */
    
    inline void fillRound3DRectangle (int x, int y, int width, int height, int ArcWidth, int ArcHeight);
    /*<  */
    
    inline void setClip (int x, int y, int width, int height);
    /*<  */
    
    inline void setXORMode (Color color);
    /*<  */
    
    inline void translate (int x, int y);
    /*<  */
    
    private:

    float leftEdge,                 //< The left edge of the cell.
        topEdge,                    //< The top edge of the cell.
        width,                      //< The width of the cell.
        height;                     //< The height of the cell.
    
    Color backgroundColor,          //< The background color.   
        forgroundColor;             //< The forground color.

    Font& font;                     //< The font.
};

}   //< _G
