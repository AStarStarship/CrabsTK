/** Kabuki SDK
    @file       /.../KabukiSDK-Library/_G/Texture/Texture.hpp
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D.Vector_f interface.
*/

#

using namespace _G {
/** Bitmap file header
	Provides general information about the file. 
*/
struct BitmapTextureFileHeader
{
	short   bitmapIsValid;
	long    sizeInBytes;
	short   bfReserved1;
	short   bfReserved2;
	long    colourDataStartBit;
}

/**  */
struct BitmapcolourDataHeader
{
	long    sizeInBytes;
	long    width;
	long    height;
	short   numPlanes;
	short   bitsPerPixel;
	long    compressionType;
	long    imageSizeInBytes;
	long    pixelsPerMeterX;
	long    pixelsPerMeterY;
	long    numColorsUsed;
	long    numImportantColorIndexs;
}


class Texture
/*< An OpenGL-compatible texture.  */
{
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
    
	bool load (std::string fileName);
    /*<  */
    
	bool loadBitmap (const string* fileName);
    /*<  */
    
	void setPixel (int X, int Y, Color pixelColor);
    /*<  */
    
	void setPixel (int X, int Y, color_t Color);

	/**  */
	void setPixel (int X, int Y, int ColorA, int ColorB);
    /*<  */
	void setPixel (int X, int Y, int Red, int Green, int Blue);

	/**  */
	void setPixel (int X, int Y, int Red, int Green, int Blue, int Alpha);
    /*<  */
    
	void setPixel (int pixel, int imageDataOffset, Color_i color);
    /*<  */
    
	void setPixel (int pixel, int imageDataOffset, int colourValue);
    /*<  */
    
	void setPixel (int pixel, int imageDataOffset, int colourValue1, int colourValue2);
    /*<  */
    
	void setPixel (int pixel, int imageDataOffset, int Red, int Green, int Blue);
	/*<  */
    
	void setPixel (int pixel, int imageDataOffset, int Red, int Green, int Blue, int Alpha);

	void draw (const Cell& C, Texture* Source);
	/*< Draws the source to the canvas on the NW corner. */
	
	void draw (const Cell& C, const Texture& Source, int LeftEdge, int TopEdge);
	/*< This method draws the source to the canvas. */

	void drawClippedSection (const Cell& C, const Texture& Source, int CanvasOffset, int CanvasLineEndOffset, 
		int CanvasLastLineOffset);
    /*< Draws a clipped section of this image. */

	int getSourceOffset ();

	void drawClippedHorizontalLine (int CanvasColorData, int SourceColorData, int CanvasOffset, 
		int LineEndOffset, int SourceOffset);

    private:
	
	Color bgColor;
	int colourData;
	int imageRowWidthInBytes;

	bool isLoaded;
	int bpp;
};
}
