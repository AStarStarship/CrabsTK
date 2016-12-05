/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Font.cpp
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#include <string>
#include "_G/Font.hpp"

namespace _G {

Font::Font ()
{
    name = DefaultFont;
    size = DefaultSize;
    attributes = Plain;
}

Font::Font (string Name, int Size, int Attributes)
{
    /// Initialize as the default, then modify.
    name = DefaultFont;
    size = DefaultSize;
    attributes = Plain;

    setName (Name);
    SetSize (Size);
    SetAttributes (Attributes);
}

string Font::getName () { return name; }

bool Font::setName (const string& S)
{
    name = S;
}

int Font::getAtributes () { return attributes; }

int Font::setFont (cont Font& F) { attributes = F; }

float Font::getSize () { return size; }

int Font::setSize (float Value) { size = Value; }

string Font::toString ()
{
	string s = "Font: \"";
	
	switch (attributes)
	{
		case 0:
			return s + name + "\" Size: " + size + " Attributes: None";
		case 1:
			return s + name + "\" Size: " + size + " Attributes: Bold";
		case 2:
			return s + name + "\" Size: " + size + " Attributes: Italisized";
		case 3:
			return s + name + "\" Size: " + size + " Attributes: Bold - Italisized";
		case 4:
			return s + name + "\" Size: " + size + " Attributes: Underlined";
		case 5:
			return s + name + "\" Size: " + size + " Attributes: Bold - Underlined";
		case 6:
			return s + name + "\" Size: " + size + " Attributes: Italisized - Underlined";
		case 7:
			return s + name + "\" Size: " + size + " Attributes: Bold - Italisized - Underlined";
		default:
			return s + name + "\" Size: " + size + " Attributes: None";
	}
}
}   //  _G
