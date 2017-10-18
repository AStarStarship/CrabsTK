/** Kabuki Starship
    @file    ~/Source/_hmi/Buttons/SwapButton.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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


#include <KabukiTheater-Config.h>
#include "../IButton.h"

namespace _hmi  { namespace Buttons {

/*  @class _KabukiTheater_   SwapButton
    @brief   A Button that cycles a Control's functionality through various different modes.
    @details The primary purpose of this type of control is for instance that you have a Knob that has a Button
    that you can press that changes the functionality of the knob through a cycle of different parameters.
 */

class _KabukiTheater_ SwapButton: public _hmi::Button
{
    public:

    SwapButton  (const char* initName=T::empty);
    /*< Constructor. */
	
    SwapButton  (const SwapButton& thisPage);
    //< Copy constructor.

   ~SwapButton ();
   //< Destructor.
    
    void print () const;
    /*< Prints this object to the stdout. */
    
    private:

    uint32_t mode;                        //< The index of the current Button*

    std::vector<_hmi::IButton*> controlModes;   //< An array of the different control_modes.
};

}   //< Buttons
}   //< _hmi

