/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_G/String.h
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

using namespace _G;

String::String ()
{
    fontSize = DefaultFontSize;
    fontStyle = DefalutFontStyle;
    fontWidth = 6;
    fontHeight = 15;
    numCharictors = 0;

    fontName = DefaultFontName;
    colour = DefaultTextColor;
    font = DefaultFont;
}

String::String (int leftEdge, int bottomEdge):
    Cell (leftEdge, bottomEdge, 0, 0)
{
}

/**  */
String::String (int leftEdge, int bottomEdge, string initText):
    Cell (leftEdge, bottomEdge, 0, 0)
{
    text = initText;
    numCharictors = text.numChars ();
}

/**  */
String::String (int leftEdge, int bottomEdge, string initText, int initFontSize):
    Cell (leftEdge, bottomEdge, 0, 0)
{
    fontSize = initFontSize;
    text = initText;
    numCharictors = text.numChars ();
}

/**  */
String::String (int leftEdge, int bottomEdge, string initText, _G.Color initTextColor):
    Cell (leftEdge, bottomEdge, 0, 0)
{
    text = initText;
    colour = initTextColor;
    numCharictors = text.numChars ();
}

/**  */
String::String (int leftEdge, int bottomEdge, string initText, _G.Color initTextColor, int initFontSize):
    Cell (leftEdge, bottomEdge, 0, 0)
{
    text = initText;
    numCharictors = text.numChars ();
    colour = initTextColor;
    fontSize = initFontSize;
}

/**  */
void String::AddText (string newString)
{
    text.concat (newString);
    numCharictors = text.numChars ();
    SetWidth (text.numChars ()  fontWidth);
}

/**  */
string String::AddTextOverflow (const string& newString)
{
    int newTextWidth = newString.numChars ()  fontWidth;
    if (newTextWidth + Width > MaxWidth ())
    {
        int difference = newTextWidth+Width -MaxWidth (),
        overflowIndex = (newTextWidth-Width)/fontWidth;
        addText (text.SubText (0,overflowIndex));
        return newString.SubText (overflowIndex,newString.numChars ());
    }
    AddText (newString);
    numCharictors = text.numChars ();
    return new string ();
}

/**  */
bool String::InsertText (const string& newString, int here)
{
    if (newString.numChars ()fontWidth > MaxWidth ())
        return false;
    text = text.SubText (0, here).concat (text.SubText (here+1, text.numChars ()));
    numCharictors = text.numChars ();
    return true;
}    

/**  */
void String::RemoveCharAtIndex (int index1)
{
    text = text.SubText (0,index1-1).concat (text.SubText (index1,text.numChars ()));
    numCharictors = text.numChars ();
}

/**  */
void String::RemoveTextAtIndex (int index1, int index2)
{
    text = text.SubText (0,index1).concat (text.SubText (index2,text.numChars ()));
    numCharictors = text.numChars ();
}  

/**  */
void String::setFont (const string& newFontName)
{
    fontName = newFontName;
    font = new Font (fontName, fontStyle, fontSize);
}  

/**  */
void String::setFont (Font& newFont)
{
    if (newFont==null)
    {
        fontName = newFont.Name;
        fontSize = newFont.Size ();
    }
}  

/**  */
void String::setFontSyle (int newStyle)
{
    fontStyle = newStyle;
    font = new Font (fontName, fontStyle, fontSize);
}  

/**  */
float String::getFontSize ()
{
    return fontSize;
}

void String::setFontSize (float newSize)
{
    fontSize = newSize;
    font = new Font (fontName, fontStyle, fontSize);
}

/**  */
Color String::getDefaultColor ()
{
    return DefaultTextColor;
}

/**  */
void String::setFontColor (Color newColor)
{
    colour = newColor;
}  

string String::getString () { return string; }

int String::setString (const string& S) { string = S }

void String::Update ()
{  
}

void String::UpdateHeight ()
{
    Char currentChar;
    Iterator iteration = charictors.iterator ();
    while (iteration.IsNotDone ())
    {
        currentChar = (Char) iteration.nextObject ();
        int newHeight = currentChar.fontSizefontHeight;
        if (newHeight > Height)
            SetHeight (newHeight);
    }  
}

int String::getLength ()
{
    return numCharictors;
}

void String::draw (ScreenBuffer screenBuffer)
{
    if (text == null)
        return;
    screenBuffer.setPixel (0,0, colour);
}

}   //< _G
