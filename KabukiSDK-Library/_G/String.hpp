/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/String.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#include <string>

#include "Cell.hpp"
#include "Color.hpp"
#include "Font.hpp"

namespace _G {
    
/** A drawable string. */
class _KabukiSDK String : public Cell
{
	public:
	
    const _G::Color DefaultTextColor = _G::Color.black;

    const string DefaultFontName = "Times New Roman";

    const int DefaultFontSize = 12,
        DefalutFontStyle = Font.Plain;

    const Font DefaultFont = new Font (DefaultFontName, DefalutFontStyle, DefaultFontSize);
    /*<  */

    String ();
    /*<  */

    String (int leftEdge, int bottomEdge);
    /*<  */

    String (int leftEdge, int bottomEdge, string initText);
    /*<  */

    String (int leftEdge, int bottomEdge, string initText, int initFontSize);
    /*<  */

    String (int leftEdge, int bottomEdge, string initText, _G::Color_i initTextColor);
    /*<  */

    String (int leftEdge, int bottomEdge, string initText, _G::Color_i initTextColor, int initFontSize);
    /*<  */

    void addText (string newString);
    /*<  */

    string addTextOverflow (string newString);
    /*<  */

    bool insertText (string newString, int here);
    /*<  */

    void removeCharAtIndex (int index1);
    /*<  */

    void removeTextAtIndex (int index1, int index2);
    /*<  */

    void setFont (string newFontName);
    /*<  */

    void setFont (Font& newFont);
    /*<  */

    void getFontSyle (int newStyle);
    /*<  */

    int getFontSize ();
    /*<  */

    void setFontSize (int newSize);
    /*<  */

    Color getDefaultColor ();
    /*<  */

    void setFontColor (Color newColor);

    /**  */
    string getString ();
    /*<  */

    void setString (const string s);
    /*<  */

	void update ();
    /*<  */

    void updateHeight ();
    /*<  */

    int getLength ();
    /*<  */

    void draw (const Cell& C);
    /*<  */

	private:
	
    int fontSize,
        fontStyle,
        fontWidth,
        fontHeight,
        numCharictor;

    string fontName;

    Color color;
    Font font;
};
}   //< namespace _G
