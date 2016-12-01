/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Font.cpp
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#include <string>
#include "_G/Font.h"

namespace _G {

Font::Font ()
{
    name = DefaultFont;
    size = DefaultSize;
    attributes = Plain;
}

Font::Font (std::string Name, int Size, int Attributes)
{
    /// Initialize as the default, then modify.
    name = DefaultFont;
    size = DefaultSize;
    attributes = Plain;

    SetName (Name);
    SetSize (Size);
    SetAttributes (Attributes);
}

std::string Font::GetName () { return name; }

bool Font::SetName (const std::string& S)
{
    name = S;
}

int Font::GetAtributes () { return attributes; }

int Font::SetFont (cont Font& F) { attributes = F; }

float Font::GetSize () { return size; }

int Font::SetSize (float Value) { size = Value; }

std::string Font::ToString ()
{
	std::string s = "Font: \"";
	
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
}   //< namespace _G
