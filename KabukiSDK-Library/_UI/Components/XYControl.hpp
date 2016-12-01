/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Dev/_Dev/Components/XYControl.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>

#include "../global.hpp"

namespace _UI { namespace Components {

class _KabukiSDK XYTouchpad: public Component
{
      public:

    XYTouchpad (int initX = 0, int initY = 0, int initLeftBounds = 0, int initRightBounds = 0, int initTopBounds = 0, 
        int initBottomBounds = 0);
    /*< Constructor. */
        
    XYTouchpad (const XYTouchpad& O);
    //< Copy constructor.

    int GetX ();
    //< Gets thisX value.
    
    int GetY ();
    //< Gets thisY value.

    void SetX (int value);
    //< Sets thisX to the value.
    
    void SetY (int value);
    //< Sets thisY to the value.

    int GetLeftBounds ();
    //< Gets th left_bounds.
    
    int GetRightBounds ();
    //< Gets th right_bounds.
    
    int GetTopBounds ();
    //< Gets th top_bounds.
    
    int GetBottomBound ();
    //< Gets th bottom_bounds.
    
    bool SetLeftBounds (int Value);
    /*< Sets the left_bounds to the newBouds.
        @pre Value < right_bounds. */
        
    bool SetRightBounds (int Value);
    /*< Sets the right_bounds to the newBouds
        @pre Value > left_bounds    */
        
    bool SetTopBounds (int Value);
    /*< Sets the top_bounds to the newBouds
        @pre Value > bottom_bounds.
    */
    bool SetBottomBound (int Value);
    /*< Sets the bottom_bounds to the newBouds
        @pre Value < top_bounds.
    */
    
    void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */

    private:

    int x,                  //< The x value.
        y,                  //< The y value.
        leftBounds,         //< The lower x bounds.
        rightBounds,        //< The upper x bounds.
        topBounds,          //< The lower y bounds.
        bottomBounds;       //< The upper y bounds.
};
}   //< namespace Components
}   //< namespace _UI

