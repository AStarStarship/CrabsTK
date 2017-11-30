/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/swap_button.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/
 
#ifndef HEADER_FOR_KABUKI_HMI_SWAPBUTTON
#define HEADER_FOR_KABUKI_HMI_SWAPBUTTON

#include "button.h"

namespace kabuki  { namespace hmi {

/** A Button that cycles a Control's functionality through various different modes.
    @details The primary purpose of this type of control is for instance that you have a Knob that has a Button
    that you can press that changes the functionality of the knob through a cycle of different parameters.
 */
class KABUKI SwapButton: public Button {
    public:

    /** Constructor. */
    SwapButton  (const char* initName=T::empty);
    
    /** Copy constructor. */
    SwapButton  (const SwapButton& page);

   /** Destructor. */
   ~SwapButton ();
    
    /** Prints this object to the stdout. */
    void Print () const;
    
    private:

    uint32_t mode;                       //< Index of the current Button.
    std::vector<Button*> control_modes_; //< Array of control mode buttons.
};

}       //< hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_SWAPBUTTON
