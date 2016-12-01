/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Cell.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _G::Cell class _G_API.
*/

#pragma once

#include "Color.h"
#include "Font.h"
#include "Rect.h"

#include <string>

namespace _G {
    
class _G_API Cell
/** A graphics cell to draw on. */
{
    public:
    
    Cell (int LeftEdge, int TopEdge, int Width, int Height);
    /*<  */

    void Draw (const char* S, int X, int Y);
    /*<  */

    void Draw (const char* S, int X, int Y, float Angle);
    /*<  */

    static inline void SetColor (Color_i Color);
    /*< Sets the _G::Color_i_i. */
    
    static inline void DrawImage (Image& I, int LeftEdge, int TopEdge
    /*<  */
    
    static inline void DrawImage (Image& I, int LeftEdge, int TopEdge, int newWidth, int newHeight);
    /*<  */

    static inline void DrawTriangle (const Cell& C, _G::Color color, int x1, int y1, int x2, int y2, int x3, int y3);
    /*< Draws a triangle on the screen */

    static inline void FillTriangle (const Cell& C, _G::Color color, int x1, int y1, int x2, int y2, int x3, int y3);
    /*< Fills the triangle with the given color. */

    static inline void DrawBottomTriangle (const Cell& C, _G::Color color, int x1, int y1, int x2, int y2, int x3, int y3);
    /*< Fills the bottom of the given triangle with the given color. */
    
    static inline void DrawLine (const Cell& C, float StartX, float StartY, float EndX, float EndY);
    /*<  */
    
    static inline void DrawLine (const Cell& C, Color_i Color, int StartX, int StartY, int EndX, int EndY);

    /*<  */
    
    static inline void DrawRectangle (const Cell& C, Color_i Color, int LeftEdge, int TopEdge, int Width, int Height);
    /*<  */
    
    static inline void DrawRectangle (const Cell& C, Color_i Color, int LeftEdge, int TopEdge, int Width, int Height, int thickness);
    /*<  */
    
    static inline void DrawOval (int X, int Y, int Width, int Height);
    /*<  */
    
    static inline void DrawRoundedRectangle (int X, int Y, int Width, int Height, int ArcWidth, int ArcHeight);
    /*<  */
    
    static inline void ClearArea (int X, int Y, int Width, int Height);
    /*<  */
    
    static inline void ClipArea (int X, int Y, int Width, int Height
    /*<  */
    
    static inline void CopyArea (int X, int Y, int Width, int Height, int DX, int DY);

    /**  */
    static inline void DrawArc (int X, int Y, int Width, int Height, int StartAngle, int ArcAngle);
    /*<  */
    
    static inline void DrawText (const char* thisText, int offset, int length, int X, int Y);
    /*<  */
    
    static inline void DrawPolygon (int* XPoints, int* YPoints, int NPoints);
    /*<  */
    
    static inline void DrawPolYline (int* XPoints, int* YPoints, int NPoints);
    /*<  */
    
    static inline void DrawRect (int X, int Y, int Width, int Height);
    /*<  */
    
    static inline void Fill (_2D::Polygon_i& Shape);
    /*<  */
    
    static inline void Draw (_2D::Polygon_i& Shape);
    /*<  */
    
    static inline void Fill3DRect (int X, int Y, int Width, int Height, bool Raised);
    /*<  */
    
    static inline void FillArc (int X, int Y, int Width, int Height, int StartAngle, int ArcAngle);
    /*<  */
    
    static inline void FillOval (int X, int Y, int Width, int Height);
    /*<  */
    
    static inline void FillPolygon (int& xPoints, int& YPoints, int NPoints);
    /*<  */
    
    static inline void FillRectangle (int X, int Y, int Width, int Height);
    /*<  */
    
    static inline void FillRound3DRectangle (int X, int Y, int Width, int Height, int ArcWidth, int ArcHeight
    /*<  */
    
    static inline void SetClip (int X, int Y, int Width, int Height);
    /*<  */
    
    static inline void SetXORMode (Color_i Color);
    /*<  */
    
    static inline void Translate (int X, int Y);
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
