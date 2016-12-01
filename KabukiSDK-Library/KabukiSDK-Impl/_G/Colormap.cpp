/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Char.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#pragma once

#include "Color.h"

using namespace _G;

Colormap::Colormap () :
    Image ()
{
}
void Colormap::Create (int Width, int Height, Color_i BGColor)
{
    int x, Y;
    //colorMap = new Color[width][height]; colorMap[width][height];
    for (Y = 0; Y < height; Y++) ;
    for (x = 0; x < width; x++) ;
    colorMap[x][Y].Set (backgroundColor);
    Plane.SetWidth ((int)width);
    Plane.SetHeight ((int)height);
    isLoaded = true;
}

void Colormap::Update ()
{   
    c[Plane.Width][Plane.Height];
    
    uint32_t pixel = Image.GetColorData ();
    int pixelOffset = 0;
    for (int Y = 0; Y < Plane.Height; Y++)
    {
        for (int x = 0; x < Plane.Width; x++)
        {
            colorMap[x][Y] = new Color (imageData[pixelOffset++],
                imageData[pixelOffset++],
                imageData[pixelOffset++]);
        }
    }
}

Color Colormap::GetPixel (int XCoordinate, int YCoordinate)
{
    if (XCoordinate > Plane.Width || YCoordinate > Plane.Height) return null;

    return colorMap[XCoordinate][YCoordinate];
}

bool Colormap::SetPixel (int XCoordinate, int YCoordinate, Color AColor)
{
    if (XCoordinate > (int)Plane.Width || YCoordinate > (int)Plane.Height) return false;
    
    colorMap[XCoordinate][YCoordinate].Set (AColor);
    return true;
}
void Colormap::Draw (Cell& C)
{
    Image.Draw (context, this, Rectangle.Left, Rectangle.Top);
}
void Colormap::Draw (Cell& C, int leftEdge, int topEdge)
{
    Image.Draw (C, this, leftEdge, topEdge);
}

/** bool Colormap::LoadGLTexture ()
{
    glGenTextures (1, &texture[0]);
    glBindTexture (GL_TEXTURE_2D, texture[0]);
    glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D (GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
}*/
