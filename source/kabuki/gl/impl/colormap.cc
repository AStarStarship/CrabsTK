/** Kabuki Theater
    @file       /.../Source/KabukiTheater-Impl/_G/Char.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once

#include "Color.h"

using namespace _G;

Colormap::Colormap () :
    Image ()
{
}
void Colormap::Create (int Width, int Height, Color BGColor)
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

Color Colormap::getPixel (int XCoordinate, int YCoordinate)
{
    if (XCoordinate > Plane.Width || YCoordinate > Plane.Height) return null;

    return colorMap[XCoordinate][YCoordinate];
}

bool Colormap::setPixel (int XCoordinate, int YCoordinate, Color AColor)
{
    if (XCoordinate > (int)Plane.Width || YCoordinate > (int)Plane.Height) return false;
    
    colorMap[XCoordinate][YCoordinate].Set (AColor);
    return true;
}
void Colormap::draw (Cell& C)
{
    Image.draw (context, this, Rectangle.Left, Rectangle.Top);
}
void Colormap::draw (Cell& C, int leftEdge, int topEdge)
{
    Image.draw (C, this, leftEdge, topEdge);
}

/** bool Colormap::LoadGLTexture ()
{
    glGenTextures (1, &texture[0]);
    glBindTexture (GL_TEXTURE_2D, texture[0]);
    glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D (GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
}*/
