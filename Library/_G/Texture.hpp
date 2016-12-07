/** Kabuki Software Development Kit
    @file       /.../KabukiSDK-Library/_G/Texture/Texture.hpp
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D::Vector_f interface.
*/

#pragma once

#include "Color.hpp"

namespace _G {


class Texture
/*< An OpenGL-compatible texture.  */
{
    public:
    
	Texture ();
    /*<  */
    
	Texture (int initWidth, int initHeight, int bitsPerPixel, Color backgroundColor);
    /*< 
        @param BPP Bits per pixel.    
    */
    
	void markLoaded ();
    /*<  */
    
	int getBPP ();
    /*<  */
    
	void setBPP (int BPP);
    /*<  */
    
	int getWidth ();
    /*<  */
    
	Color getPixel (int X, int Y);
    /*<  */
    
	bool load (char fileName);
    /*<  */
    
	bool load (string fileName);
    /*<  */
    
	bool loadBitmap (const string& fileName);
    /*<  */
    
	void setPixel (int X, int Y, Color color);
    /*<  */
    
	void setPixel (int X, int Y, color_t color);

	/**  */
	void setPixel (int X, int Y, int colorA, int colorB);
    /*<  */
	void setPixel (int X, int Y, int red, int green, int blue);

	/**  */
	void setPixel (int X, int Y, int red, int green, int blue, int alpha);
    /*<  */
    
	void setPixel (int pixel, int imageDataOffset, Color color);
    /*<  */
    
	void setPixel (int pixel, int imageDataOffset, int colourValue);
    /*<  */
    
	void setPixel (int pixel, int imageDataOffset, int colourValue1, int colourValue2);
    /*<  */
    
	void setPixel (int pixel, int imageDataOffset, int red, int green, int blue);
	/*<  */
    
	void setPixel (int pixel, int imageDataOffset, int red, int green, int blue, int alpha);

	void draw (Cell& C, Texture* texture);
	/*< Draws a texture to the canvas on the NW corner. */
	
	void draw (Cell& C, const Texture& texture, int dx, int dy);
	/*< Draws a textureon the cell with the given offsets delta x and delta y. */
	
	void draw (Cell& C, const Texture& texture, float dx, float dy);
	/*< Draws a textureon the cell with the given offsets delta x and delta y. */

	void drawClippedSection (Cell& c, const Texture& source, int canvasOffset, int canvasLineEndOffset, 
		int CanvasLastLineOffset);
    /*< Draws a clipped section of this image. */

	int getSourceOffset ();

	void drawClippedHorizontalLine (int canvasColorData, int sourceColorData, int canvasOffset, 
		int lineEndOffset, int sourceOffset);

    private:
	
	Color bgColor;              //< The background color.
	int colorData,              //< The color data.
	    imageRowWidthInBytes,   //< The width of a row in bytes.
        bpp;                    //< Bits per pixel
	bool isLoaded;              //< Flag for if text has been loaded or not.
};

}   //< _G
