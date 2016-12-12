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
    
    void create (int width, int height, Color bgColor);
    
    void update ();

    Color getPixel (int x, int y);
    
    bool SetPixel (int x, int y, Color color);
    
    void draw (Cell& c);
    
    void draw (Cell& c, int leftEdge, int topEdge);
    
    //bool LoadGLTexture ();
    
    void print (Terminal& io);
    /*< Prints this object to a terminal. */

    private:

    bool isLoaded;
    int bpp;            //< The bits per pixel.
    Color* colorMap;    //< An array of colors.
    const char* 
};

}   //< _G
