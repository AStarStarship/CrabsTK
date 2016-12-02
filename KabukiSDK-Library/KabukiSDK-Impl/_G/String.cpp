/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/String.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D::Vector_f interface.
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

void String::Draw (ScreenBuffer screenBuffer)
{
    if (text == null)
        return;
    screenBuffer.setPixel (0,0, colour);
}