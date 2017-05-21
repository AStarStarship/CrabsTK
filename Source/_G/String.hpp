/** Kabuki Theater
    @file       /.../Source/KabukiTheater-Impl/_G/String.hpp
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

#include <string>

#include "Cell.hpp"
#include "Color.hpp"
#include "Font.hpp"

namespace _G {
    
/** A drawable string. */
class _KabukiTheater_ String : public Cell
{
	public:
	
    const _G::Color DefaultTextColor = _G::Color.black;

    const string DefaultFontName = "Times New Roman";

    enum {
        DefaultFontSize = 12,
        DefalutFontStyle = Font::Plain
    };

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

    String (int leftEdge, int bottomEdge, string initText, _G::Color initTextColor);
    /*<  */

    String (int leftEdge, int bottomEdge, string initText, _G::Color initTextColor, int initFontSize);
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

    void draw (Cell& C);
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
}   //  _G
