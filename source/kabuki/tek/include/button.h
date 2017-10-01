/** Kabuki Tek
    @file    /.../Source/tek/sensors/Button.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

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
 
#ifndef KABUKI_TEK_SENSOR_BUTTON
#define KABUKI_TEK_SENSOR_BUTTON

#include <KabukiTek\Config.h>


namespace tek { namespace sensors {

class Button
/** A button.
    Buttons are composed of a single integer type that stores the bit offset from the Controller. This bit offset then 
    gets translated to a byte offset and a mask through some bit shifting.
*/
{
      public:
    
    FunctionPointer PressFunction,      //< Function pointer for when the button is pressed.
        DepressFunction,                //< Function pointer for when the button is depressed.
        DoublePressFunction,            //< Function pointer for when the button is depressed.
    
    Button  (uint bit);
    /*< Constructs a button with the given bit offset. */
    
    inline byte getState  (byte* digitalIns);
    /*< Gets the state of the button. */
    
    inline void Poll  (byte* debouncedXOR);
    /*< Polls the state of the switch using the debounced XOR. */
    
    private:
    
    uint bitNumber;     //< The bit number in the packed button array.
};
}   	//< namespace sensors
}   	//< namespace tek
#endif	//< KABUKI_TEK_SENSOR_BUTTON