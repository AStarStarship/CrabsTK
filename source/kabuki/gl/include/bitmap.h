/** Kabuki Starship
    @file       /.../Source/_G/Texture/Texture.h
    @author     Cale McCollough <cale.mccollough@gmail.com>
    @license    Copyright (C) 2017 Cale McCollough <calemccollough.github.io>

                        All rights reserved (R).

        This program is free software: you can redistribute it and/or modify it 
        under the terms of the GNU General Public License as published by the 
        Free Software Foundation, either version 3 ofthe License, or (at your 
        option) any later version.

        This program is distributed in the hope that it will be useful, but 
        WITHOUT ANY WARRANTY; without even the implied warranty of 
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
        General Public License for more details.

        You should have received a copy of the GNU General Public License 
        along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "Color.h"

namespace _G {

struct BitmapTextureFileHeader
/** Bitmap file header
	Provides general information about the file. 
*/
{
	short   bitmapIsValid;
	long    sizeInBytes;
	short   bfReserved1;
	short   bfReserved2;
	long    colourDataStartBit;
};

struct BitmapcolourDataHeader
/**  */
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
};

bool Bitmap::loadBitmap (const string& filename)
{
    // Load the file into memory
    FILE inFile;
    inFile = fopen (filename, "rb");

    //Create the bitmap file
    BitmapBitmapFileHeader bitmapFile;
    fread (&bitmapFile, sizeof (BitmapBitmapFileHeader), 1, inFile);

    //Create the BitmapColorDataHeader
    BitmapColorDataHeader thisBitmap;
    fread (&thisBitmap, sizeof (BitmapColorDataHeader), 1, inFile);

    //load the palette for 8 bits per pixel
    int numColors = thisBitmap.bitsPerPixel;
    if (numColors == 8)
    {  ColorPallet = new Color_RGBA[numColors];
        fread (ColorPallet, sizeof (Color_RGBA), thisBitmap.bitsPerPixel, inFile);
    }

    long ColorDataSizeInBytes;
    ColorDataSizeInBytes = (long) bitmapFile.sizeInBytes 
                                         - bitmapFile.ColorDataStartBit;

    uint32_t tempColorData;
    tempColorData = new uint32_t[ColorDataSizeInBytes];
    if (sizeof (tempColorData) == 0)
    {  fclose (inFile);
        return false;
    }
    fread (tempColorData, sizeof (uint32_t), ColorDataSizeInBytes, inFile);

    //  To determine the pixelRowWidth_In32-BitColor you must find
    //  out how many bits are in each row of the image data table.

    long ColorDataArrayWidthInBtyes;
    ColorDataArrayWidthInBtyes = (long) (((float)thisBitmap.Width) * ((float) bpp / 8.0));
    long newColorDataArrayWidth;

    newColorDataArrayWidth = ColorDataArrayWidthInBtyes;

    //This loop adds a little bit of white space in the image data array because
    //some programs wont acknowledge the image data unless its in 32-bit format

    while (newColorDataArrayWidth % 4 != 0)
        newColorDataArrayWidth++;

    //Set newColorDataSizeInBytes to the actual files image data size
    long newColorDataSizeInBytes;
    newColorDataSizeInBytes = thisBitmap.Height  ColorDataArrayWidthInBtyes;

    //Allocate memory for the image data
    ColorData = new uint32_t[newColorDataSizeInBytes];
    if (ColorData == null)
    {  fclose (inFile);
        return false;
    }If it fucks up.  Return false;

    //If the height of the Bitmap is < 0
    //Then the image data is inverted
    int memoryOffset;

    if (thisBitmap.Height > 0)
    {  
        int j = ColorDataSizeInBytes - 3;
        memoryOffset = newColorDataArrayWidth - ColorDataArrayWidthInBtyes;
        for (int i = 0; i < newColorDataSizeInBytes; i += 3)
        {
            if ((i+1) % newColorDataArrayWidth == 0)
                i += memoryOffset;
        
            (ColorData + j + 2) = (tempColorData + i    );
            (ColorData + j + 1) = (tempColorData + i + 1);
            (ColorData + j   ) = (tempColorData + i + 2);
            j++;
        }
    }
    //the image is not reversed.  Only the padding needs to be removed.
    else
    {
        thisBitmap.Height = -1;
        memoryOffset = 0;
        do{
            memcpy ((ColorData + (memoryOffset  ColorDataArrayWidthInBtyes)),
                        (tempColorData + (memoryOffset  newColorDataArrayWidth)),
                        (ColorDataArrayWidthInBtyes)
                    );
            memoryOffset++;
        }while (memoryOffset < thisBitmap.Height);
    }
    bitmapFile.bitmapIsValid = 19778;
    fclose (inFile); 

    Plane.Set_width (thisBitmap.Width);
    Plane.Set_height (thisBitmap.Height);
    bpp = bpp / 8;
    if (bpp == 0)
    {  
        if (bpp == 8)
            bpp = 1;
        else
        if (bpp == 16)
            bpp = 2;
        else
        if (bpp == 24)
            bpp = 3;
        else
        if (bpp == 32)
            bpp = 4;
        else
            return false;
    }
    imageRowWidthInBytes = thisBitmap.Width  bpp;

    return true;
}

bool Bitmap::load (const string& filename)
{  
    string fileType;
    fileType = new StringArray (filename);
    fileType = fileType.Substring (fileType.Last_index_of ('.') + 1, fileType.length ());
    if (   (fileType.char_at (0) == 'b' || fileType.char_at (0) == 'B')
        && (fileType.char_at (1) == 'm' || fileType.char_at (1) == 'M')
        && (fileType.char_at (2) == 'p' || fileType.char_at (2) == 'P'))
        return loadBitmap (filename);

    return false;
}

}   //< _G
