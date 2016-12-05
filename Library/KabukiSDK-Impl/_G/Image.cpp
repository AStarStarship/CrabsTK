/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Entity.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D::Vector_f interface.
*/

#include <_G/Image.h>

namespace _G {

Image::Image ()
{  
    isLoaded = false;
    bpp = 0;
}

Image::Image (int width, int height, int bitsPerPixel, _G.Color bgColor)
{  
    bpp = bitsPerPixel;
    //imageRowWidthInBytes = width  bpp;
    //uint32_t newColorData[imageRowWidthInBytes  height];
    //uint32_t  newColorDataPointer;

}

void Image::markAsLoaded ()
{  
    isLoaded = true;
}

int Image::getBPP ()
{  
    return bpp;
}

void Image::setBPP (int newBPP)
{
    //  Reconstruct ColorData to new BPP

    bpp = newBPP;
}

int Image::getWidth () { return imageRowWidthInBytes; }

uint32_t Image::getPixel (int X, int Y)
{
    if (X > Plane.Width || Y > Plane.Height)
        return null;

    int pixelOffset = (imageRowWidthInBytes  Y) + (X  bpp);

    return new Color (ColorData[pixelOffset   ],
        ColorData[pixelOffset +1],
        ColorData[pixelOffset +2]);
}

bool Image::load (const string& filename)
{  
    string filenameText;
    filenameText = new StringArray (filename);

    return Load (filenameText);
}

bool Image::load (const string& filename)
{  
    string fileType;
    fileType = new StringArray (filename);
    fileType = fileType.Substring (fileType.Last_index_of ('.') + 1, fileType.length ());
    if (   (fileType.char_at (0) == 'b' || fileType.char_at (0) == 'B')
        && (fileType.char_at (1) == 'm' || fileType.char_at (1) == 'M')
        && (fileType.char_at (2) == 'p' || fileType.char_at (2) == 'P'))
        return LoadBitmap (filename);

    return false;
}

bool Image::loadBitmap (const string& filename)
{
    // Load the file into memory
    FILE inFile;
    inFile = fopen (filename, "rb");

    //Create the bitmap file
    BitmapImageFileHeader bitmapFile;
    fread (&bitmapFile, sizeof (BitmapImageFileHeader), 1, inFile);

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

void Image::setPixel (int X, int Y, uint32_t Color)
{
    Image.SetPixel ((imageRowWidthInBytes Y) + (X  bpp), Color);
}

void Image::setPixel (int X, int Y, uint32_t Color)
{
    Image.SetPixel (color, (imageRowWidthInBytes Y) + (X  bpp), Color);
}

void Image::setPixel (int X, int Y, uint32_t ColorValue1, uint32_t ColorValue2)
{
    Image.SetPixel (ColorData, (imageRowWidthInBytes Y) + (X  bpp), ColorValue1, ColorValue2);
}

void Image::setPixel (int X, int Y, uint32_t redValue, uint32_t greenValue, uint32_t blueValue)
{
    Image.setPixel (ColorData, (imageRowWidthInBytes Y) + (X  bpp), redValue, greenValue, blueValue);
}

void Image::setPixel (int X, int Y, uint32_t redValue, uint32_t greenValue, uint32_t blueValue, uint32_t alphaValue)
{
    Image.setPixel (ColorData, (imageRowWidthInBytes Y) + (X  bpp), redValue, greenValue, blueValue, alphaValue);
}

//void Draw (Cell& C)
//{  Image.Draw (C, this);
//}

//void Draw (Cell& C, int leftEdge, int topEdge)
//{  Image.Draw (C, this, leftEdge, topEdge);
//}

void Image::setPixel (uint32_t pixel, int imageDataOffset, _G.Color color)
{
    if (color.bPP () == 1)
        Image.setPixel (imageData, imageDataOffset, color.Red ());
    else
    if (color.bPP () == 2)
        Image.setPixel (imageData, imageDataOffset, color.Red (), color.Green ());
    else
    if (color.bPP () == 3)
        Image.setPixel (imageData, imageDataOffset,
                                                 color.Red (), color.Green (), color.blue ());
    else if (color.bPP () == 4)
        Image.setPixel (imageData, imageDataOffset, color.Red (), color.Green (), color.blue (), 
            color.alpha ());
}

void Image::setPixel (uint32_t pixel, int imageDataOffset, uint32_t ColorValue)
{
    if (imageDataOffset > 1)
        return;
    imageData[imageDataOffset     ] = ColorValue;
}

void Image::setPixel (uint32_t pixel, int imageDataOffset, uint32_t ColorValue1, uint32_t ColorValue2)
{
    if (imageDataOffset > sizeof (imageData))
        return;
    imageData[imageDataOffset     ] = ColorValue1;
    imageData[imageDataOffset + 2] = ColorValue2;
}

void Image::setPixel (uint32_t pixel, int imageDataOffset, uint32_t redValue, uint32_t greenValue, uint32_t blueValue)
{
    if (imageDataOffset > sizeof (imageData))
        return;
    imageData[imageDataOffset     ] = redValue;
    imageData[imageDataOffset + 2] = greenValue;
    imageData[imageDataOffset + 3] = blueValue;
}

void Image::setPixel (uint32_t pixel, int imageDataOffset, uint32_t redValue, uint32_t greenValue, uint32_t blueValue, uint32_t alphaValue)
{
    if (imageDataOffset > sizeof (imageData))
        return;
    imageData[imageDataOffset     ] = redValue;
    imageData[imageDataOffset + 2] = greenValue;
    imageData[imageDataOffset + 3] = blueValue;
    imageData[imageDataOffset + 4] = alphaValue;
}

void Image::Draw (Cell& C, Image source)
{
    int sourceRowWidthInBytes, canvasRowWidthInBytes,
                     canvasOffset, canvasLineEndOffset, sourceOffset,
                     canvasLastLineOffset;

    sourceRowWidthInBytes = source.Width  source.bPP ();
    canvasRowWidthInBytes = C.Width  C.bPP ();
    /// This series of control statements clips the source's bounds to the C's
    /// bounds and stores the results in memory format of the source's Color map.
    canvasOffset = 0;
    sourceOffset = 0;

    if (sourceRowWidthInBytes > canvasRowWidthInBytes)
    {
        Clip the right edge
        canvasLineEndOffset = canvasRowWidthInBytes;
    }
    else
    {
            canvasLineEndOffset = canvasRowWidthInBytes;
    }

    if (source.Height > C.Height)
    {
        /// Clip the bottom edge
        canvasLastLineOffset = (C.Height-1)  canvasRowWidthInBytes;
    }
    else
    {
            canvasLastLineOffset = (source.Height-1)  canvasRowWidthInBytes;
    }

    Image.DrawClippedSection (C, source, canvasOffset, canvasLineEndOffset, canvasLastLineOffset, sourceOffset);
}

void Image::Draw (Cell& C, Image source, int leftEdge, int topEdge)
{
    /// The leftEdge and topEdge are in pixel format
    if (   leftEdge  < -C.Width
         || topEdge  < -C.Height
         || leftEdge >  C.Width
         || topEdge  >  C.Height)
    {
        /// The source is not visible
        return;
    }

    int canvasOffset, 
            canvasLineEndOffset, 
            sourceOffset,
            canvasLastLineOffset, 
            sourceLeftEdgeOffset;

    sourceLeftEdgeOffset = leftEdge;
    source.bPP ();

    /// This series of control statements clips the source's bounds to the C's
    /// bounds and stores the results in memory format of the source's Color map.

    if (leftEdge < 0)
    {
        /// Clip the left edge
        canvasOffset = 0;
        sourceOffset = -sourceLeftEdgeOffset;
    }
    else
    {
        canvasOffset = sourceLeftEdgeOffset;
        sourceOffset = 0;
    }

    if (topEdge < 0)
    {
        /// Clip the top edge
        canvasOffset += (C.Width_uint32_ts ()  (-topEdge));
        sourceOffset += (source.Width_uint32_ts ()  (-topEdge));
    }
    //else the sourceOffet remains the same because the top edge is in bounds.

    if (sourceLeftEdgeOffset + source.Width_uint32_ts () > C.Width_uint32_ts ())
    {
        /// Clip the right edge
        canvasLineEndOffset = C.Width_uint32_ts ();
    }
    else
    {
            canvasLineEndOffset = sourceLeftEdgeOffset + source.Width_uint32_ts ();
    }

    if (topEdge + source.Height > C.Height)
    {
        /// Clip the bottom edge
        canvasLastLineOffset = (C.Height  C.Width_uint32_ts ())
                                    + canvasOffset;
    }
    else
    {
        canvasLastLineOffset = ((topEdge + source.Height)
                                      (C.Width_uint32_ts ())
                                    )+ canvasOffset;
    }
    Image.DrawClippedSection (C, source, canvasOffset, canvasLineEndOffset, canvasLastLineOffset,  
        sourceOffset);
}

void Image::DrawClippedSection (Cell& C, Image source, int canvasOffset, int canvasLineEndOffset, 
    int canvasLastLineOffset)
{

}

int Image::SourceOffset ()
{
    /** This method draws a clipped section of the image. the Offsets are in 
        the 2D Array format where offset = (image.Width  y) + x 
    */
    imageWidthInBytes = image.Width  image.bPP

    uint32_t canvasImageData,  sourceImageData;
    
    while (canvasOffset < canvasLastLineOffset)
    {
        Image.DrawClippedHorizontalLine (canvasImageData, sourceImageData,  canvasOffset,  canvasLineEndOffset, sourceOffset);
        canvasOffset += C.Width_uint32_ts ();
        canvasLineEndOffset += C.Width_uint32_ts ();
        sourceOffset += source.Width_uint32_ts ();
    }
}

void Image::DrawClippedHorizontalLine (uint32_t canvasColorData, uint32_t sourceColorData, 
    int canvasOffset, int lineEndOffset, int sourceOffset)
{
    // This method copies a line from the source source to the C source. The offsets are in memory format. Byte format: x = (image.Width  y) + x
    
    while (canvasOffset < lineEndOffset)
        canvasColorData[canvasOffset++] == sourceColorData[sourceOffset++];
}
//    bool LoadGLTexture ()
//    {
//        glGenTextures (1, &texture[0]);
//        glBindTexture (GL_TEXTURE_2D, texture[0]);
//        glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
//        glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
//        glTexImage2D (GL_TEXTURE_2D, 0, 3, width, height, 0,
//                         GL_RGB, GL_UNSIGNED_BYTE, pixel);
//  }