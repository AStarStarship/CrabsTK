/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_G/Colormap.h
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

namespace _G {

class _KabukiTheater_ Colormap : Image
/*<  */
{
    public:

    Colormap ();
    
    void create (int width, int height, Color bgColor);
    
    void update ();

    Color getPixel (int x, int y);
    
    bool setPixel (int x, int y, Color color);
    
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
