/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Char.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D::Vector_f interface.
*/

#include "_G/Texture.hpp"
using namespace _G;

Texture::Texture () :
    Image ()
{
}

void Texture::Create (int width, int height, _G.Color backgroundColor)
{
    int x, y;
    //colorMap = new Color[width][height];
    colorMap[width][height];
    for (y = 0; y < height; y++) ;
    for (x = 0; x < width; x++) ;
    colorMap[x][y] = new Color (backgroundColor);
    Plane.Set_width ((int)width);
    Plane.Set_height ((int)height);
    isLoaded = true;
}

void Texture::Update ()
{
    colorMap[Plane.Width]
        [Plane.Height];
    uint pixel = Image.color_data ();
    int pixelOffset = 0;
    for (int y = 0; y < Plane.Height; y++)
    {
        for (int x = 0; x < Plane.Width; x++)
        {
            colorMap[x][y] = new Color (imageData[pixelOffset++],
                imageData[pixelOffset++],
                imageData[pixelOffset++]);
        }
    }
}

Color Texture::Pixel (int xCoordinate, int yCoordinate)
{
    if (xCoordinate > Plane.Width
      || yCoordinate > Plane.Height
    ) return null;

    return colorMap[xCoordinate][yCoordinate];
}
bool Texture::setPixel (int xCoordinate, int yCoordinate, Color newColor)
{
    if (xCoordinate > (int)Plane.Width
      || yCoordinate > (int)Plane.Height
    ) return false;
    colorMap[xCoordinate][yCoordinate].Set (newColor);
    return true;
}
void Texture::draw (Cell& c)
{  
    //Image.Draw (C, this,
    //                         Rectangle.Left,
    //                         Rectangle.Top);
}

void Texture::Draw (Cell& c, int LeftEdge, int TopEdge)
{
    Image.Draw (C, this, LeftEdge, TopEdge);
}

//   bool LoadGLTexture ()
//   {  
//      glGenTextures (1, &texture[0]);
//      glBindTexture (GL_TEXTURE_2D, texture[0]);
//      glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
//      glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
//      glTexImage2D (GL_TEXTURE_2D, 0, 3, width, height, 0,
//                   GL_RGB, GL_UNSIGNED_BYTE, pixel);
//  }
