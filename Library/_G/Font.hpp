/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Font.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#pragma once 

#include <KabukiSDK-Config.h>

#include "Cell.hpp"

namespace _G {

class _KabukiSDK Font
/** A text font. 
    @todo
*/
{
	public:

	typedef enum {   
        Plain       = (1 << 0),
		Bold        = (1 << 1),
		Italisized  = (1 << 2),
		Underlined  = (1 << 3)
    } Attributes;

    enum {
        DefaultAttributes = 0
    };
    
	static const float DefaultSize = 10.0f;
    static string DefaultFont = "Times New Roman";

	Font ();

	Font (string Name, float Size, int Attributes);

	string getName ();
	
	void setName (string S);

	int getAttributes ();

	void setAttributes (int A);

	int getSize ();

	void setSize (int A);
    
    int load (string Filename, uint8_t RelativePath, float FontSize, uint32_t TextureWidth, 
        uint32_t TextureHeight, int FirstCharacter, int CountCharacter);
    
    void draw (Cell& c, float X, float Y, const string& Text, Color color);
    
    float getLength (char *text);

	void print (Terminal& slot);

	private:
	
	string name;

	float size;
	
	int textureWidth, 
        textureHeight,
        firstCharacter,
        attributes; 
    
	string characterData;
	
	int countCharacter;
	
	uint32_t tid;
};

}   //< _G
