/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/portal.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
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

#ifndef KABUKI_SERIAL_SERIAL_PORTAL_H
#define KABUKI_SERIAL_SERIAL_PORTAL_H

#include "../../script/include/portal.h"
#include "serial.h"

namespace serial {

/** A portal that connects two Chinese Rooms through a UART connection.
    
*/
class KABUKI SerialPortal {

    /** Constructs a SerialPortal. */
    SerialPortal ();

    /** Sets the Portal up for a batch of bytes transfer.
         Implementation of this function is not required to do anything, but 
         regardless it will be called by the Set. */
    virtual void Prime () {

    }

    /** Gets the length of current portal.
        @warning Length might not be the actual length, but rather the length
                 of the data that is read to be pulled. */
    virtual uint_t Length () {
        return 0;
    }

    /** Feeds tx messages through the a without scanning them. */
    virtual void Feed (byte b) {

    }

    /** Pulls rx messages through the a and runs them through the scanner. */
    virtual byte Pull () {
        return 0;
    }
};      //< class SerialPortal
}       //< namespace serial
#endif  //< KABUKI_SERIAL_SERIAL_PORTAL_H
