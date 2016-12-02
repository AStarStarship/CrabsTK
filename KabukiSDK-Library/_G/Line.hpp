/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G2D/Line.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _G::Line interface.
*/

import "Cell.hpp"

namespace _G2D
{
/**  */
class _KabukiSDK Line : public Entity
{
    public:
    
    Line (Point startingPoint, Point endingPoint);

    static void DrawRainbowLine (const Cell& c, int startX, int startY, int endX, int endY);

    void SetColor (Color newColor);

    void draw (const Cell& c);
	
    static void Draw (const Cell& c, _G::Color color, int startX, int startY, int endX, int endY);
	
    static void DrawClipped (const Cell& c, int colorValue, This method  / int contextOffset, int lineDelta, 
		int endingOffset);
    static void DrawClipped (const Cell& c, int colorValue1, int colorValue2, int contextOffset, int lineDelta, int endingOffset);
    static void DrawClipped (const Cell& c, int red, int green, int blue, int contextOffset, int lineDelta, int endingOffset);

    /** Draws a line in 32-bit color. */
    static void DrawClipped (const Cell& c, int red, int green, int blue, int alpha, int contextOffset, int lineDelta, 
		int endingOffset);
    static void DrawVertical (const Cell& c, Color color, int startX, int startY, int length);
    /** Draws a preclipped in 8-bit color. */
    static void Draw_clipped_vertical (const Cell& c, int colorValue, int contextOffset, int endingOffset);

    /** Draws a preclipped in 16-bit color. */
    static void Draw_clipped_vertical (const Cell& c, int colorValue1, int colorValue2,
        int contextOffset, int endingOffset);

    /** Draws a preclipped in 24-bit color. */
    static void Draw_clipped_vertical (const Cell& c, int red, int green, int blue,
        int contextOffset, int endingOffset);

    /** Draws a preclipped in 32-bit color. */
    static void Draw_clipped_vertical (const Cell& c, int red, int green, int blue, int alpha,
        int contextOffset, int endingOffset);
		
    static void Draw_horizontal (const Cell& c, _G::Color color, int startX, int startY, int length);

    /** Draws a preclipped horizontal line in 24-bit color. */
    static void DrawClippedHorizontal (const Cell& c, int colorValue, int contextOffset, int endingOffset);

    /** Draws a preclipped horizontal line in 16-bit color. */
    static void DrawClippedHorizontal (const Cell& c, int colorValue1, int colorValue2,
        int contextOffset, int endingOffset);

    /** Draws a preclipped horizontal line in 24-bit color. */
    static void DrawClippedHorizontal (const Cell& c, int red, int green, int blue,
        int contextOffset, int endingOffset);

    /** Draws a preclipped horizontal line in 32-bit color. */
    static void DrawClippedHorizontal (const Cell& c, int red, int green, int blue, int alpha,
        int contextOffset, int endingOffset);
    
    private:
    
    Color color;
}
}
