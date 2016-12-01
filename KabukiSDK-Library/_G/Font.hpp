/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Font.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#pragma once 

#include <stdint.h>
#include <string>
using namespace std;

#include "_Math/vect4.h"
using namespace _Math;

#include <freetype/config/ftheader.h>

namespace _G {

class _G_API Font
/** A text font. */
{
	    public:

	enum {   
        Plain = 0,
		Bold,
		Italisized,
		Underlined
    };
		
    static const int DefaultAttributes = 0;
	static const float DefaultSize = 10.0f;
    static const char* DefaultFont = "Times New Roman";

	Font ();

	Font (const char* Name, float Size, int Attributes);

	const char* getName ();
	
	void setName (const char* S);

	int getAttributes ();
	void setAttributes (int A);

	int getSize ();
	void setSize (int A);
    
    int load (const char* Filename, uint8_t RelativePath, float FontSize, uint32_t TextureWidth, 
        uint32_t TextureHeight, int FirstCharacter, int CountCharacter);
    
    void draw (const Cell& c, float X, float Y, const string* Text, vec4* color);
    
    float GetLength (char *text);

	void print (I2P::Terminal& slot);

	    private::
	
	const char* name;

	float size;
	
	int textureWidth, 
        textureHeight,
        firstCharacter,
        attributes; 
    
	
	stbtt_bakedchar* character_data;
	
	
	int countCharacter;
	
	uint32_t tid;
};
}

