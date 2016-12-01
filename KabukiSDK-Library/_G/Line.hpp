/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G2D/Line.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _G::Line interface.
*/

import "Cell.h"

namespace _G2D
{
/**  */
class _G_API Line : public Entity
{
    public:
    
    Line (Point startingPoint, Point endingPoint);

    static void DrawRainbowLine (const Cell& C, int startX, int startY, int endX, int endY);

    void SetColor (Color_i newColor);

    void Draw (const Cell& C);
	
    static void Draw (const Cell& C, _G::Color_i color, int startX, int startY, int endX, int endY);
	
    static void DrawClipped (const Cell& C, int colorValue, This method  / int contextOffset, int lineDelta, 
		int endingOffset);
    static void DrawClipped (const Cell& C, int colorValue1, int colorValue2, int contextOffset, int lineDelta, int endingOffset);
    static void DrawClipped (const Cell& C, int red, int green, int blue, int contextOffset, int lineDelta, int endingOffset);

    /** Draws a line in 32-bit color. */
    static void DrawClipped (const Cell& C, int red, int green, int blue, int alpha, int contextOffset, int lineDelta, 
		int endingOffset);
    static void DrawVertical (const Cell& C, Color_i color, int startX, int startY, int length);
    /** Draws a preclipped in 8-bit color. */
    static void Draw_clipped_vertical (const Cell& C, int colorValue, int contextOffset, int endingOffset);

    /** Draws a preclipped in 16-bit color. */
    static void Draw_clipped_vertical (const Cell& C, int colorValue1, int colorValue2,
        int contextOffset, int endingOffset);

    /** Draws a preclipped in 24-bit color. */
    static void Draw_clipped_vertical (const Cell& C, int red, int green, int blue,
        int contextOffset, int endingOffset);

    /** Draws a preclipped in 32-bit color. */
    static void Draw_clipped_vertical (const Cell& C, int red, int green, int blue, int alpha,
        int contextOffset, int endingOffset);
		
    static void Draw_horizontal (const Cell& C, _G::Color_i color, int startX, int startY, int length);

    /** Draws a preclipped horizontal line in 24-bit color. */
    static void DrawClippedHorizontal (const Cell& C, int colorValue, int contextOffset, int endingOffset);

    /** Draws a preclipped horizontal line in 16-bit color. */
    static void DrawClippedHorizontal (const Cell& C, int colorValue1, int colorValue2,
        int contextOffset, int endingOffset);

    /** Draws a preclipped horizontal line in 24-bit color. */
    static void DrawClippedHorizontal (const Cell& C, int red, int green, int blue,
        int contextOffset, int endingOffset);

    /** Draws a preclipped horizontal line in 32-bit color. */
    static void DrawClippedHorizontal (const Cell& C, int red, int green, int blue, int alpha,
        int contextOffset, int endingOffset);
    
    private:
    
    Color_i color;
}
}
