/** Kabuki Theater
    @file    /.../Source/_Theater/HMI/Buttons/SwapButton.hpp
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


#include <KabukiTheater-Config.hpp>
#include "../Button.hpp"

namespace _Theater { namespace HMI  { namespace Buttons {

/*  @class _KabukiTheater_   SwapButton
    @brief   A Button that cycles a Control's functionality through various different modes.
    @details The primary purpose of this type of control is for instance that you have a Knob that has a Button
    that you can press that changes the functionality of the knob through a cycle of different parameters.
 */

class _KabukiTheater_ SwapButton: public Controls::Button
{
    public:

    // Constructor.
    SwapButton  (const char* initName=T::empty);
    SwapButton  (const SwapButton& thisPage);
    //< Copy constructor.

   ~SwapButton ();
   //< Destructor.
    
    void print (I2P::Terminal& io);
    /*< Prints this object to a terminal. */
    
    private:

    uint32_t mode;                        //< The index of the current Button*

    Array<::ISymmetric::_Dev::Button*> control_modes;   //< An array of the different control_modes.
};

}   //< namespace Buttons
}   //< namespace _Theater { namespace HMI 

