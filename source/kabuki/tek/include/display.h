/** Kabuki Tek
    @file       /.../Source/tek/display/display.h
    @author     Cale McCollough <cale.mccollough@gmail.com>
    @license    Copyright (C) 2017 Cale McCollough <calemccollough.github.io>

                        All rights reserved  (R).

        This program is free software: you can redistribute it and/or modify it 
        under the terms of the GNU General Public License as published by the 
        Free Software Foundation, either version 3 ofthe License, or  (at your 
        option) any later version.

        This program is distributed in the hope that it will be useful, but 
        WITHOUT ANY WARRANTY; without even the implied warranty of 
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
        General Public License for more details.

        You should have received a copy of the GNU General Public License 
        along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
 
#pragma once

#include <tek/config.h>

namespace tek { 

class Controller;

namespace displays {

/** A Display.
    
*/
class Display: public I2P::Device
{
      public:
    
    Display () = 0;
    
    virtual ~Display () = 0;

    void Update () = 0;
    
    /*< I2P operations. */
    const Member* Op (byte index, Uniprinter* io) override;

    private:


};  
}   //< namespace Displays
}   //< namespace tek

