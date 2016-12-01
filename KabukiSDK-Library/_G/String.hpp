/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/String.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#include <string>

#include "Cell.h"
#include "Color.h"
#include "Font.h"

namespace _G {
    
/** A drawable string. */
class _G_API String : public Cell
{
	public:
	
    const _G::Color DefaultTextColor = _G::Color.black;

    const const char* DefaultFontName = "Times New Roman";

    const int DefaultFontSize = 12,
        DefalutFontStyle = Font.Plain;

    const Font DefaultFont = new Font (DefaultFontName, DefalutFontStyle, DefaultFontSize);

    /**  */
    String ();

    String (int leftEdge, int bottomEdge);

    /**  */
    String (int leftEdge, int bottomEdge, const char* initText);

    /**  */
    String (int leftEdge, int bottomEdge, const char* initText, int initFontSize);

    /**  */
    String (int leftEdge, int bottomEdge, const char* initText, _G::Color_i initTextColor);

    /**  */
    String (int leftEdge, int bottomEdge, const char* initText, _G::Color_i initTextColor, int initFontSize);

    /**  */
    void AddText (const char* newString);

    /**  */
    const char* AddTextOverflow (const char* newString);

    /**  */
    bool InsertText (const char* newString, int here);

    /**  */
    void RemoveCharAtIndex (int index1);

    /**  */
    void RemoveTextAtIndex (int index1, int index2);

    /**  */
    void SetFont (const char* newFontName);

    /**  */
    void SetFont (Font& newFont);

    /**  */
    void SetFontSyle (int newStyle);

    /**  */
    int GetFontSize ();

    void SetFontSize (int newSize);

    /**  */
    Color GetDefaultColor ();

    /**  */
    void SetFontColor (Color newColor);

    /**  */
    const char* GetString ();
	
    void SetString (const const char* s);
    

    /**  */
	void Update ();

    /**  */
    void UpdateHeight ();
    int Length ();
    void Draw (const Cell& C);

	    private:
	
    int fontSize,
        fontStyle,
        fontWidth,
        fontHeight,
        numCharictor;

    const char* fontName;

    Color color;
    Font font;
};
}   //< namespace _G
