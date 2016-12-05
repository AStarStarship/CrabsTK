/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Char.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#pragma once

#include "Color.hpp"

namespace _G {

class _KabukiSDK Colormap : Image
/*<  */
{
    public:

    Colormap ();
    
    void Create (int width, int height, Color bgColor);
    
    void Update ();

    Color GetPixel (int x, int y);
    
    bool SetPixel (int x, int y, Color color);
    
    void Draw (Cell& c);
    
    void Draw (Cell& c, int leftEdge, int topEdge);
    
    //bool LoadGLTexture ();
    
    void Print (Terminal& Slot);
    /*< Prints this object to a terminal. */

    private:

    bool isLoaded;
    int bpp;

    Color* colorMap;
};

}   //< _G
