/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Char.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#pragma once

#include "Color.h"

namespace _G {
/**  */
class _G_API Colormap : Image
{
    public:
    
    bool isLoaded;
    int bpp;

    Color_i* colorMap;

    Colormap ();
    
    void Create (int Width, int Height, Color_i BGColor);
    
    void Update ();

    Color GetPixel (int X, int Y);
    
    bool SetPixel (int X, int Y, Color AColor);
    
    void Draw (Cell& C);
    
    void Draw (Cell& C, int leftEdge, int topEdge);
    
    //bool LoadGLTexture ();
    
    void Print (I2P::Terminal& Slot);
    /*< Prints this object to a terminal. */
}
}
